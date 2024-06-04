#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createVlknInstance(VkInstance& vlkn_instance);
    std::vector<const char*> getRequiredExtensions();
}