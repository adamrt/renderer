#include <algorithm>
#include <cmath>

#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_colorbuffer(m_width * m_height)
{
}

void Framebuffer::clear(uint32_t color)
{
    std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), color);
}

void Framebuffer::draw_pixel(int x, int y, uint32_t color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
    m_colorbuffer[(m_width * y) + x] = color;
}
