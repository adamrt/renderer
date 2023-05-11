#include "color.h"

#include "stdint.h"
// color_new returns a uint32_t for individual components.
//
// The returned format is 0xAABBGGRR.
// This matches ImGUI's ImVec4 and SDL2's SDL_PIXELFORMAT_ABGR8888.
auto color_new(int r, int g, int b, int a) -> uint32_t {
    return (a << 24) | (b << 16) | (g << 8) | r;
}
