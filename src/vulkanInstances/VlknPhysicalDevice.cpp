#include "VlknPhysicalDevice.hpp"

void VulkanRenderer::pickPhysicalDevice(VkPhysicalDevice& vlkn_device, VkPhysicalDeviceFeatures& vlkn_physical_device_features, VkPhysicalDeviceProperties& vlkn_physical_device_properties, VlknQueueFamilyIndices& vlkn_queue_family_indices, VkSampleCountFlagBits& vlkn_msaa_samples, const VkInstance& vlkn_instance, const VkSurfaceKHR& vlkn_surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vlkn_instance, &deviceCount, nullptr);
    if (deviceCount == 0) { // If there are 0 devices with Vulkan support then there is no point going further.
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount); // Create a vector to hold all the available VkPhysicalDevice handles
    vkEnumeratePhysicalDevices(vlkn_instance, &deviceCount, devices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        vlkn_device = candidates.rbegin()->second;
        vkGetPhysicalDeviceFeatures(candidates.rbegin()->second, &vlkn_physical_device_features);	// Query
        vkGetPhysicalDeviceProperties(candidates.rbegin()->second, &vlkn_physical_device_properties);
        vlkn_msaa_samples = getMaxUsableSampleCount(vlkn_physical_device_properties);
        findQueueFamilies(vlkn_device, vlkn_queue_family_indices, vlkn_surface);
        if (!vlkn_queue_family_indices.isComplete())
        {
            throw std::runtime_error("GPU does not support required queue families!");
        }
        if (!checkDeviceExtensionSupport(vlkn_device))
        {
            throw std::runtime_error("GPU does not support required device extensions");
        }
        
        
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}


int VulkanRenderer::rateDeviceSuitability(const VkPhysicalDevice& device)
{
    VkPhysicalDeviceProperties deviceProperties;	// Basic device properties like the name, type and supported Vulkan version
	vkGetPhysicalDeviceProperties(device, &deviceProperties);	// Query
	std::cout << deviceProperties.deviceName << std::endl;
	
	VkPhysicalDeviceFeatures deviceFeatures;	// support for optional features
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);	// Query

    int score = 0;
    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) 
    {
        score += 1000;
    }
    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;
    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader) 
    {
        return 0;
    }
    return score;
}

bool VulkanRenderer::checkDeviceExtensionSupport(const VkPhysicalDevice &vlkn_device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(vlkn_device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(vlkn_device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void VulkanRenderer::findQueueFamilies(VkPhysicalDevice vlkn_device, VlknQueueFamilyIndices &vlkn_queue_family_indices, const VkSurfaceKHR& vlkn_surface)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vlkn_device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vlkn_device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            vlkn_queue_family_indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;	// Check for Windows Surface Support
		vkGetPhysicalDeviceSurfaceSupportKHR(vlkn_device, i, vlkn_surface, &presentSupport);

		if (presentSupport)
		{
			vlkn_queue_family_indices.presentFamily = i;
		}
        if (vlkn_queue_family_indices.isComplete())
		{
			break;
		}
        i++;
    }
}

VkSampleCountFlagBits VulkanRenderer::getMaxUsableSampleCount(const VkPhysicalDeviceProperties &vlkn_physical_device_properties)
{
    VkSampleCountFlags counts = vlkn_physical_device_properties.limits.framebufferColorSampleCounts & vlkn_physical_device_properties.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}
