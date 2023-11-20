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
    , m_depthbuffer(m_width * m_height)
{
}

void Framebuffer::clear_colorbuffer(Color color)
{
    std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), color);
}

void Framebuffer::clear_depthbuffer()
{
    std::fill(m_depthbuffer.begin(), m_depthbuffer.end(), 1.0f);
}

f32 Framebuffer::get_depth(i32 x, i32 y)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return 1.0f;
    }
    return m_depthbuffer[(m_width * y) + x];
}

void Framebuffer::set_depth(i32 x, i32 y, f32 depth)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
    m_depthbuffer[(m_width * y) + x] = depth;
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
    Vec4 a = t.points[0], b = t.points[1], c = t.points[2];
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
    Vec2 v0 = a.xy();
    Vec2 v1 = b.xy();
    Vec2 v2 = c.xy();

    f32 area = edge_cross(v0, v1, v2);

    for (p.y = min_y; p.y <= max_y; p.y++) {
        for (p.x = min_x; p.x <= max_x; p.x++) {

            // These are the opposing vertices to the side we want.
            float w0 = edge_cross(v1, v2, p);
            float w1 = edge_cross(v2, v0, p);
            float w2 = edge_cross(v0, v1, p);

            f32 alpha = w0 / area;
            f32 beta = w1 / area;
            f32 gamma = w2 / area;

            if ((alpha >= 0) && (beta >= 0) && (gamma >= 0)) {

                auto interpolated_reciprocal_w = (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;
                auto depth = 1.0f - interpolated_reciprocal_w;
                if (depth < get_depth(p.x, p.y)) {

                    Vec2 tex_coord {};
                    if (m_ui.perspective_correction) {
                        auto interpolated_u = (at.x / a.w) * alpha + (bt.x / b.w) * beta + (ct.x / c.w) * gamma;
                        auto interpolated_v = (at.y / a.w) * alpha + (bt.y / b.w) * beta + (ct.y / c.w) * gamma;
                        tex_coord.x = interpolated_u /= interpolated_reciprocal_w;
                        tex_coord.y = interpolated_v /= interpolated_reciprocal_w;
                    } else {
                        tex_coord = (at * alpha) + (bt * beta) + (ct * gamma);
                    }

                    i32 tex_x = static_cast<int>(tex_coord.x * tex.width());
                    i32 tex_y = static_cast<int>(tex_coord.y * tex.height());

                    i32 offset = (tex_y * tex.width() + tex_x) * tex.channels();
                    u8 raw[4] = { tex.data()[offset], tex.data()[offset + 1], tex.data()[offset + 2], tex.data()[offset + 3] };

                    u32 rgba = (raw[0] << 24) | (raw[1] << 16) | (raw[2] << 8) | raw[3];

                    Color color(rgba);
                    color = color * t.light_intensity;

                    draw_pixel(p.x, p.y, color);
                    set_depth(p.x, p.y, depth);
                }
            }
        }
    }
}

// draw_triangle_filled draws a filled triangle using the edge function algorithm
void Framebuffer::draw_triangle_filled(Triangle& t)
{
    Color color(m_ui.fill_color);
    color = color * t.light_intensity;

    Vec4 a = t.points[0];
    Vec4 b = t.points[1];
    Vec4 c = t.points[2];

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
    Vec2 v0 = a.xy();
    Vec2 v1 = b.xy();
    Vec2 v2 = c.xy();

    for (p.y = min_y; p.y <= max_y; p.y++) {
        for (p.x = min_x; p.x <= max_x; p.x++) {
            f32 w0 = edge_cross(v0, v1, p);
            f32 w1 = edge_cross(v1, v2, p);
            f32 w2 = edge_cross(v2, v0, p);

            if (w0 <= 0 && w1 <= 0 && w2 <= 0) {
                draw_pixel(p.x, p.y, color);
            }
        }
    }
}

inline f32 edge_cross(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
};
