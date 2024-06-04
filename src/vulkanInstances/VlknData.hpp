#pragma once

#define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#ifndef VK_USE_PLATFORM_WIN32_KHR
    #define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h>
#ifndef GLFW_INCLUDE_VULKAN
    #define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#ifndef GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <tiny_obj_loader.h>

#include <iostream>
#include <stdexcept>
#include <optional>
#include <vector>
#include <set>
#include <chrono>   //for time

#include "VertexInfo.hpp"

namespace VulkanRenderer
{
    /*const std::vector<VlknVertex> vertices = 
    {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
    const std::vector<uint16_t> indices =   //uint16_t because we r using < 65535 unique vertices
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };*/

    struct VlknQueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
    struct VlknSwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct VlknData
    {
        VkInstance vlknInstance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger;    //for debugging
        VkPhysicalDevice vlknPhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties vlknPhysicalDeviceProperties{};
        VkPhysicalDeviceFeatures vlknPhysicalDeviceFeatures{};
        VlknQueueFamilyIndices vlknQueueFamilyIndices;
        VkDevice vlknDevice = VK_NULL_HANDLE;
        VkQueue vlknGraphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        VkSurfaceKHR vlknSurface = VK_NULL_HANDLE;

        VlknSwapChainSupportDetails VlknSwapChainSupportDetails{};
        VkSwapchainKHR vlknSwapChain = VK_NULL_HANDLE;
        VkFormat vlknSwapChainImageFormat;
        VkExtent2D vlknSwapChainExtent;
        std::vector<VkImage> vlknSwapChainImages;
        std::vector<VkImageView> vlknSwapChainImageViews;

        VkDescriptorSetLayout vlknDescriptorSetLayout;
        VkDescriptorPool vlknDescriptorPool;
        std::vector<VkDescriptorSet> vlknDescriptorSets;

        VkPipelineLayout vlknPipelineLayout = VK_NULL_HANDLE;
        VkRenderPass vlknRenderPass;
        VkPipeline vlknGraphicsPipeline;
        std::vector<VkFramebuffer> vlknSwapChainFramebuffers;

        VkCommandPool vlknCommandPool;
        std::vector<VkCommandBuffer> vlknCommandBuffers;    //Each frame should have its own command buffer

        std::vector<VkSemaphore> vlknImageAvailableSemaphores;  //Each frame should have its own set of semaphores
        std::vector<VkSemaphore> vlknRenderFinishedSemaphores;  //Each frame should have its own set of semaphores
        std::vector<VkFence> vlknInFlightFences;    //Each frame should have its own fence

        uint32_t vlknCurrentFrame = 0;

        bool framebufferResized = false;    //for handling window resizes

        VkBuffer vlknVertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vlknVertexBufferMemory = VK_NULL_HANDLE;
        VkBuffer vlknIndexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vlknIndexBufferMemory = VK_NULL_HANDLE;
        VkCommandPool vlknTransferCommandPool = VK_NULL_HANDLE;

        std::vector<VkBuffer> vlknUniformBuffers;
        std::vector<VkDeviceMemory> vlknUniformBuffersMemory;
        std::vector<void*> vlknUniformBuffersMapped;    //get a pointer to which we can write the data later on

        //for storing textures
        VkImage vlknTextureImage;
        VkDeviceMemory vlknTextureImageMemory;
        VkImageView vlknTextureImageView;
        VkSampler vlknTextureSampler;

        //for Depth buffering
        VkImage vlknDepthImage;
        VkDeviceMemory vlknDepthImageMemory;
        VkImageView vlknDepthImageView;
        VkFormat vlknDepthFormat;

        //for Multisampling
        VkSampleCountFlagBits vlknMsaaSamples = VK_SAMPLE_COUNT_1_BIT;
        VkImage vlknColorImage;
        VkDeviceMemory vlknColorImageMemory;
        VkImageView vlknColorImageView;

        const char* MODEL_PATH = "src/models/viking_room.obj.txt";
        const char* TEXTURE_PATH = "src/images/viking_room.png";
        std::vector<VlknVertex> vertices;
        std::vector<uint32_t> indices;
    };

    const int MAX_FRAMES_IN_FLIGHT = 2; // how many frames should be processed concurrently

    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    // for debuging
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };    //see below of validationLayer options
    #ifdef NDEBUG
    const bool enableValidationLayers = false;
    #else
    const bool enableValidationLayers = true;
    #endif // NDEBUG

    bool checkValidationLayerSupport();
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    void setupDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT& debug_messenger);
}

/* ValidationLayer Options
 • VK_LAYER_LUNARG_api_dump prints Vulkan calls and their parameters and values to the
 console.
 • VK_LAYER_LUNARG_core_validation performs validation on parameters and state used
 in descriptor sets, pipeline state, and dynamic state; validates the interfaces between SPIR-V
 modules and the graphics pipeline; and tracks and validates usage of GPU memory used to back
 objects.
 • VK_LAYER_LUNARG_device_limits ensures that values passed to Vulkan commands as
 arguments or data structure members fall within the device’s supported feature set limits.
 • VK_LAYER_LUNARG_image validates that image usage is consistent with supported formats.
 • VK_LAYER_LUNARG_object_tracker performs tracking on Vulkan objects, attempting to
 catch leaks, use-after-free errors, and other invalid object usage.
 • VK_LAYER_LUNARG_parameter_validation confirms that all parameter values passed
 to Vulkan functions are valid.
 • VK_LAYER_LUNARG_swapchain performs validation on functionality provided by the WSI
 (Window System Integration) extensions described in Chapter 5, “Presentation.”
 • VK_LAYER_GOOGLE_threading ensures valid usage of Vulkan commands with respect to
 threading, ensuring that no two threads access the same object at the same time when they
 shouldn’t.
 • VK_LAYER_GOOGLE_unique_objects ensures that every object will have a unique handle
 for easier tracking by the application, avoiding cases where an implementation might de
 duplicate handles that represent objects with the same parameters.
*/