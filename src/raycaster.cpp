#include "raycaster.h"

#include "dimension2D.h"
#include "framebuffer.h"
#include "map.h"
#include "player.h"
#include "point2D.h"
#include "rgba.h"
#include "sprite.h"
#include "textures.h"
#include "utils.h"

#include <cassert>
#include <cmath>

namespace
{
    Dimension2D<size_t> getTileSize(const FrameBuffer& fb, const Map& map)
    {
        return {(fb.getWidth() / 2) / map.getWidth(),
                fb.getHeight() / map.getHeight()};
    }

    void drawSpriteOnMap(const Sprite& sprite, FrameBuffer& fb, const Map& map)
    {
        const Dimension2D<size_t> tileSize = getTileSize(fb, map);
        const Point2D<size_t> origin = {
            static_cast<size_t>(sprite.getX() * tileSize.width),
            static_cast<size_t>(sprite.getY() * tileSize.height)};
        fb.drawRectangle({origin.x - 3, origin.y - 3},
                         {6, 6},
                         packColor({255, 0, 0, 255}));
    }

    void drawSprite(const Sprite& sprite,
                    FrameBuffer& fb,
                    std::vector<float>& depthBuffer,
                    const Player& player,
                    const TextureSet& spriteTextures)
    {
        float spriteDirection = std::atan2(sprite.getY() - player.getY(),
                                           sprite.getX() - player.getX());
        // get within [-M_PI, M_PI]
        while (spriteDirection - player.getAngle() > M_PI) {
            spriteDirection -= 2 * M_PI;
        }
        while (spriteDirection - player.getAngle() < -M_PI) {
            spriteDirection += 2 * M_PI;
        }

        const float distanceToSprite =
            std::sqrt(std::pow(player.getX() - sprite.getX(), 2)
                      + std::pow(player.getY() - sprite.getY(), 2));
        const size_t spriteScreenHeight = static_cast<size_t>(std::min(
            1000, static_cast<int>(fb.getHeight() / distanceToSprite)));
        const size_t spriteScreenWidth = spriteScreenHeight;
        const float percentAlongFOV =
            (spriteDirection - player.getAngle()) / player.getFieldOfView();
        const size_t halfOfViewWidth = (fb.getWidth() / 2) / 2;
        const int leftSideOfTexture =
            halfOfViewWidth - spriteTextures.getTextureWidth() / 2;
        const Point2D<int> topLeftOfTexture{
            static_cast<int>(percentAlongFOV * (fb.getWidth() / 2))
                + leftSideOfTexture,
            static_cast<int>(fb.getHeight() / 2 - spriteScreenHeight / 2)};

        for (size_t col = 0; col < spriteScreenWidth; ++col) {
            const int screen3DViewX =
                topLeftOfTexture.x + static_cast<int>(col);
            if (screen3DViewX < 0
                || screen3DViewX >= static_cast<int>(fb.getWidth() / 2)) {
                continue;
            }
            for (size_t row = 0; row < spriteScreenHeight; ++row) {
                const int screen3DViewY =
                    topLeftOfTexture.y + static_cast<int>(row);
                if (screen3DViewY < 0
                    || screen3DViewY >= static_cast<int>(fb.getHeight())) {
                    continue;
                }

                const size_t depthBufferIndex =
                    screen3DViewX + screen3DViewY * (fb.getWidth() / 2);
                if (depthBuffer[depthBufferIndex] < distanceToSprite) {
                    continue; // Occluded
                }

                const Point2D<size_t> texturePixel = {
                    col * spriteTextures.getTextureWidth() / spriteScreenWidth,
                    row * spriteTextures.getTextureHeight()
                        / spriteScreenHeight};
                const uint32_t color =
                    spriteTextures.get(texturePixel, sprite.getID());
                RGBA rgba = unpackColor(color);
                if (rgba.a > 128) {
                    depthBuffer[depthBufferIndex] = distanceToSprite;
                    const Point2D<size_t> pixel = {
                        fb.getWidth() / 2 + static_cast<size_t>(screen3DViewX),
                        static_cast<size_t>(screen3DViewY)};
                    fb.setPixel(pixel, color);
                }
            }
        }
    }
} // namespace

void render(FrameBuffer& fb,
            const Map& map,
            const Player& player,
            const std::vector<Sprite>& sprites,
            const TextureSet& wallTextures,
            const TextureSet& monsterTextures)
{
    const Dimension2D<size_t> tileSize = getTileSize(fb, map);

    // Draw the map
    for (size_t row = 0; row < map.getHeight(); ++row) {
        for (size_t col = 0; col < map.getWidth(); ++col) {
            if (map.isEmpty({col, row})) {
                continue;
            }
            const Point2D<size_t> topLeft = {col * tileSize.width,
                                             row * tileSize.height};
            const size_t textureID = map.get({col, row});
            assert(textureID < wallTextures.getCount());
            fb.drawRectangle(topLeft,
                             tileSize,
                             wallTextures.get({0, 0}, textureID));
        }
    }

    std::vector<float> depthBuffer(fb.getWidth() / 2 * fb.getHeight(), 1e3);
    // Field of view sweep
    for (size_t i = 0; i < fb.getWidth() / 2; ++i) {
        const float angle = (player.getAngle() - (player.getFieldOfView() / 2))
                            + player.getFieldOfView()
                                  * (i / static_cast<float>(fb.getWidth() / 2));

        // Ray march
        for (float t = 0; t < 20; t += 0.01) {
            const Point2D<float> pointAlongRay = {
                player.getX() + t * std::cos(angle),
                player.getY() + t * std::sin(angle)};

            const Point2D<size_t> nearestTileOnMap = {
                static_cast<size_t>(pointAlongRay.x),
                static_cast<size_t>(pointAlongRay.y)};

            // Draw the ray
            const Point2D<size_t> nearestPixel = {
                static_cast<size_t>(pointAlongRay.x * tileSize.width),
                static_cast<size_t>(pointAlongRay.y * tileSize.height)};
            fb.setPixel(nearestPixel, packColor({160, 160, 160, 255}));

            if (map.isEmpty(nearestTileOnMap)) {
                continue;
            }

            // Draw the wall
            const size_t textureID = map.get(nearestTileOnMap);
            assert(textureID < wallTextures.getCount());

            const float distance = t * std::cos(angle - player.getAngle());
            for (size_t y = 0; y < fb.getHeight(); ++y) {
                depthBuffer[i + y * (fb.getWidth() / 2)] = distance;
            }
            const auto columnHeight =
                static_cast<size_t>(fb.getHeight() / distance);

            // Find the fractional offset from the top left of the tile we hit
            const Point2D<float> mapTileOffset = {
                pointAlongRay.x - std::round(pointAlongRay.x),
                pointAlongRay.y - std::round(pointAlongRay.y)};
            // Multiply by the texture width to get the offset within the
            // texture for that tile
            auto textureXOffset = static_cast<int>(
                mapTileOffset.x * wallTextures.getTextureWidth());
            if (std::abs(mapTileOffset.y) > std::abs(mapTileOffset.x)) {
                // Assume we hit a vertical wall (in the map)
                textureXOffset = static_cast<int>(
                    mapTileOffset.y * wallTextures.getTextureWidth());
            }
            if (textureXOffset < 0) {
                // If we're more than halfway right (horizontal) or down
                // (vertical), mapTileOffset would have been negative.
                textureXOffset += wallTextures.getTextureWidth();
            }
            assert(textureXOffset >= 0
                   && textureXOffset
                          < static_cast<int>(wallTextures.getTextureWidth()));

            // Load the column texture
            const std::vector<uint32_t> column = wallTextures.getScaledColumn(
                textureID, static_cast<size_t>(textureXOffset), columnHeight);

            // Draw the column
            Point2D<size_t> pixel = {fb.getWidth() / 2 + i, 0};
            for (size_t y = 0; y < columnHeight; ++y) {
                pixel.y = y + fb.getHeight() / 2 - columnHeight / 2;
                if (pixel.y < fb.getHeight()) {
                    fb.setPixel(pixel, column[y]);
                }
            }
            break;
        }
    }

    for (const Sprite& sprite : sprites) {
        drawSpriteOnMap(sprite, fb, map);
        drawSprite(sprite, fb, depthBuffer, player, monsterTextures);
    }
}