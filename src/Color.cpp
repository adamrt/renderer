#include <algorithm>

#include "Color.h"

Color::Color()
    : rgba { 0x000000FF }
{
}

Color::Color(uint32_t color)
    : rgba { color }
{
}

Color Color::operator*(float factor) const
{
    if (factor < 0.0f) {
        factor = 0.0f;
    }
    if (factor > 1.0f) {
        factor = 1.0f;
    }

    // Extract RGBA components
    uint8_t red = (rgba >> 24) & 0xFF;
    uint8_t green = (rgba >> 16) & 0xFF;
    uint8_t blue = (rgba >> 8) & 0xFF;
    uint8_t alpha = rgba & 0xFF;

    // Scale each component
    red = std::min(static_cast<uint32_t>(red * factor), 255u);
    green = std::min(static_cast<uint32_t>(green * factor), 255u);
    blue = std::min(static_cast<uint32_t>(blue * factor), 255u);

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
