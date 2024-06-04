#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createLogicalDevice(VkDevice& vlkn_logical_device, VkQueue& vlkn_graphics_queue, VkQueue& vlkn_present_queue, const VlknQueueFamilyIndices& vlkn_queue_family_indices, const VkPhysicalDevice& vlkn_physical_device, VkPhysicalDeviceFeatures& vlkn_physical_device_features);
}