#include "color.h"

auto color_rgb(int r, int g, int b) -> uint32_t { return (r << 24) | (g << 16) | (b << 8) | 255; }
