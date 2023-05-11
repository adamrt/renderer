#pragma once

#include <cstdint>

auto color_new(int r, int g, int b, int a = 255) -> uint32_t;

namespace Colors {
const uint32_t Black = color_new(0, 0, 0);
const uint32_t White = color_new(255, 255, 255);
const uint32_t Red = color_new(255, 0, 0);
const uint32_t Green = color_new(0, 255, 0);
const uint32_t Blue = color_new(0, 0, 255);
} // namespace Colors
