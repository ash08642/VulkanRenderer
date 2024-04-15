cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Vulkan_Renderer CXX)

#include("${CMAKE_CURRENT_LIST_DIR}/src/vulkan/vulkan.cmake")

add_executable(
    Vulkan_Renderer
    src/Main.cpp
)

add_subdirectory(src/vulkan)
add_subdirectory(src/glfw)

find_package(Vulkan REQUIRED)

target_link_libraries(Vulkan_Renderer PRIVATE Vulkan)
