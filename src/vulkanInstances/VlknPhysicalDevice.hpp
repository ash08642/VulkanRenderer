#pragma once

#include <map>

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void pickPhysicalDevice(VkPhysicalDevice& vlkn_device, VkPhysicalDeviceFeatures& vlkn_physical_device_features, VkPhysicalDeviceProperties& vlkn_physical_device_properties, VlknQueueFamilyIndices& vlkn_queue_family_indices, VkSampleCountFlagBits& vlkn_msaa_samples, const VkInstance& vlkn_instance, const VkSurfaceKHR& vlkn_surface);
    int rateDeviceSuitability(const VkPhysicalDevice& vlkn_device);
    bool checkDeviceExtensionSupport(const VkPhysicalDevice& vlkn_device);
    void findQueueFamilies(VkPhysicalDevice vlkn_device, VlknQueueFamilyIndices& vlkn_queue_family_indices, const VkSurfaceKHR& vlkn_surface);
    VkSampleCountFlagBits getMaxUsableSampleCount(const VkPhysicalDeviceProperties& vlkn_physical_device_properties);
}