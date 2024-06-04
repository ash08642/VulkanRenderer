#include "Window.hpp"

LiveWindow::Window::Window(const char *window_name, unsigned int window_widht, unsigned int window_height, bool* re_size):
    windowName{window_name},
    windowWidht{window_widht},
    windowHeight{window_height},
    reSize{re_size}
{
    initWindow();
}

LiveWindow::Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool LiveWindow::Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

GLFWwindow *LiveWindow::Window::getWindow()
{
    return window;
}

void LiveWindow::Window::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   //tell glfw to not create opengl context 
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     //resize is disabled

    window = glfwCreateWindow(windowWidht, windowHeight, windowName, nullptr, nullptr);
    glfwSetWindowUserPointer(window, reSize);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void LiveWindow::Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    bool* reSized = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
    *reSized = true;
}
