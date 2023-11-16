#include <cstdint>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_internal.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int BUFFER_WIDTH = 800;
const int BUFFER_HEIGHT = 600;
const int ROW_WIDTH = BUFFER_WIDTH * sizeof(uint32_t);

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    auto window = SDL_CreateWindow("Heretic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        exit(EXIT_FAILURE);
    }

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        exit(EXIT_FAILURE);
    }

    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, BUFFER_WIDTH, BUFFER_HEIGHT);
    if (!texture) {
        fprintf(stderr, "Error creating SDL texture.\n");
        exit(EXIT_FAILURE);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigDockingWithShift = true;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    auto max = BUFFER_WIDTH * BUFFER_HEIGHT;

    std::vector<uint32_t> color_buffer(BUFFER_WIDTH * BUFFER_HEIGHT);

    auto is_running = true;

    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                    break;
                }
            }
        }

        int pixel = rand() % (max + 1);
        color_buffer[pixel] = 0xFFFF0000;
        SDL_UpdateTexture(texture, NULL, &color_buffer[0], ROW_WIDTH);

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton;
        static bool dockspaceOpen = true;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMove;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Heretic", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        ImGuiID dock_id = ImGui::GetID("MainDockSpace");
        const ImVec2 dockspace_size = ImGui::GetContentRegionAvail();
        ImGui::DockSpace(dock_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        {
            ImGui::Begin("Render", nullptr);
            ImGui::Image(texture, ImVec2(BUFFER_WIDTH, BUFFER_HEIGHT), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
            ImGui::End();
        }

        {
            ImGui::Begin("Control", nullptr);
            ImGui::Text("Right Contents");
            ImGui::End();
        }

        static bool first_frame = true;
        if (first_frame) {
            first_frame = false;

            ImGui::DockBuilderRemoveNode(dock_id);
            ImGui::DockBuilderAddNode(dock_id);
            ImGui::DockBuilderSetNodeSize(dock_id, dockspace_size);

            ImGuiID dock_id_left;
            ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Right, 0.35f, NULL, &dock_id_left);

            ImGui::DockBuilderDockWindow("Render", dock_id_left);
            ImGui::DockBuilderDockWindow("Control", dock_id_right);

            ImGui::DockBuilderFinish(dock_id);
        }
        ImGui::End();

        ImGui::Render();
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData()); // After RenderCopy
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
