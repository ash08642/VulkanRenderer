#include "VlknWindowSurface.hpp"

void VulkanRenderer::createSurface(VkSurfaceKHR &vlkn_surface, GLFWwindow *window, const VkInstance &vlkn_instance)
{
    /*
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(window);   //get the raw HWND from the GLFW window object
    createInfo.hinstance = GetModuleHandle(nullptr);

    if (vkCreateWin32SurfaceKHR(vlkn_instance, &createInfo, nullptr, &vlkn_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
    */
    if (glfwCreateWindowSurface(vlkn_instance, window, nullptr, &vlkn_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}