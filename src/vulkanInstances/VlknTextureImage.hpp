#pragma once

#include "VlknData.hpp"
#include "VlknVertexBuffers.hpp"
#include "VlknImageViews.hpp"

namespace VulkanRenderer
{
    //load image and upload it to a VulkanImage object
    void createTextureImage(
        const char* image_path,
        VkImage& vlkn_texture_image,
        VkDeviceMemory& vlkn_texture_image_memory,
        const VkDevice& vlkn_device, 
        const VkPhysicalDevice& vlkn_physical_device,
        const VkFormat& vlkn_swap_chain_image_format,
        const VkCommandPool& vlkn_command_pool,
        const VkQueue& vlkn_graphics_queue
    );
    void createImage(
        VkImage& vlkn_texture_image,
        VkDeviceMemory& vlkn_texture_image_memory,
        const VkDevice& vlkn_device,
        const VkPhysicalDevice& vlkn_physical_device,
        uint32_t width, 
        uint32_t height,
        const VkFormat& format, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkMemoryPropertyFlags properties
    );
    void transitionImageLayout(
        const VkImage& image,
        const VkFormat& format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout,
        const VkDevice& vlkn_device,
        const VkCommandPool& vlkn_command_pool,
        const VkQueue& vlkn_graphics_queue
    );
    void copyBufferToImage(
        VkBuffer buffer,
        VkImage image,
        uint32_t width,
        uint32_t height,
        const VkDevice& vlkn_device,
        const VkCommandPool& vlkn_command_pool,
        const VkQueue& vlkn_graphics_queue
    );
    void createTextureImageView(VkImageView& vlkn_texture_image_view, VkImage& vlkn_texture_image, const VkFormat& vlkn_swap_chain_image_format, const VkDevice& vlkn_device);

    //Textures are usually accessed through samplers, which will apply filtering and transformations to compute the final color that is retrieved.
    void createTextureSampler(VkSampler& vlkn_texture_sampler, const VkDevice& vlkn_device, const VkPhysicalDeviceProperties& vlkn_physical_device_properties, const VkPhysicalDeviceFeatures& vlkn_physical_device_features);
}