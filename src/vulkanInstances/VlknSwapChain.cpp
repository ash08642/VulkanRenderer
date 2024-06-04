#include "VlknSwapChain.hpp"

void VulkanRenderer::createSwapChain
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
)
{
    querySwapChainSupport(vlkn_swap_chain_support_details, vlkn_physical_device, vlkn_surface);
    if(vlkn_swap_chain_support_details.formats.empty() || vlkn_swap_chain_support_details.presentModes.empty())
    {
        throw std::runtime_error("swapchain is not supported by GPU");
    }

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(vlkn_swap_chain_support_details.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(vlkn_swap_chain_support_details.presentModes);
    vlkn_swap_chain_extent = chooseSwapExtent(vlkn_swap_chain_support_details.capabilities, window);
    vlkn_swap_chain_image_format = surfaceFormat.format;

    if (vlkn_swap_chain_image_format == VK_FORMAT_UNDEFINED)
    {
        std::cout << "swap format undefined2" << std::endl;
    }

    uint32_t imageCount = vlkn_swap_chain_support_details.capabilities.minImageCount + 1;	// number of images we want to use 1 ... 64
	if (vlkn_swap_chain_support_details.capabilities.maxImageCount > 0 && imageCount > vlkn_swap_chain_support_details.capabilities.maxImageCount)	// make sure to not exceed the maximum number of images
	{
		imageCount = vlkn_swap_chain_support_details.capabilities.maxImageCount;
	}

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vlkn_surface;  //specify which surface the swap chain should be tied to
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = vlkn_swap_chain_extent;
    createInfo.imageArrayLayers = 1;    //specifies the amount of layers each image consists of. This is always 1 unless you are developing a stereoscopic 3D application.
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;    //VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT: render directly to the images. Use VK_IMAGE_USAGE_TRANSFER_DST_BIT for preprocessing

    uint32_t queueFamilyIndices[] = {vlkn_queue_family_indices.graphicsFamily.value(), vlkn_queue_family_indices.presentFamily.value()};
    if (vlkn_queue_family_indices.graphicsFamily != vlkn_queue_family_indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;   //Images can be used across multiple queue families without explicit ownership transfers.
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;    //An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family. This option offers the best performance.
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = vlkn_swap_chain_support_details.capabilities.currentTransform;    //do not want any transformation
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;  //ignore the alpha channel, used for blending with other windows in the window system
    createInfo.presentMode = presentMode;   //FIFO
    createInfo.clipped = VK_TRUE;   //don't care about the color of pixels that are obscured, for example because another window is in front of them
    createInfo.oldSwapchain = VK_NULL_HANDLE;   //for resizing, not implemented yet

    if (vkCreateSwapchainKHR(vlkn_device, &createInfo, nullptr, &vlkn_swap_chain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(vlkn_device, vlkn_swap_chain, &imageCount, nullptr);    //retrieve the handles of images
    vlkn_swap_chain_images.resize(imageCount);
    vkGetSwapchainImagesKHR(vlkn_device, vlkn_swap_chain, &imageCount, vlkn_swap_chain_images.data());
}

void VulkanRenderer::recreateSwapChain
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
)
{
    vkDeviceWaitIdle(vlkn_device);  //call vkDeviceWaitIdle because we shouldn't touch resources that may still be in use

    cleanupSwapChain(vlkn_swap_chain, vlkn_swap_chain_framebuffers, vlkn_swap_chain_image_views, vlkn_depth_image, vlkn_depth_image_memory, vlkn_depth_image_view, vlkn_device);

    createSwapChain(vlkn_swap_chain, vlkn_swap_chain_images, vlkn_swap_chain_image_format, vlkn_swap_chain_extent, vlkn_swap_chain_support_details,
    vlkn_physical_device, vlkn_device, vlkn_surface, window, vlkn_queue_family_indices);

    createImageViews(vlkn_swap_chain_image_views, vlkn_swap_chain_images, vlkn_swap_chain_image_format, vlkn_device);
    
    createDepthResources(vlkn_depth_image, vlkn_depth_image_memory, vlkn_depth_image_view, vlkn_depth_format, vlkn_swap_chain_extent, vlkn_device, vlkn_physical_device);
    createFrameBuffers(vlkn_swap_chain_framebuffers, vlkn_swap_chain_image_views, vlkn_depth_image_view, vlkn_render_pass, vlkn_device, vlkn_swap_chain_extent);
}

void VulkanRenderer::cleanupSwapChain
(
    VkSwapchainKHR& vlkn_swap_chain,
    std::vector<VkFramebuffer>& vlkn_swap_chain_framebuffers,
    std::vector<VkImageView>& vlkn_swap_chain_image_views,
    VkImage& vlkn_depth_image,
    VkDeviceMemory& vlkn_depth_image_memory,
    VkImageView& vlkn_depth_image_view,
    const VkDevice& vlkn_device
){
    vkDestroyImageView(vlkn_device, vlkn_depth_image_view, nullptr);
    vkDestroyImage(vlkn_device, vlkn_depth_image, nullptr);
    vkFreeMemory(vlkn_device, vlkn_depth_image_memory, nullptr);

    for (size_t i = 0; i < vlkn_swap_chain_framebuffers.size(); i++) {
        vkDestroyFramebuffer(vlkn_device, vlkn_swap_chain_framebuffers[i], nullptr);
    }

    for (size_t i = 0; i < vlkn_swap_chain_image_views.size(); i++) {
        vkDestroyImageView(vlkn_device, vlkn_swap_chain_image_views[i], nullptr);
    }

    vkDestroySwapchainKHR(vlkn_device, vlkn_swap_chain, nullptr);
}

void VulkanRenderer::querySwapChainSupport(VlknSwapChainSupportDetails &vlkn_swap_chain_support_details, const VkPhysicalDevice &vlkn_physical_device, const VkSurfaceKHR &vlkn_surface)
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vlkn_physical_device, vlkn_surface, &vlkn_swap_chain_support_details.capabilities);   //query surface capabilities

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vlkn_physical_device, vlkn_surface, &formatCount, nullptr);
    if (formatCount != 0) {
        vlkn_swap_chain_support_details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(vlkn_physical_device, vlkn_surface, &formatCount, vlkn_swap_chain_support_details.formats.data()); //querying the supported surface formats
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vlkn_physical_device, vlkn_surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        vlkn_swap_chain_support_details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(vlkn_physical_device, vlkn_surface, &presentModeCount, vlkn_swap_chain_support_details.presentModes.data());  //querying the supported presentation modes
    }
}

VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &available_formats)
{
    for (const auto& availableFormat : available_formats)
    {
        /*
        '.format' specifies the color channels and types
        '.colorSpace' indicates if the SRGB color space is supported or not
        */
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &available_present_modes)
{
    for (const auto& availablePresentMode : available_present_modes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow* window)
{
    #ifdef max
        #undef max
    #endif
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    #define max(a,b) (((a) > (b)) ? (a) : (b))
    {
        return capabilities.currentExtent;
    } else 
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);    // query the resolution of the window in pixel

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
