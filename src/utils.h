#pragma once

#include "dimension2D.h"

#include <cstdint>
#include <string>
#include <vector>

class FrameBuffer;
struct RGBA;

uint32_t packColor(const RGBA& rgba);
RGBA unpackColor(uint32_t color);
void writePPMImage(const std::string& filename, const FrameBuffer& image);