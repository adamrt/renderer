#include "Window.h"

Window::Window(Framebuffer& fb, int width, int height)
    : m_fb(fb)
    , m_width(width)
    , m_height(height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    m_window = SDL_CreateWindow("Heretic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_fb.width(), m_fb.height());
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

void Window::update_gui()
{
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
        ImGui::Image(m_texture, ImVec2(m_fb.width(), m_fb.height()), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
        ImGui::End();
    }

    {
        ImGui::Begin("Control", nullptr);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io->Framerate, m_io->Framerate);
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
}

void Window::render()
{
    // Update our texture with the framebuffer data.
    SDL_UpdateTexture(m_texture, NULL, &m_fb.colorbuffer()[0], m_fb.width() * sizeof(uint32_t));

    // Draw the GUI.
    update_gui();

    // Render the GUI
    ImGui::Render();
    SDL_RenderClear(m_renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData()); // After RenderCopy
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
