#include "VlknLogicalDevice.hpp"

void VulkanRenderer::createLogicalDevice(VkDevice& vlkn_logical_device, VkQueue& vlkn_graphics_queue, VkQueue& vlkn_present_queue, const VlknQueueFamilyIndices& vlkn_queue_family_indices, const VkPhysicalDevice& vlkn_physical_device, VkPhysicalDeviceFeatures& vlkn_physical_device_features)
{

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {vlkn_queue_family_indices.graphicsFamily.value(), vlkn_queue_family_indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        std::cout << "Queue Family: " << queueFamily << std::endl;
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &vlkn_physical_device_features;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(vlkn_physical_device, &createInfo, nullptr, &vlkn_logical_device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(vlkn_logical_device, vlkn_queue_family_indices.graphicsFamily.value(), 0, &vlkn_graphics_queue);
    vkGetDeviceQueue(vlkn_logical_device, vlkn_queue_family_indices.presentFamily.value(), 0, &vlkn_present_queue);
}
