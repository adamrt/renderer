#pragma once

#include <cstdint>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Texture.h"
#include "Triangle.h"
#include "Vector.h"

class Framebuffer {

public:
    Framebuffer(Camera& camera, i32 width, i32 height);

    i32 width() const { return m_width; }
    i32 height() const { return m_height; }
    f32 aspect() const { return (f32)m_height / (f32)m_width; }

    const std::vector<Color>& colorbuffer() const { return m_colorbuffer; }

    void clear_colorbuffer(Color color);
    void clear_depthbuffer();

    f32 get_depth(i32 x, i32 y);
    void set_depth(i32 x, i32 y, f32 depth);

    void render();
    void draw_pixel(i32 x, i32 y, Color color);
    void draw_grid(Color color);
    void draw_line(i32 x0, i32 y0, i32 x1, i32 y1, Color color);
    void draw_rect(i32 x, i32 y, i32 w, i32 h, Color color);
    void draw_triangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, Color color);
    void draw_triangle_filled(const Triangle& t, Color color);
    void draw_triangle_textured(const Triangle& t, const Texture& tex);

    bool enable_perspective_correction = true;
    bool enable_fill_convention = true;
    bool enable_lighting = true;
    f32 ambient_strength = 0.8f;

private:
    Camera& m_camera;
    i32 m_width;
    i32 m_height;
    std::vector<Color> m_colorbuffer;
    std::vector<f32> m_depthbuffer;
};
