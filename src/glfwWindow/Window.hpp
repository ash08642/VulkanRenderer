#pragma once

//#include <GLFW/glfw3.h>
#ifndef GLFW_INCLUDE_VULKAN
    #define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#ifndef GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>
namespace LiveWindow
{
    class Window
    {
    public:
        Window(const char* window_name, unsigned int window_widht, unsigned int window_height, bool* re_size);
        ~Window();
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        bool shouldClose();

        GLFWwindow* getWindow();
    private:
        const char* windowName;
        GLFWwindow* window;

        unsigned int windowWidht;
        unsigned int windowHeight;

        bool* reSize;

        void initWindow();

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };
}
