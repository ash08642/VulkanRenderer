#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createSurface(VkSurfaceKHR& vlkn_surface, GLFWwindow* window, const VkInstance& vlkn_instance);
}