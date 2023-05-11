#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "window.h"

Window::Window(int width, int height) : m_width(width), m_height(height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    m_window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width,
                                m_height, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        fprintf(stderr, "Error creating SDL window.\n");
        exit(EXIT_FAILURE);
    }

    m_renderer =
        SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        exit(EXIT_FAILURE);
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                  m_width, m_height);

    m_color_buffer.resize(get_size());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer_Init(m_renderer);
}

Window::~Window() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Window::render() {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        static float f = 0.0;
        static int counter = 0;

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::SliderFloat("float", &f, 0.0, 1.0);
        ImGui::ColorEdit3("clear color", (float *)&m_clear_color);

        if (ImGui::Button("Button")) {
            m_counter++;
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0 / m_io->Framerate, m_io->Framerate);
        ImGui::End();
    }

    int row_width = m_width * (int)sizeof(uint32_t);

    ImGui::Render();
    SDL_RenderSetScale(m_renderer, m_io->DisplayFramebufferScale.x,
                       m_io->DisplayFramebufferScale.y);

    SDL_UpdateTexture(m_texture, nullptr, m_color_buffer.data(), row_width);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_renderer);
}

void Window::clear(uint32_t color) {
    for (int i = 0; i < get_size(); i++) {
        m_color_buffer[i] = color;
    }
}

void Window::set_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
    m_color_buffer[(m_width * y) + x] = color;
}
