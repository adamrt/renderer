#include <algorithm>
#include <cmath>
#include <iostream>

#include "Framebuffer.h"

Framebuffer::Framebuffer(UI& ui, int width, int height)
    : m_ui(ui)
    , m_width(width)
    , m_height(height)
    , m_colorbuffer(m_width * m_height)
{
}

void Framebuffer::clear(Color color)
{
    std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), color);
}

void Framebuffer::draw_grid(Color color)
{
    for (int y = 0; y < m_height; y += 10) {
        for (int x = 0; x < m_width; x += 10) {
            m_colorbuffer[(m_width * y) + x] = color;
        }
    }
}

void Framebuffer::draw_pixel(int x, int y, Color color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
    m_colorbuffer[(m_width * y) + x] = color;
}

// draw_line uses the Bresenham's line algorithm
void Framebuffer::draw_line(int x0, int y0, int x1, int y1, Color color)
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

void Framebuffer::draw_rect(int x, int y, int w, int h, Color color)
{
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            draw_pixel(x + i, y + j, color);
        }
    }
}

void Framebuffer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void Framebuffer::draw_triangle_textured(Triangle& t, Texture& tex)
{
    auto a = t.points[0], b = t.points[1], c = t.points[2];
    auto at = t.texcoords[0], bt = t.texcoords[1], ct = t.texcoords[2];

    int min_x = std::min({ a.x, b.x, c.x });
    int min_y = std::min({ a.y, b.y, c.y });
    int max_x = std::max({ a.x, b.x, c.x });
    int max_y = std::max({ a.y, b.y, c.y });

    // Clamp possible values to the framebuffer so we don't overdraw.
    min_x = std::max({ min_x, 0 });
    min_y = std::max({ min_y, 0 });
    max_x = std::min({ max_x, m_width - 1 });
    max_y = std::min({ max_y, m_height - 1 });

    Vec2 p;
    for (p.y = min_y; p.y <= max_y; p.y++) {
        for (p.x = min_x; p.x <= max_x; p.x++) {
            auto weights = barycentric(a, b, c, p);

            if ((weights.x >= 0) && (weights.y >= 0) && (weights.z >= 0)) {
                Vec2 texCoord = (at * weights.x) + (bt * weights.y) + (ct * weights.z);

                // Fetch color from the texture
                int texX = static_cast<int>(texCoord.x * tex.width());
                int texY = static_cast<int>(texCoord.y * tex.height());

                // Assuming the image is loaded as RGBA
                int offset = (texY * tex.width() + texX) * tex.channels();
                unsigned char color[4] = { tex.data()[offset], tex.data()[offset + 1], tex.data()[offset + 2], tex.data()[offset + 3] };

                uint32_t colorInt = (color[3] << 24) | (color[0] << 16) | (color[1] << 8) | color[2];

                draw_pixel(p.x, p.y, Color(colorInt));
            }
        }
    }
}

// draw_triangle_filled draws a filled triangle using the edge function algorithm
void Framebuffer::draw_triangle_filled(int x0, int y0, int x1, int y1, int x2, int y2, Color color)
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

Vec3 barycentric(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p)
{
    Vec2 ab = b - a;
    Vec2 ac = c - a;
    Vec2 ap = p - a;

    float d00 = Vec2::dot(ab, ab);
    float d01 = Vec2::dot(ab, ac);
    float d11 = Vec2::dot(ac, ac);
    float d20 = Vec2::dot(ap, ab);
    float d21 = Vec2::dot(ap, ac);
    float denom = d00 * d11 - d01 * d01;

    Vec3 result;

    auto v = (d11 * d20 - d01 * d21) / denom;
    auto w = (d00 * d21 - d01 * d20) / denom;
    auto u = 1.0f - v - w;

    return Vec3 { u, v, w };
}
