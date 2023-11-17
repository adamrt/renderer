cmake_minimum_required(VERSION 3.18)
project(
  Heretic
  VERSION     0.1
  DESCRIPTION "Final Fantasy Tactics Tooling"
  LANGUAGES   CXX)

# Using C++11 standard to allow building on PSVita.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

set(IMGUI_PATH  "lib/imgui/")
file(GLOB IMGUI_SOURCES "lib/imgui/*.h" "lib/imgui/*.cpp")
add_library(imgui ${IMGUI_SOURCES} lib/imgui/imgui.cpp)

add_executable(heretic
  src/main.cpp
  src/Engine.cpp
  src/Framebuffer.cpp
  src/Mesh.cpp
  src/Triangle.cpp
  src/Window.cpp)
target_include_directories(heretic PRIVATE ${SDL2_INCLUDE_DIRS} lib/imgui/)
target_link_libraries(heretic ${SDL2_LIBRARIES} imgui)
target_compile_options(heretic PRIVATE -Werror -Wall -Wextra -Wpedantic)
