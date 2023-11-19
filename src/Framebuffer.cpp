#include <algorithm>
#include <cmath>

#include "Framebuffer.h"

Framebuffer::Framebuffer(UI& ui, int width, int height)
    : m_ui(ui)
    , m_width(width)
    , m_height(height)
    , m_colorbuffer(m_width * m_height)
{
}

void Framebuffer::clear(uint32_t color)
{
    std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), color);
}

void Framebuffer::draw_grid(uint32_t color)
{
    for (int y = 0; y < m_height; y += 10) {
        for (int x = 0; x < m_width; x += 10) {
            m_colorbuffer[(m_width * y) + x] = color;
        }
    }
}

void Framebuffer::draw_pixel(int x, int y, uint32_t color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
    m_colorbuffer[(m_width * y) + x] = color;
}

// draw_line uses the Bresenham's line algorithm
void Framebuffer::draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);
    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            draw_pixel(y, x, color);
        } else {
            draw_pixel(x, y, color);
        }
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void Framebuffer::draw_rect(int x, int y, int w, int h, uint32_t color)
{
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            draw_pixel(x + i, y + j, color);
        }
    }
}

void Framebuffer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

// draw_triangle_filled draws a filled triangle using the edge function algorithm
void Framebuffer::draw_triangle_filled(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    int min_x = std::min({ x0, x1, x2 });
    int min_y = std::min({ y0, y1, y2 });
    int max_x = std::max({ x0, x1, x2 });
    int max_y = std::max({ y0, y1, y2 });

    // Clamp possible values to the framebuffer so we don't overdraw.
    min_x = std::max({ min_x, 0 });
    min_y = std::max({ min_y, 0 });
    max_x = std::min({ max_x, m_width - 1 });
    max_y = std::min({ max_y, m_height - 1 });

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int e0 = edge_function(x0, y0, x1, y1, x, y);
            int e1 = edge_function(x1, y1, x2, y2, x, y);
            int e2 = edge_function(x2, y2, x0, y0, x, y);

            if (e0 <= 0 && e1 <= 0 && e2 <= 0) {
                draw_pixel(x, y, color);
            }
        }
    }
}

inline int edge_function(int x0, int y0, int x1, int y1, int x2, int y2)
{
    return (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
};
