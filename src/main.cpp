#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "AK.h"
#include "Camera.h"
#include "Engine.h"
#include "Framebuffer.h"
#include "UI.h"
#include "Window.h"

constexpr bool FULLSCREEN = false;
constexpr i32 WINDOW_WIDTH = FULLSCREEN ? 2560 : (1024 / 3) * 4;
constexpr i32 WINDOW_HEIGHT = FULLSCREEN ? 1440 : 768;

// Currently always using 1:1 ratio since performance is fine for small models.
constexpr i32 FB_SCALE = FULLSCREEN ? 1 : 1;
constexpr i32 FB_WIDTH = ((WINDOW_WIDTH / 4) * 3) / FB_SCALE;
constexpr i32 FB_HEIGHT = WINDOW_HEIGHT / FB_SCALE;

constexpr f32 FOV_Y = K_PI / 3.0f;
constexpr f32 ZNEAR = 0.01f;
constexpr f32 ZFAR = 100.0f;

i32 main()
{
    Mesh mesh { "res/drone.obj" };
    Texture texture { "res/drone.png" };
    Model model { mesh, texture };

    Light light_b { Vec3(2.0f, 0.0f, 0.0f), Color::Blue };
    Light light_r { Vec3(-2.0f, 0.0f, 0.0f), Color::Red };

    Scene scene;
    scene.models.push_back(model);
    scene.lights.push_back(light_b);
    scene.lights.push_back(light_r);

    Camera camera { FB_WIDTH, FB_HEIGHT, FOV_Y, ZNEAR, ZFAR };
    Framebuffer framebuffer { camera, FB_WIDTH, FB_HEIGHT, FB_SCALE };
    Window window { framebuffer, WINDOW_WIDTH, WINDOW_HEIGHT, FULLSCREEN };
    UI ui { window, framebuffer, camera, scene };
    Engine engine { window, framebuffer, camera, ui, scene };

    while (engine.is_running()) {
        engine.process_input();
        engine.update();
        engine.render();
    }

    return 0;
}
