#pragma once

#include "VlknData.hpp"
#include "VlknTextureImage.hpp"

namespace VulkanRenderer
{
    void createDepthResources(
        VkImage& vlkn_depth_image,
        VkDeviceMemory& vlkn_depth_image_memory,
        VkImageView& vlkn_depth_image_view,
        const VkFormat& vlkn_depth_format,
        const VkExtent2D vlkn_swap_chain_extent,
        const VkDevice& vlkn_device,
        const VkPhysicalDevice& vlkn_physical_device
    );
    VkFormat findDepthFormat(const VkPhysicalDevice& vlkn_physical_device);
    bool hasStencilComponent(const VkFormat& format);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, const VkPhysicalDevice& vlkn_physical_device);
}