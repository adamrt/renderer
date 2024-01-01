#include "imgui.h"

#include "UI.h"

UI::UI(Window& window, Framebuffer& framebuffer, Camera& camera, Scene& scene)
    : m_window(window)
    , m_framebuffer(framebuffer)
    , m_camera(camera)
    , m_scene(scene)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();

    m_io->IniFilename = nullptr;
    m_io->LogFilename = nullptr;
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window.sdl_window(), window.sdl_renderer());
    ImGui_ImplSDLRenderer2_Init(window.sdl_renderer());
}

i32 UI::width() const
{
    return m_window.width() - m_framebuffer.scaled_width();
}

UI::~UI()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
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
        ImGui::Begin("Renderer", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMove);
        ImGui::PopStyleVar();

        ImGuiID dock_id = ImGui::GetID("MainDockSpace");

        static bool first_frame = true;
        if (first_frame) {
            first_frame = false;

            ImGui::DockBuilderRemoveNode(dock_id);
            ImGui::DockBuilderAddNode(dock_id);
            ImGui::DockBuilderSetNodeSize(dock_id, ImGui::GetContentRegionAvail());

            f32 ui_width = (m_window.width() - (f32)m_framebuffer.scaled_width()) / m_window.width();

            ImGuiID dock_id_left;
            ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Right, ui_width, NULL, &dock_id_left);

            ImGui::DockBuilderDockWindow("Render", dock_id_left);
            ImGui::DockBuilderDockWindow("Control", dock_id_right);
            ImGui::DockBuilderFinish(dock_id);
        }

        ImGui::DockSpace(dock_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton);

        auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Render", nullptr, window_flags);
            ImGui::Image(m_window.sdl_texture(), ImVec2(m_framebuffer.scaled_width(), m_framebuffer.scaled_height()), ImVec2(0, 0), ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f));
            ImGui::End();
            ImGui::PopStyleVar();
        }

        {
            ImGui::Begin("Control", nullptr, window_flags);
            ImGui::Text("%.1f FPS (%.3f ms/frame)", m_io->Framerate, 1000.0f / m_io->Framerate);
            ImGui::Text("Visible Triangles: %d/%d", visible_triangles, total_triangles);

            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {

                if (ImGui::RadioButton("Perspective", m_camera.projection == Projection::Perspective)) {
                    m_camera.projection = Projection::Perspective;
                    m_camera.update();
                    m_framebuffer.enable_perspective_correction = true;
                }
                if (ImGui::RadioButton("Orthographic", m_camera.projection == Projection::Orthographic)) {
                    m_camera.projection = Projection::Orthographic;
                    m_framebuffer.enable_perspective_correction = false;
                    m_camera.update();
                }
            }
            if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Surface
                if (ImGui::RadioButton("Empty", draw_empty)) {
                    draw_texture = false;
                    draw_filled = false;
                    draw_empty = true;
                    draw_normals = false;
                }

                if (ImGui::RadioButton("Filled", draw_filled)) {
                    draw_texture = false;
                    draw_filled = true;
                    draw_empty = false;
                    draw_normals = false;
                }
                if (draw_filled) {
                    ImGui::SameLine((width() / (f32)3));
                    ImGui::ColorEdit4("Color", &fill_color.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs);
                }

                if (ImGui::RadioButton("Normals", draw_normals)) {
                    draw_texture = false;
                    draw_empty = false;
                    draw_filled = false;
                    draw_normals = true;
                }
                if (draw_normals) {
                    ImGui::SameLine((width() / (f32)3));
                    ImGui::Checkbox("Smooth Shading", &m_framebuffer.enable_smooth_shading);
                }

                if (ImGui::RadioButton("Textured", draw_texture)) {
                    draw_texture = true;
                    draw_filled = false;
                    draw_empty = false;
                    draw_normals = false;
                    m_framebuffer.enable_perspective_correction = true;
                }

                if (draw_texture) {
                    ImGui::SameLine((width() / (f32)3));
                    ImGui::Checkbox("Perspective Correction", &m_framebuffer.enable_perspective_correction);
                }

                ImGui::Separator();

                ImGui::Checkbox("Wireframe", &draw_wireframe);
                ImGui::Checkbox("Backface Culling", &backface_culling);

                if (ImGui::Checkbox("Auto Scaled and Centered", &auto_scaled_and_centered)) {
                    if (auto_scaled_and_centered) {
                        m_scene.models[0].scale = m_scene.models[0].mesh.normalized_scale();
                        m_scene.models[0].translation = m_scene.models[0].mesh.center_translation();
                    } else {
                        m_scene.models[0].translation = Vec3(0.0f, 0.0f, 0.0f);
                        m_scene.models[0].scale = Vec3(1.0f, 1.0f, 1.0f);
                    }
                }

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
                if (ImGui::Button("Reset")) {
                    orientation_event();
                }
            }

            if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Checkbox("Enable Lighting", &m_framebuffer.enable_lighting);
                ImGui::Separator();
                ImGui::Text("Point Lights");
                for (size_t i = 0; i < m_scene.lights.size(); i++) {
                    ImGui::PushID(i);
                    auto color = m_scene.lights[i].color.imvec4();
                    if (ImGui::ColorEdit4("", &color.x, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs)) {
                        m_scene.lights[i].color = Color(color);
                    }
                    ImGui::SameLine();
                    ImGui::SliderFloat3("Position", &m_scene.lights[i].position.x, -10.0f, 10.0f);
                    ImGui::PopID();
                }
                ImGui::Separator();
                ImGui::SliderFloat("Ambient Strength", &m_framebuffer.ambient_strength, 0.0f, 1.1f);
            }

            if (ImGui::CollapsingHeader("Information")) {
                ImGui::Text("%dx%d framebuffer", m_framebuffer.width(), m_framebuffer.height());
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
