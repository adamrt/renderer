#include "Window.h"
#include "AK.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_video.h"

Window::Window(Framebuffer& framebuffer, i32 width, i32 height, bool fullscreen)
    : m_framebuffer(framebuffer)
    , m_width(width)
    , m_height(height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    u32 flags = SDL_WINDOW_SHOWN;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    m_window = SDL_CreateWindow("Heretic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, flags);

    // Create a SDL renderer.
    // - SDL_RENDERER_PRESENTVSYNC isn't included because it caps the
    //   framerate to the monitor's refresh rate. We want to control
    //   this via our time step.
    // - SDL_RENDERER_ACCELERATED isn't included because it is the
    // - default. Its commonly incorrectly included.
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    // Using RGBA8888 so we can use intuitive 0xRRGGBBAA uint32_t values for colors
    // FIXME: Might be worth changing to a native format like ARGB8888 for performance.
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_framebuffer.width(), m_framebuffer.height());
    if (!m_texture) {
        fprintf(stderr, "Error creating SDL texture.\n");
        exit(EXIT_FAILURE);
    }
}

void Window::update_texture()
{
    SDL_UpdateTexture(m_texture, NULL, m_framebuffer.colorbuffer().data(), m_framebuffer.width() * sizeof(u32));
    auto source = SDL_Rect { 0, 0, m_framebuffer.width(), m_framebuffer.height() };
    auto destination = SDL_Rect { 0, 0, m_framebuffer.scaled_width(), m_framebuffer.scaled_height() };
    SDL_RenderCopy(m_renderer, m_texture, &source, &destination);
}

void Window::render()
{
    SDL_RenderPresent(m_renderer);
}

Window::~Window()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
