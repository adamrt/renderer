#include "UI.h"

UI::UI(int ui_width)
    : m_width(ui_width)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
}

UI::~UI()
{

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UI::init(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t, int fb_width, int fb_height)
{
    m_texture = t;
    m_fb_width = fb_width;
    m_fb_height = fb_height;

    m_io->IniFilename = nullptr;
    m_io->LogFilename = nullptr;
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(w, r);
    ImGui_ImplSDLRenderer2_Init(r);
}

void UI::render()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    update();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void UI::update()
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
            ImGui::Image(m_texture, ImVec2(m_fb_width, m_fb_height), ImVec2(0, 0), ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f));
            ImGui::End();
            ImGui::PopStyleVar();
        }

        {
            ImGui::Begin("Control", nullptr, window_flags);
            ImGui::Text("%.1f FPS (%.3f ms/frame)", m_io->Framerate, 1000.0f / m_io->Framerate);
            ImGui::Text("Visible Triangles: %d/%d", visible_triangles, total_triangles);

            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {

                ImGui::Text("Camera Phi %f", camera_phi);
                ImGui::SameLine((m_width / (f32)2));
                ImGui::Text("Theta %f", camera_theta);

                if (ImGui::RadioButton("Perspective", projection == Projection::Perspective)) {
                    projection = Projection::Perspective;
                    perspective_correction = true;
                    update_camera();
                }
                if (ImGui::RadioButton("Orthographic", projection == Projection::Orthographic)) {
                    projection = Projection::Orthographic;
                    perspective_correction = false;
                    update_camera();
                }
            }
            if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Surface
                if (ImGui::RadioButton("Empty", draw_empty)) {
                    draw_texture = false;
                    draw_filled = false;
                    draw_empty = true;
                }

                if (ImGui::RadioButton("Filled", draw_filled)) {
                    draw_texture = false;
                    draw_filled = true;
                    draw_empty = false;
                }
                ImGui::SameLine((m_width / (f32)3));
                ImGui::ColorEdit4("Color", &fill_color.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs);

                if (ImGui::RadioButton("Textured", draw_texture)) {
                    draw_texture = true;
                    draw_filled = false;
                    draw_empty = false;
                }
                ImGui::SameLine((m_width / (f32)3));
                ImGui::Checkbox("Perspective Correction", &perspective_correction);

                ImGui::Separator();
                ImGui::Checkbox("Lighting", &enable_lighting);
                ImGui::ColorEdit4("Ambient Light", &ambient_light.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs);
                ImGui::SameLine((m_width / (f32)3));
                ImGui::SliderFloat("Ambient Strength", &ambient_strength, 0.0f, 1.1f);
                ImGui::Separator();

                ImGui::Checkbox("Wireframe", &draw_wireframe);
                ImGui::Checkbox("Backface Culling", &backface_culling);
                ImGui::Checkbox("Fill Convention", &enable_fill_convention);

                ImGui::Separator();
                // Rotation Buttons
                ImGui::Checkbox("Rotate", &rotate);
                ImGui::SameLine();
                ImGui::Checkbox("X", &rotate_x);
                ImGui::SameLine();
                ImGui::Checkbox("Y", &rotate_y);
                ImGui::SameLine();
                ImGui::Checkbox("Z", &rotate_z);
                ImGui::SameLine();
                if (ImGui::Button("Reset Orientation")) {
                    orientation_event();
                }
            }

            if (ImGui::CollapsingHeader("Information")) {
                ImGui::Text("%dx%d framebuffer", m_fb_width, m_fb_height);
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
