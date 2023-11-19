#pragma once

#include <cstdint>
#include <vector>

#include "UI.h"

class Framebuffer {
public:
    Framebuffer(UI& ui, int width, int height);

    const std::vector<uint32_t>& colorbuffer() { return m_colorbuffer; }

    int width() const { return m_width; }
    int height() const { return m_height; }
    float aspect() const { return (float)m_height / (float)m_width; }

    void render();
    void clear(uint32_t color);
    void draw_pixel(int x, int y, uint32_t color);
    void draw_grid(uint32_t color);
    void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
    void draw_rect(int x, int y, int w, int h, uint32_t color);
    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    void draw_triangle_filled(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

private:
    UI& m_ui;
    int m_width;
    int m_height;
    std::vector<uint32_t> m_colorbuffer;
};
