#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createImageViews(std::vector<VkImageView>& vlkn_swap_chain_image_views, const std::vector<VkImage>& vlkn_swap_chain_images, const VkFormat& vlkn_swap_chain_image_format, const VkDevice& vlkn_device);
    VkImageView createImageView(const VkImage& image, const VkFormat& format, const VkDevice& vlkn_device, VkImageAspectFlags aspectFlags);
}