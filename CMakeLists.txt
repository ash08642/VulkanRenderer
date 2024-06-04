cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Vulkan_Renderer CXX)

add_executable(
    Vulkan_Renderer
    src/Main.cpp
)

find_package(Vulkan REQUIRED)
include_directories(C:/VulkanSDK/1.3.280.0/Include)
target_link_directories(Vulkan_Renderer
    PRIVATE buildtree/external/glfw/src/Debug)

add_subdirectory(external/glfw)
target_include_directories(Vulkan_Renderer PUBLIC external/glfw/include)
include_directories(external/glfw/include)

target_include_directories(Vulkan_Renderer PUBLIC external/stbImage)
include_directories(external/stbImage)

target_include_directories(Vulkan_Renderer PUBLIC external/tinyObjLoader)
include_directories(external/tinyObjLoader)

add_subdirectory(src/glfwWindow)
add_subdirectory(src/vulkanInstances)
add_subdirectory(src/vertexEngine)
add_subdirectory(src)

target_link_libraries(Vulkan_Renderer
    glfw3 Vulkan::Vulkan Window Application)


