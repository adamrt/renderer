#include <algorithm>
#include <cmath>
#include <iostream>

#include "AK.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer(Camera& camera, i32 width, i32 height)
    : m_camera(camera)
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

void Framebuffer::draw_triangle_textured(const Triangle& t, const Texture& tex)
{
    Vec4 a = t.vertices[0].position, b = t.vertices[1].position, c = t.vertices[2].position;
    Vec2 at = t.vertices[0].uv, bt = t.vertices[1].uv, ct = t.vertices[2].uv;

    i32 min_x = std::min({ a.x, b.x, c.x });
    i32 min_y = std::min({ a.y, b.y, c.y });
    i32 max_x = std::max({ a.x, b.x, c.x });
    i32 max_y = std::max({ a.y, b.y, c.y });

    // Clamp possible values to the framebuffer so we don't overdraw.
    min_x = std::max({ min_x, 0 });
    min_y = std::max({ min_y, 0 });
    max_x = std::min({ max_x, m_width - 1 });
    max_y = std::min({ max_y, m_height - 1 });

    Vec2 v0 = a.xy();
    Vec2 v1 = b.xy();
    Vec2 v2 = c.xy();

    // Fill convention
    f32 bias0 = Vec2::is_top_left(v1, v2) ? 0 : -0.0001;
    f32 bias1 = Vec2::is_top_left(v2, v0) ? 0 : -0.0001;
    f32 bias2 = Vec2::is_top_left(v0, v1) ? 0 : -0.0001;

    f32 w0_delta_col = v1.y - v2.y;
    f32 w1_delta_col = v2.y - v0.y;
    f32 w2_delta_col = v0.y - v1.y;

    f32 w0_delta_row = v2.x - v1.x;
    f32 w1_delta_row = v0.x - v2.x;
    f32 w2_delta_row = v1.x - v0.x;

    f32 area = Vec2::edge_cross(v0, v1, v2);

    Vec2 p0(min_x, min_y);
    f32 w0_row = Vec2::edge_cross(v1, v2, p0);
    f32 w1_row = Vec2::edge_cross(v2, v0, p0);
    f32 w2_row = Vec2::edge_cross(v0, v1, p0);

    if (enable_fill_convention) {
        w0_row += bias0;
        w1_row += bias1;
        w2_row += bias2;
    }

    for (i32 y = min_y; y <= max_y; y++) {
        f32 w0 = w0_row;
        f32 w1 = w1_row;
        f32 w2 = w2_row;
        for (i32 x = min_x; x <= max_x; x++) {
            bool is_inside = (w0 >= 0) && (w1 >= 0) && (w2 >= 0);
            if (is_inside) {
                f32 alpha = w0 / area;
                f32 beta = w1 / area;
                f32 gamma = w2 / area;

                f32 depth = 0.0f;
                f32 interpolated_reciprocal_w = 0.0f;
                if (m_camera.projection == Projection::Perspective) {
                    if (enable_perspective_correction) {
                        interpolated_reciprocal_w = (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;
                        depth = 1.0f - interpolated_reciprocal_w;
                    } else {
                        depth = (1 / a.z) * alpha + (1 / b.z) * beta + (1 / c.z) * gamma;
                        depth = 1.0f - depth;
                    }
                } else if (m_camera.projection == Projection::Orthographic) {
                    // NOTE: I'm not sure why we need the reciprocal of these, but we do.
                    depth = (1 / a.z) * alpha + (1 / b.z) * beta + (1 / c.z) * gamma;
                }

                if (depth < get_depth(x, y)) {

                    Vec2 tex_coord {};
                    if (enable_perspective_correction) {
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
                    if (enable_lighting) {
                        color = (color * ambient_strength) + (t.light_sum * color);
                    }

                    draw_pixel(x, y, color);
                    set_depth(x, y, depth);
                }
            }
            w0 += w0_delta_col;
            w1 += w1_delta_col;
            w2 += w2_delta_col;
        }
        w0_row += w0_delta_row;
        w1_row += w1_delta_row;
        w2_row += w2_delta_row;
    }
}

// draw_triangle_filled draws a filled triangle using the edge function algorithm
void Framebuffer::draw_triangle_filled(const Triangle& t, Color color)
{
    if (enable_lighting) {
        color = (color * ambient_strength) + (t.light_sum * color);
    }

    Vec4 a = t.vertices[0].position;
    Vec4 b = t.vertices[1].position;
    Vec4 c = t.vertices[2].position;

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

    // Fill convention
    f32 bias0 = Vec2::is_top_left(v1, v2) ? 0 : -0.0001;
    f32 bias1 = Vec2::is_top_left(v2, v0) ? 0 : -0.0001;
    f32 bias2 = Vec2::is_top_left(v0, v1) ? 0 : -0.0001;

    f32 area = Vec2::edge_cross(v0, v1, v2);

    for (p.y = min_y; p.y <= max_y; p.y++) {
        for (p.x = min_x; p.x <= max_x; p.x++) {
            f32 w0 = Vec2::edge_cross(v1, v2, p);
            f32 w1 = Vec2::edge_cross(v2, v0, p);
            f32 w2 = Vec2::edge_cross(v0, v1, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {

                if (enable_fill_convention) {
                    w0 += bias0;
                    w1 += bias1;
                    w2 += bias2;
                }

                f32 alpha = w0 / area;
                f32 beta = w1 / area;
                f32 gamma = w2 / area;

                f32 depth = 0.0f;
                if (m_camera.projection == Projection::Perspective) {
                    if (enable_perspective_correction) {
                        auto interpolated_reciprocal_w = (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;
                        depth = 1.0f - interpolated_reciprocal_w;
                    } else {
                        depth = (1 / a.z) * alpha + (1 / b.z) * beta + (1 / c.z) * gamma;
                        depth = 1.0f - depth;
                    }
                } else if (m_camera.projection == Projection::Orthographic) {
                    // NOTE: I'm not sure why we need the reciprocal of these, but we do.
                    depth = (1 / a.z) * alpha + (1 / b.z) * beta + (1 / c.z) * gamma;
                }

                if (depth < get_depth(p.x, p.y)) {
                    draw_pixel(p.x, p.y, color);
                    set_depth(p.x, p.y, depth);
                }
            }
        }
    }
}
