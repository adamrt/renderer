cmake_minimum_required(VERSION 3.20)
project(Renderer CXX)

set(LLVM_VERSION 11.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-pedantic)

add_executable(renderer
  src/main.cpp
  src/color.cpp
  src/renderer.cpp
  src/window.cpp
  lib/imgui/imgui.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_tables.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/backends/imgui_impl_sdl2.cpp
  lib/imgui/backends/imgui_impl_sdlrenderer.cpp)


include_directories(
  lib/imgui
  lib/imgui/backends)

target_link_libraries(renderer m SDL2)
