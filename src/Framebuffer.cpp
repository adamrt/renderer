#include <algorithm>
#include <cmath>
#include <iostream>

#include "AK.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer(UI& ui, i32 width, i32 height)
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
    for (i32 y = 0; y < m_height; y += 10) {
        for (i32 x = 0; x < m_width; x += 10) {
            m_colorbuffer[(m_width * y) + x] = color;
        }
    }
}

void Framebuffer::draw_pixel(i32 x, i32 y, Color color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
    m_colorbuffer[(m_width * y) + x] = color;
}

// draw_line uses the Bresenham's line algorithm
void Framebuffer::draw_line(i32 x0, i32 y0, i32 x1, i32 y1, Color color)
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

    i32 dx = x1 - x0;
    i32 dy = std::abs(y1 - y0);
    i32 error = dx / 2;
    i32 ystep = (y0 < y1) ? 1 : -1;
    i32 y = y0;

    for (i32 x = x0; x <= x1; x++) {
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

void Framebuffer::draw_rect(i32 x, i32 y, i32 w, i32 h, Color color)
{
    for (i32 i = 0; i < w; i++) {
        for (i32 j = 0; j < h; j++) {
            draw_pixel(x + i, y + j, color);
        }
    }
}

void Framebuffer::draw_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, Color color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void Framebuffer::draw_triangle_textured(Triangle& t, Texture& tex)
{
    Vec2 a = t.points[0], b = t.points[1], c = t.points[2];
    Vec2 at = t.texcoords[0], bt = t.texcoords[1], ct = t.texcoords[2];

    i32 min_x = std::min({ a.x, b.x, c.x });
    i32 min_y = std::min({ a.y, b.y, c.y });
    i32 max_x = std::max({ a.x, b.x, c.x });
    i32 max_y = std::max({ a.y, b.y, c.y });

    // Clamp possible values to the framebuffer so we don't overdraw.
    min_x = std::max({ min_x, 0 });
    min_y = std::max({ min_y, 0 });
    max_x = std::min({ max_x, m_width - 1 });
    max_y = std::min({ max_y, m_height - 1 });

    Vec2 p;
    for (p.y = min_y; p.y <= max_y; p.y++) {
        for (p.x = min_x; p.x <= max_x; p.x++) {
            Vec3 weights = barycentric(a, b, c, p);

            if ((weights.x >= 0) && (weights.y >= 0) && (weights.z >= 0)) {
                Vec2 tex_coord = (at * weights.x) + (bt * weights.y) + (ct * weights.z);

                // Fetch color from the texture
                i32 tex_x = static_cast<int>(tex_coord.x * tex.width());
                i32 tex_y = static_cast<int>(tex_coord.y * tex.height());

                // Assuming the image is loaded as RGBA
                i32 offset = (tex_y * tex.width() + tex_x) * tex.channels();
                u8 raw[4] = { tex.data()[offset], tex.data()[offset + 1], tex.data()[offset + 2], tex.data()[offset + 3] };

                u32 rgba = (raw[0] << 24) | (raw[1] << 16) | (raw[2] << 8) | raw[3];

                draw_pixel(p.x, p.y, Color(rgba));
            }
        }
    }
}

// draw_triangle_filled draws a filled triangle using the edge function algorithm
void Framebuffer::draw_triangle_filled(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, Color color)
{
    i32 min_x = std::min({ x0, x1, x2 });
    i32 min_y = std::min({ y0, y1, y2 });
    i32 max_x = std::max({ x0, x1, x2 });
    i32 max_y = std::max({ y0, y1, y2 });

    // Clamp possible values to the framebuffer so we don't overdraw.
    min_x = std::max({ min_x, 0 });
    min_y = std::max({ min_y, 0 });
    max_x = std::min({ max_x, m_width - 1 });
    max_y = std::min({ max_y, m_height - 1 });

    for (i32 y = min_y; y <= max_y; y++) {
        for (i32 x = min_x; x <= max_x; x++) {
            i32 e0 = edge_function(x0, y0, x1, y1, x, y);
            i32 e1 = edge_function(x1, y1, x2, y2, x, y);
            i32 e2 = edge_function(x2, y2, x0, y0, x, y);

            if (e0 <= 0 && e1 <= 0 && e2 <= 0) {
                draw_pixel(x, y, color);
            }
        }
    }
}

inline i32 edge_function(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2)
{
    return (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
};

Vec3 barycentric(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p)
{
    Vec2 ab = b - a;
    Vec2 ac = c - a;
    Vec2 ap = p - a;

    f32 d00 = Vec2::dot(ab, ab);
    f32 d01 = Vec2::dot(ab, ac);
    f32 d11 = Vec2::dot(ac, ac);
    f32 d20 = Vec2::dot(ap, ab);
    f32 d21 = Vec2::dot(ap, ac);
    f32 denom = d00 * d11 - d01 * d01;

    Vec3 result;

    f32 v = (d11 * d20 - d01 * d21) / denom;
    f32 w = (d00 * d21 - d01 * d20) / denom;
    f32 u = 1.0f - v - w;

    return Vec3 { u, v, w };
}
