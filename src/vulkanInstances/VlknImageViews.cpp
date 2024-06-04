#include "VlknImageViews.hpp"

void VulkanRenderer::createImageViews(std::vector<VkImageView> &vlkn_swap_chain_image_views, const std::vector<VkImage> &vlkn_swap_chain_images, const VkFormat& vlkn_swap_chain_image_format, const VkDevice &vlkn_device)
{
    vlkn_swap_chain_image_views.resize(vlkn_swap_chain_images.size());  //resize the list to fit all of the image views

    for (uint32_t i = 0; i < vlkn_swap_chain_images.size(); i++) 
    {
        vlkn_swap_chain_image_views[i] = createImageView(vlkn_swap_chain_images[i], vlkn_swap_chain_image_format, vlkn_device, VK_IMAGE_ASPECT_COLOR_BIT);
    }
    
}

VkImageView VulkanRenderer::createImageView(const VkImage &image, const VkFormat &format, const VkDevice &vlkn_device, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    //viewType and format fields specify how the image data should be interpreted
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;// '.viewType': treat images as 1D textures, 2D textures, 3D textures and cube maps
    viewInfo.format = format;

    // components field allows you to swizzle the color channels around
    // For example, you can map all of the channels to the red channel for a monochrome texture. (select 'VK_COMPONENT_SWIZZLE_IDENTITY' and press F12)
    //viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    //viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    //viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    //viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    // subresourceRange field describes what the image's purpose is and which part of the image should be accessed. 
    // Our images will be used as color targets without any mipmapping levels or multiple layers.
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(vlkn_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}
