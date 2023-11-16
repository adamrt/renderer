#pragma once

#include <cstdint>
#include <vector>

class Framebuffer {
public:
    Framebuffer(int width, int height);

    const std::vector<uint32_t>& colorbuffer() { return m_colorbuffer; }
    int width(void) { return m_width; }
    int height(void) { return m_height; }

    void render(void);
    void clear(uint32_t color);
    void draw_pixel(int x, int y, uint32_t color);

private:
    int m_width;
    int m_height;
    std::vector<uint32_t> m_colorbuffer;
};
