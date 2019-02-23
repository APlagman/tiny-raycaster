#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using FrameBuffer = std::vector<uint32_t>;
using Color = uint32_t;

struct RGBA
{
    using Type = uint8_t;
    Type r = 255;
    Type g = 255;
    Type b = 255;
    Type a = 255;

    static RGBA white() { return RGBA{}; };
};

struct PixelSize
{
    using Type = size_t;
    Type width;
    Type height;
};

struct Pixel
{
    using Type = size_t;
    Type x;
    Type y;
};

struct Point
{
    using Type = float;
    Type x;
    Type y;
};

struct Tile
{
    using Type = int;
    Type x;
    Type y;
};

using Radians = float;

Color packColor(const RGBA& color)
{
    return (color.a << 24) + (color.b << 16) + (color.g << 8) + color.r;
}

RGBA unpackColor(const Color color)
{
    RGBA result;
    result.r = (color >> 0) & 255;
    result.g = (color >> 8) & 255;
    result.b = (color >> 16) & 255;
    result.a = (color >> 24) & 255;
    return result;
}

void writePpmImage(const std::string& filename, const FrameBuffer& image, const PixelSize& size)
{
    assert(image.size() == size.width * size.height);
    std::ofstream ofs(filename, std::ofstream::out | std::ofstream::binary);
    ofs << "P6\n" << size.width << " " << size.height << "\n255\n";
    for (size_t i = 0; i < size.height * size.width; ++i) {
        const RGBA color = unpackColor(image[i]);
        ofs << static_cast<char>(color.r);
        ofs << static_cast<char>(color.g);
        ofs << static_cast<char>(color.b);
    }
    ofs.close();
}

void drawRectangle(FrameBuffer& image, const PixelSize& imageSize, const Pixel& topLeft, const PixelSize& rectSize, const Color color)
{
    assert(image.size() == imageSize.width * imageSize.height);
    for (size_t row = 0; row < rectSize.height; ++row) {
        for (size_t col = 0; col < rectSize.width; ++col) {
            const Pixel pixel{ topLeft.x + col, topLeft.y + row };
            if (pixel.x > imageSize.width || pixel.y > imageSize.height)
                continue;
            image[pixel.x + pixel.y * imageSize.width] = color;
        }
    }
}

int main()
{
    const PixelSize windowSize = { 1024, 512 };

    constexpr PixelSize mapSize = { 16, 16 };
    const char map[] = 
                       "0000222222220000"\
                       "1              0"\
                       "1      11111   0"\
                       "1     0        0"\
                       "0     0  1110000"\
                       "0     3        0"\
                       "0   10000      0"\
                       "0   0   11100  0"\
                       "0   0   0      0"\
                       "0   0   1  00000"\
                       "0       1      0"\
                       "2       1      0"\
                       "0       0      0"\
                       "0 0000000      0"\
                       "0              0"\
                       "0002222222200000";
    assert(sizeof(map) == mapSize.width * mapSize.height + 1);

    const Point playerPos = { 3.456, 2.345 };
    Radians playerAngle = 1.523;
    const float fieldOfView = M_PI / 3.0;

    std::vector<Color> colors(10);
    for (size_t i = 0; i < colors.size(); ++i) {
        const auto getRandomRGB = []() { return static_cast<RGBA::Type>(rand() % 255); };
        colors[i] = packColor({ getRandomRGB(), getRandomRGB(), getRandomRGB() });
    }

    const PixelSize tileSize = { (windowSize.width / 2) / mapSize.width, windowSize.height / mapSize.height };
    
    for (size_t frame = 0; frame < 360; ++frame) {
        std::cout << "Frame: " << frame << "\n";
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        playerAngle += 2 * M_PI / 360;

        FrameBuffer framebuffer(windowSize.width * windowSize.height, packColor(RGBA::white()));

        for (size_t row = 0; row < mapSize.height; ++row) {
            for (size_t col = 0; col < mapSize.width; ++col) {
                if (map[col + row * mapSize.width] == ' ')
                    continue;
                const Pixel topLeft = { col * tileSize.width, row * tileSize.height };
                const size_t icolor = map[col + row * mapSize.width] - '0'; // Each tile type has its own color
                assert(icolor < colors.size());
                drawRectangle(framebuffer, windowSize, topLeft, tileSize, colors[icolor]);
            }
        }

        for (size_t i = 0; i < windowSize.width / 2; ++i) {
            const Radians angle = (playerAngle - (fieldOfView / 2)) + fieldOfView * (i / static_cast<float>(windowSize.width / 2));
            for (float t = 0; t < 20; t += 0.01) {
                const Point along = { playerPos.x + t * std::cos(angle), playerPos.y + t * std::sin(angle) };
                
                const Tile mapPos = { static_cast<Tile::Type>(along.x), static_cast<Tile::Type>(along.y) };

                const Pixel nearest = { static_cast<Pixel::Type>(mapPos.x * tileSize.width), static_cast<Pixel::Type>(mapPos.y * tileSize.height) };
                framebuffer[nearest.x + nearest.y * windowSize.width] = packColor({ 160, 160, 160 });

                if (map[mapPos.x + mapPos.y * mapSize.width] != ' ') {
                    // Touching a wall
                    const size_t icolor = map[mapPos.x + mapPos.y * mapSize.width] - '0';
                    assert(icolor < colors.size());
                    const PixelSize::Type columnHeight = windowSize.height / (t * std::cos(angle - playerAngle));
                    const Pixel topLeft = { windowSize.width / 2 + i, windowSize.height / 2 - columnHeight / 2 };
                    drawRectangle(framebuffer, windowSize, topLeft, { 1, columnHeight }, colors[icolor]);
                    break;
                }
            }
        }

        writePpmImage(ss.str(), framebuffer, windowSize);
    }

    return 0;
}