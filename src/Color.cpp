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
    u8 r = (rgba >> 24) & 0xFF;
    u8 g = (rgba >> 16) & 0xFF;
    u8 b = (rgba >> 8) & 0xFF;
    u8 a = rgba & 0xFF;

    // Scale each component
    r = std::min(static_cast<u32>(r * factor), 255u);
    g = std::min(static_cast<u32>(g * factor), 255u);
    b = std::min(static_cast<u32>(b * factor), 255u);

    return (r << 24) | (g << 16) | (b << 8) | a;
}

Color Color::operator+(const Color& rhs) const
{
    // Extract RGBA components for the first color
    u8 lhs_r = (rgba >> 24) & 0xFF;
    u8 lhs_g = (rgba >> 16) & 0xFF;
    u8 lhs_b = (rgba >> 8) & 0xFF;
    // Just take the alpha of the first color for simplicity.
    u8 a = rgba & 0xFF;

    // Extract RGBA components for the second color
    u8 rhs_r = (rhs.rgba >> 24) & 0xFF;
    u8 rhs_g = (rhs.rgba >> 16) & 0xFF;
    u8 rhs_b = (rhs.rgba >> 8) & 0xFF;

    // Add each component, clamping to 255
    u8 r = std::min(static_cast<u32>(lhs_r + rhs_r), 255u);
    u8 g = std::min(static_cast<u32>(lhs_g + rhs_g), 255u);
    u8 b = std::min(static_cast<u32>(lhs_b + rhs_b), 255u);

    return (r << 24) | (g << 16) | (b << 8) | a;
}

Color Color::operator*(const Color& rhs) const
{
    u8 lhs_r = (rgba >> 24) & 0xFF;
    u8 lhs_g = (rgba >> 16) & 0xFF;
    u8 lhs_b = (rgba >> 8) & 0xFF;
    // Just take the alpha of the first color for simplicity.
    u8 a = rgba & 0xFF;

    // Extract RGBA components for the second color
    u8 rhs_r = (rhs.rgba >> 24) & 0xFF;
    u8 rhs_g = (rhs.rgba >> 16) & 0xFF;
    u8 rhs_b = (rhs.rgba >> 8) & 0xFF;

    // Multiply each color component and normalize to 255
    u8 r = (lhs_r * rhs_r) / 255;
    u8 g = (lhs_g * rhs_g) / 255;
    u8 b = (lhs_b * rhs_b) / 255;

    return (r << 24) | (g << 16) | (b << 8) | a;
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
