#include "VlknDepthBuffer.hpp"

void VulkanRenderer::createDepthResources(
    VkImage& vlkn_depth_image, 
    VkDeviceMemory& vlkn_depth_image_memory, 
    VkImageView& vlkn_depth_image_view,
    const VkFormat& vlkn_depth_format,
    const VkExtent2D vlkn_swap_chain_extent,
    const VkDevice& vlkn_device, 
    const VkPhysicalDevice &vlkn_physical_device
    )
{
    //vlkn_depth_format = findDepthFormat(vlkn_physical_device); // moved to renderpass
    if(vlkn_depth_format == VK_FORMAT_UNDEFINED)
    {
        std::cout << "depth format is undefined" << std::endl;
    }

    createImage(vlkn_depth_image, vlkn_depth_image_memory, vlkn_device, vlkn_physical_device, vlkn_swap_chain_extent.width, vlkn_swap_chain_extent.height, vlkn_depth_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vlkn_depth_image_view = createImageView(vlkn_depth_image, vlkn_depth_format, vlkn_device, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkFormat VulkanRenderer::findDepthFormat(const VkPhysicalDevice &vlkn_physical_device)
{
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
        vlkn_physical_device
    );
}

bool VulkanRenderer::hasStencilComponent(const VkFormat &format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat VulkanRenderer::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features, const VkPhysicalDevice &vlkn_physical_device)
{
    for (VkFormat format : candidates)
    {
        /*VkFormatProperties struct contains three fields:
            linearTilingFeatures: Use cases that are supported with linear tiling
            optimalTilingFeatures: Use cases that are supported with optimal tiling
            bufferFeatures: Use cases that are supported for buffers*/
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(vlkn_physical_device, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }

        std::cout << std::endl;
        std::cout << "failed to find support fromat" << std::endl;
        throw std::runtime_error("failed to find supported format!");
    }
}
