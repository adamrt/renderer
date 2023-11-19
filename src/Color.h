#pragma once

#include <cstdint>

struct Color {
    uint32_t rgba;

    Color();
    Color(uint32_t color);

    Color operator*(float factor) const;

    static const Color Black;
    static const Color White;
    static const Color DarkGray;
    static const Color LightGray;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Purple;
};
