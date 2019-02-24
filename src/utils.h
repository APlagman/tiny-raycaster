#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "dimension2D.h"

class FrameBuffer;
struct RGBA;

uint32_t packColor(const RGBA& rgba);
RGBA unpackColor(uint32_t color);
void writePPMImage(const std::string& filename, const FrameBuffer& image);