#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"

#include "AK.h"
#include "Window.h"

Window::Window(Framebuffer& fb, UI& ui, i32 width, i32 height)
    : m_fb(fb)
    , m_ui(ui)
    , m_width(width)
    , m_height(height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    m_window = SDL_CreateWindow("Heretic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN);

    // Create a SDL renderer.
    // - SDL_RENDERER_PRESENTVSYNC isn't included because it caps the
    //   framerate to the monitor's refresh rate. We want to control
    //   this via our time step.
    // - SDL_RENDERER_ACCELERATED isn't included because it is the
    // - default. Its commonly incorrectly included.
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    // Using RGBA8888 so we can use intuitive 0xRRGGBBAA uint32_t values for colors
    // FIXME: Might be worth changing to a native format like ARGB8888 for performance.
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_fb.width(), m_fb.height());
    if (!m_texture) {
        fprintf(stderr, "Error creating SDL texture.\n");
        exit(EXIT_FAILURE);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->IniFilename = nullptr;
    m_io->LogFilename = nullptr;
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigDockingWithShift = true;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer2_Init(m_renderer);
}

void Window::draw_gui()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    {
        ImGui::Begin("Heretic", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMove);
        ImGui::PopStyleVar();

        ImGuiID dock_id = ImGui::GetID("MainDockSpace");

        static bool first_frame = true;
        if (first_frame) {
            first_frame = false;

            ImGui::DockBuilderRemoveNode(dock_id);
            ImGui::DockBuilderAddNode(dock_id);
            ImGui::DockBuilderSetNodeSize(dock_id, ImGui::GetContentRegionAvail());

            ImGuiID dock_id_left;
            ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Right, 0.33f, NULL, &dock_id_left);

            ImGui::DockBuilderDockWindow("Render", dock_id_left);
            ImGui::DockBuilderDockWindow("Control", dock_id_right);
            ImGui::DockBuilderFinish(dock_id);
        }

        ImGui::DockSpace(dock_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton);

        auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Render", nullptr, window_flags);
            ImGui::Image(m_texture, ImVec2(m_fb.width(), m_fb.height()), ImVec2(0, 0), ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f));
            ImGui::End();
            ImGui::PopStyleVar();
        }

        {
            ImGui::Begin("Control", nullptr, window_flags);
            ImGui::Text("%.1f FPS (%.3f ms/frame)", m_io->Framerate, 1000.0f / m_io->Framerate);
            ImGui::Text("Visible Triangles: %d/%d", m_ui.visible_triangles, m_ui.total_triangles);

            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat3("Camera Position", &m_ui.camera_position.x, -15.0f, 15.0f);
                if (ImGui::RadioButton("Perspective", m_ui.projection == Projection::Perspective)) {
                    m_ui.projection = Projection::Perspective;
                    m_ui.perspective_correction = true;
                    proj_event();
                }
                if (ImGui::RadioButton("Orthographic", m_ui.projection == Projection::Orthographic)) {
                    m_ui.projection = Projection::Orthographic;
                    m_ui.perspective_correction = false;
                    proj_event();
                }
            }
            if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Surface
                if (ImGui::RadioButton("Empty", m_ui.draw_empty)) {
                    m_ui.draw_texture = false;
                    m_ui.draw_filled = false;
                    m_ui.draw_empty = true;
                }

                if (ImGui::RadioButton("Filled", m_ui.draw_filled)) {
                    m_ui.draw_texture = false;
                    m_ui.draw_filled = true;
                    m_ui.draw_empty = false;
                }
                ImGui::SameLine((m_ui.width / (f32)3));
                ImGui::ColorEdit4("Color", &m_ui.fill_color.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs);

                if (ImGui::RadioButton("Textured", m_ui.draw_texture)) {
                    m_ui.draw_texture = true;
                    m_ui.draw_filled = false;
                    m_ui.draw_empty = false;
                }
                ImGui::SameLine((m_ui.width / (f32)3));
                ImGui::Checkbox("Perspective Correction", &m_ui.perspective_correction);

                ImGui::Separator();
                ImGui::Checkbox("Wireframe", &m_ui.draw_wireframe);
                ImGui::Checkbox("Lighting", &m_ui.enable_lighting);
                ImGui::Checkbox("Backface Culling", &m_ui.backface_culling);
                ImGui::Checkbox("Fill Convention", &m_ui.enable_fill_convention);

                ImGui::Separator();
                // Rotation Buttons
                ImGui::Checkbox("Rotate", &m_ui.rotate);
                ImGui::SameLine();
                ImGui::Checkbox("X", &m_ui.rotate_x);
                ImGui::SameLine();
                ImGui::Checkbox("Y", &m_ui.rotate_y);
                ImGui::SameLine();
                ImGui::Checkbox("Z", &m_ui.rotate_z);
                ImGui::SameLine();
                if (ImGui::Button("Reset Orientation")) {
                    orientation_event();
                }
            }

            if (ImGui::CollapsingHeader("Information")) {
                ImGui::Text("%dx%d framebuffer", m_fb.width(), m_fb.height());
                ImGui::Text("Left handed system");
                ImGui::Text("Row major matrices");
                ImGui::Text("Clockwise vertex winding");
            }

            if (ImGui::Button("Quit")) {
                SDL_Event event;
                event.type = SDL_QUIT;
                SDL_PushEvent(&event);
            }
            ImGui::End();
        }

        ImGui::End();
    }
}

void Window::render()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    SDL_UpdateTexture(m_texture, NULL, m_fb.colorbuffer().data(), m_fb.width() * sizeof(u32));

    draw_gui(); // Uses updated m_texture

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_renderer);
}

Window::~Window()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
