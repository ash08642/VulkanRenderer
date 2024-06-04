#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <cstdlib>
#include <stdexcept>
//include <GLFW/glfw3.h>
//#include <vulkan/vulkan.hpp>
//#include "vulkanInstances/instance/Instance.hpp"
//#include "glfwWindow/window/Window.hpp"
#include "Application.hpp"

int main()
{
    LiveWindow::Application app{};

    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}