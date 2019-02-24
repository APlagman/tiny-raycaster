#pragma once

#include <vector>

class FrameBuffer;
class Map;
class Player;
class Sprite;
class TextureSet;

void render(FrameBuffer& fb,
            const Map& map,
            const Player& player,
            const std::vector<Sprite>& sprites,
            const TextureSet& wallTextures,
            const TextureSet& monsterTextures);