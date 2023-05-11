#pragma once

#include <cstdint>

struct Color {
    Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a){};
    Color(int r, int g, int b) : r(r), g(g), b(b), a((uint8_t)255){};

    [[nodiscard]] auto sdl() const -> uint32_t { return (r << 24) | (g << 16) | (b << 8) | a; }

    uint8_t r, g, b, a;
};

namespace Colors {
const auto Black = Color(0, 0, 0);
const auto Red = Color(255, 0, 0);
const auto Green = Color(0, 255, 0);
const auto Blue = Color(0, 0, 255);
} // namespace Colors
