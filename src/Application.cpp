#include "Application.hpp"

void LiveWindow::Application::run()
{
    while (!glfwWindow.shouldClose())
    {
        glfwPollEvents();
        vulkanRenderer.drawFrame();
    }
    vulkanRenderer.vlknDeviceWaitIdle();
}
