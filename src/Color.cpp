#include "Color.h"

Color::Color()
    : rgba { 0x000000FF }
{
}

Color::Color(uint32_t color)
    : rgba { color }
{
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
