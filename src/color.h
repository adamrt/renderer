#pragma once

#include <cstdint>

auto color_rgb(int r, int g, int b) -> uint32_t;

namespace Colors {
const uint32_t Black = color_rgb(0, 0, 0);
const uint32_t White = color_rgb(255, 255, 255);
const uint32_t Red = color_rgb(255, 0, 0);
const uint32_t Green = color_rgb(0, 255, 0);
const uint32_t Blue = color_rgb(0, 0, 255);
} // namespace Colors
