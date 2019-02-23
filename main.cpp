#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "framebuffer.h"
#include "map.h"
#include "player.h"
#include "rgba.h"
#include "textures.h"
#include "utils.h"

void render(FrameBuffer& fb, const Map& map, const Player& player, const TextureSet& wallTextures)
{
    const Dimension2D<size_t> tileSize = { (fb.getWidth() / 2) / map.getWidth(), fb.getHeight() / map.getHeight() };

    // Draw the map
    for (size_t row = 0; row < map.getHeight(); ++row) {
        for (size_t col = 0; col < map.getWidth(); ++col) {
            if (map.isEmpty({ col, row }))
                continue;
            const Point2D<size_t> topLeft = { col * tileSize.width, row * tileSize.height };
            const size_t textureID = map.get({ col, row });
            assert(textureID < wallTextures.getCount());
            fb.drawRectangle(topLeft, tileSize, wallTextures.get({ 0, 0 }, textureID));
        }
    }
    
    // Field of view sweep
    for (size_t i = 0; i < fb.getWidth() / 2; ++i) {
        const float angle = (player.getAngle() - (player.getFieldOfView() / 2)) + player.getFieldOfView() * (i / static_cast<float>(fb.getWidth() / 2));

        // Ray march
        for (float t = 0; t < 20; t += 0.01) {
            const Point2D<float> along = { player.getX() + t * std::cos(angle), player.getY() + t * std::sin(angle) };
            
            const Point2D<size_t> mapPos = { static_cast<size_t>(along.x), static_cast<size_t>(along.y) };

            // Draw the ray
            const Point2D<size_t> nearest = { static_cast<size_t>(mapPos.x * tileSize.width), static_cast<size_t>(mapPos.y * tileSize.height) };
            fb.setPixel(nearest, packColor({ 160, 160, 160 }));

            if (!map.isEmpty(mapPos)) {
                // Draw the wall
                const size_t textureID = map.get(mapPos);
                assert(textureID < wallTextures.getCount());

                const size_t columnHeight = fb.getHeight() / (t * std::cos(angle - player.getAngle()));

                // Find the fractional distance along the tile we hit
                const Point2D<float> mapTileOffset = { along.x - std::round(along.x), along.y - std::round(along.y) };
                // Multiply by the texture width to get the offset within the texture for that tile
                int textureXOffset = static_cast<int>(mapTileOffset.x * wallTextures.getTextureWidth());
                if (std::abs(mapTileOffset.y) > std::abs(mapTileOffset.x)) {
                    // Assume we hit a vertical wall (in the map)
                    textureXOffset = static_cast<int>(mapTileOffset.y * wallTextures.getTextureWidth());
                }
                if (textureXOffset < 0) {
                    // If we're more than halfway right (horizontal) or down (vertical), mapTileOffset would have been negative.
                    textureXOffset += wallTextures.getTextureWidth();
                }
                assert(textureXOffset >= 0 && textureXOffset < static_cast<int>(wallTextures.getTextureWidth()));

                // Load the column texture
                std::vector<uint32_t> column = wallTextures.getScaledColumn(textureID, static_cast<size_t>(textureXOffset), columnHeight);
                
                // Draw the column
                Point2D<size_t> pixel = { fb.getWidth() / 2 + i, 0 };
                for (size_t y = 0; y < columnHeight; ++y) {
                    pixel.y = y + fb.getHeight() / 2 - columnHeight / 2;
                    if (pixel.y >= 0 && pixel.y < fb.getHeight()) {
                        fb.setPixel(pixel, column[y]);
                    }
                }
                break;
            }
        }
    }
}

int main()
{
    const Dimension2D<size_t> windowSize = { 1024, 512 };
    FrameBuffer fb(std::move(windowSize));
    fb.clear(packColor(RGBA::white()));

    Player player({ 3.456, 2.345 }, 1.523, M_PI / 3.0);

    Map map;

    TextureSet wallTextures("walltext.png");
    if (wallTextures.getCount() == 0) {
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }

    for (size_t frame = 0; frame < 360; ++frame) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player.setAngle(player.getAngle() + 2 * M_PI / 360);

        render(fb, map, player, wallTextures);
        writePPMImage(ss.str(), fb);
    }

    return 0;
}