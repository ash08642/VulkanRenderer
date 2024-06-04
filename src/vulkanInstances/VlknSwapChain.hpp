#pragma once

#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

#include "VlknData.hpp"
#include "VlknImageViews.hpp"
#include "VlknFrameBuffer.hpp"
#include "VlknDepthBuffer.hpp"

namespace VulkanRenderer
{
    void createSwapChain
    (
        VkSwapchainKHR& vlkn_swap_chain,
        std::vector<VkImage>& vlkn_swap_chain_images,
        VkFormat& vlkn_swap_chain_image_format,
        VkExtent2D& vlkn_swap_chain_extent,
        VlknSwapChainSupportDetails& vlkn_swap_chain_support_details,
        const VkPhysicalDevice& vlkn_physical_device,
        const VkDevice& vlkn_device,
        const VkSurfaceKHR& vlkn_surface,
        GLFWwindow* window,
        const VlknQueueFamilyIndices& vlkn_queue_family_indices
    );
    void recreateSwapChain
    (
        VkSwapchainKHR& vlkn_swap_chain,
        std::vector<VkImage>& vlkn_swap_chain_images,
        VkFormat& vlkn_swap_chain_image_format,
        VkExtent2D& vlkn_swap_chain_extent,
        VlknSwapChainSupportDetails& vlkn_swap_chain_support_details,
        std::vector<VkFramebuffer>& vlkn_swap_chain_framebuffers,
        std::vector<VkImageView>& vlkn_swap_chain_image_views,
        VkImage& vlkn_depth_image,
        VkDeviceMemory& vlkn_depth_image_memory,
        VkImageView& vlkn_depth_image_view,
        const VkFormat& vlkn_depth_format,
        const VkRenderPass& vlkn_render_pass,
        const VkPhysicalDevice& vlkn_physical_device,
        const VkDevice& vlkn_device,
        const VkSurfaceKHR& vlkn_surface,
        GLFWwindow* window,
        const VlknQueueFamilyIndices& vlkn_queue_family_indices
    );
    void cleanupSwapChain
    (
        VkSwapchainKHR& vlkn_swap_chain,
        std::vector<VkFramebuffer>& vlkn_swap_chain_framebuffers,
        std::vector<VkImageView>& vlkn_swap_chain_image_views,
        VkImage& vlkn_depth_image,
        VkDeviceMemory& vlkn_depth_image_memory,
        VkImageView& vlkn_depth_image_view,
        const VkDevice& vlkn_device
    );
    void querySwapChainSupport(VlknSwapChainSupportDetails& vlkn_swap_chain_support_details, const VkPhysicalDevice& vlkn_physical_device, const VkSurfaceKHR& vlkn_surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
}

/*
Swap Chain Creation Steps
    Query Swap Chain Settings
        Basic surface capabilities (min/max number of images in swap chain, min/max width and height of images)
        Surface formats (pixel format, color space)
        Available presentation modes
    Choose Swap Chain Settings
        Surface format (color depth)
        Presentation mode (conditions for "swapping" images to the screen)
        Swap extent (resolution of images in swap chain)
*/