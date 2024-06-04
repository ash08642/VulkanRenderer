#include "VlknInstance.hpp"

void VulkanRenderer::createVlknInstance(VkInstance& vlkn_instance)
{
    if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}
    
    VkApplicationInfo appInfo{};	// Information about our Application(Optional). Useful for Optimization
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Ashoxzy Engine";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 2, 0);
	appInfo.pEngineName = "Ashoxzy Engine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 2, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;	// 1_0 in tutorial pdf

    VkInstanceCreateInfo vlknInstanceCI{};
    vlknInstanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vlknInstanceCI.pApplicationInfo = &appInfo;
    vlknInstanceCI.pNext = nullptr;

    std::vector<const char*> extensions = getRequiredExtensions();
	vlknInstanceCI.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	vlknInstanceCI.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers)
	{
		vlknInstanceCI.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		vlknInstanceCI.ppEnabledLayerNames = validationLayers.data();
	
		populateDebugMessengerCreateInfo(debugCreateInfo);
		vlknInstanceCI.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		vlknInstanceCI.enabledLayerCount = 0;
		vlknInstanceCI.pNext = nullptr;
	}

	
	uint32_t glfwExtensionCount = 0;	//GLFW has a handy built - in function that returns the extension(s) it needs (ti interface with the window system) to do that which we can pass to the struct
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    if (vkCreateInstance( &vlknInstanceCI, nullptr, &vlkn_instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}

std::vector<const char*> VulkanRenderer::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);	// VK_KHR_surface, ...

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}