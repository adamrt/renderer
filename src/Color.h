#pragma once

#include "imgui.h"

#include "AK.h"

struct Color {
    u32 rgba;

    Color();
    Color(u32 color);
    Color(ImVec4 color);

    Color operator+(const Color& rhs) const;
    Color operator*(const Color& rhs) const;
    Color operator*(f32 factor) const;

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
