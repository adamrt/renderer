#include <algorithm>

#include "Color.h"

Color::Color()
    : rgba { 0x000000FF }
{
}

Color::Color(u32 color)
    : rgba { color }
{
}

Color::Color(ImVec4 color)
    : rgba { 0x000000FF }
{
    uint8_t r = static_cast<uint8_t>(color.x * 255.0f + 0.5f);
    uint8_t g = static_cast<uint8_t>(color.y * 255.0f + 0.5f);
    uint8_t b = static_cast<uint8_t>(color.z * 255.0f + 0.5f);
    uint8_t a = static_cast<uint8_t>(color.w * 255.0f + 0.5f);

    rgba = (r << 24) | (g << 16) | (b << 8) | a;
}

Color Color::operator*(f32 factor) const
{
    if (factor < 0.0f) {
        factor = 0.0f;
    }
    if (factor > 1.0f) {
        factor = 1.0f;
    }

    // Extract RGBA components
    u8 red = (rgba >> 24) & 0xFF;
    u8 green = (rgba >> 16) & 0xFF;
    u8 blue = (rgba >> 8) & 0xFF;
    u8 alpha = rgba & 0xFF;

    // Scale each component
    red = std::min(static_cast<u32>(red * factor), 255u);
    green = std::min(static_cast<u32>(green * factor), 255u);
    blue = std::min(static_cast<u32>(blue * factor), 255u);

    return (red << 24) | (green << 16) | (blue << 8) | alpha;
}

const Color Color::Black = { 0x000000FF };
const Color Color::DarkGray = { 0x111111FF };
const Color Color::LightGray = { 0x555555FF };
const Color Color::White = { 0xFFFFFFFF };
const Color Color::Red = { 0xFF0000FF };
const Color Color::Green = { 0x00FF00FF };
const Color Color::Blue = { 0x0000FFFF };
const Color Color::Yellow = { 0xFFFF00FF };
const Color Color::Cyan = { 0x00FFFFFF };
const Color Color::Magenta = { 0xFF00FFFF };
const Color Color::Purple = { 0xA500FFFF };
