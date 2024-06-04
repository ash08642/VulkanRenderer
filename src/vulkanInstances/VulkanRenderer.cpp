#include "VulkanRenderer.hpp"

VulkanRenderer::VulkanRenderEngine::VulkanRenderEngine(GLFWwindow *_window, bool* re_size) : window{_window}, reSize{re_size}
{
    time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    createVlknInstance(vlknData.vlknInstance);
    std::cout << "Instance Created" << std::endl;
    
    setupDebugMessenger(vlknData.vlknInstance, vlknData.debugMessenger);
    std::cout << "DebugMessenger Created" << std::endl;

    createSurface(vlknData.vlknSurface, window, vlknData.vlknInstance);
    std::cout << "Surface Created" << std::endl;

    pickPhysicalDevice(vlknData.vlknPhysicalDevice, vlknData.vlknPhysicalDeviceFeatures, vlknData.vlknPhysicalDeviceProperties, vlknData.vlknQueueFamilyIndices, vlknData.vlknMsaaSamples, vlknData.vlknInstance, vlknData.vlknSurface);
    std::cout << "PhysicalDevice Created" << std::endl;

    createLogicalDevice(vlknData.vlknDevice, vlknData.vlknGraphicsQueue,vlknData.presentQueue, vlknData.vlknQueueFamilyIndices, vlknData.vlknPhysicalDevice, vlknData.vlknPhysicalDeviceFeatures);
    std::cout << "LogicalDevice Created" << std::endl;

    createSwapChain(vlknData.vlknSwapChain, vlknData.vlknSwapChainImages, vlknData.vlknSwapChainImageFormat, vlknData.vlknSwapChainExtent, vlknData.VlknSwapChainSupportDetails,
        vlknData.vlknPhysicalDevice, vlknData.vlknDevice, vlknData.vlknSurface, window, vlknData.vlknQueueFamilyIndices);
    std::cout << "SwapChain Created" << std::endl;

    createImageViews(vlknData.vlknSwapChainImageViews, vlknData.vlknSwapChainImages, vlknData.vlknSwapChainImageFormat, vlknData.vlknDevice);
    std::cout << "ImageViews Created" << std::endl;
    
    createRenderPass(vlknData.vlknRenderPass, vlknData.vlknDepthFormat, vlknData.vlknSwapChainImageFormat, vlknData.vlknDevice, vlknData.vlknPhysicalDevice);
    std::cout << "RenderPass Created" << std::endl;
    
    createDescriptorSetLayout(vlknData.vlknDescriptorSetLayout, vlknData.vlknDevice);
    std::cout << "DescriptorSetLayout Created" << std::endl;
    
    createGraphicsPipeline(vlknData.vlknGraphicsPipeline, vlknData.vlknPipelineLayout,vlknData.vlknRenderPass, vlknData.vlknDevice, vlknData.vlknSwapChainExtent, vlknData.vlknDescriptorSetLayout);
    std::cout << "GraphicsPipeline Created" << std::endl;
    
    createDepthResources(vlknData.vlknDepthImage, vlknData.vlknDepthImageMemory, vlknData.vlknDepthImageView, vlknData.vlknDepthFormat, vlknData.vlknSwapChainExtent, vlknData.vlknDevice, vlknData.vlknPhysicalDevice);
    std::cout << "DepthResources Created" << std::endl;
    
    createFrameBuffers(vlknData.vlknSwapChainFramebuffers, vlknData.vlknSwapChainImageViews, vlknData.vlknDepthImageView, vlknData.vlknRenderPass, vlknData.vlknDevice, vlknData.vlknSwapChainExtent);
    std::cout << "FrameBuffers Created" << std::endl;
    
    createCommandPool(vlknData.vlknCommandPool, vlknData.vlknQueueFamilyIndices, vlknData.vlknDevice, CommandPoolType::IxCreateResetCmdBffBit);
    std::cout << "MainCommandPool Created" << std::endl;
    
    createCommandPool(vlknData.vlknTransferCommandPool, vlknData.vlknQueueFamilyIndices, vlknData.vlknDevice, CommandPoolType::IxCreateTransferBit);
    std::cout << "TransferCommandPool Created" << std::endl;

    createTextureImage(vlknData.TEXTURE_PATH, vlknData.vlknTextureImage, vlknData.vlknTextureImageMemory, vlknData.vlknDevice, vlknData.vlknPhysicalDevice, vlknData.vlknSwapChainImageFormat, vlknData.vlknTransferCommandPool, vlknData.vlknGraphicsQueue);
    std::cout << "TextureImage Created" << std::endl;
    
    createTextureImageView(vlknData.vlknTextureImageView, vlknData.vlknTextureImage, vlknData.vlknSwapChainImageFormat, vlknData.vlknDevice);
    std::cout << "TextureImageView Created" << std::endl;
    
    createTextureSampler(vlknData.vlknTextureSampler, vlknData.vlknDevice, vlknData.vlknPhysicalDeviceProperties, vlknData.vlknPhysicalDeviceFeatures);
    std::cout << "TextureSampler Created" << std::endl;
    
    loadModel(vlknData.MODEL_PATH, vlknData.vertices, vlknData.indices);
    std::cout << "Model Created" << std::endl;
    
    createVertexBuffer<std::vector<VlknVertex>>(vlknData.vlknVertexBuffer, vlknData.vlknVertexBufferMemory, BufferType::IxVertexBuffer, vlknData.vertices, vlknData.vlknDevice, vlknData.vlknPhysicalDevice, vlknData.vlknTransferCommandPool, vlknData.vlknGraphicsQueue);
    std::cout << "VertexBuffer Created" << std::endl;
    
    createVertexBuffer<std::vector<uint32_t>>(vlknData.vlknIndexBuffer, vlknData.vlknIndexBufferMemory, BufferType::IxIndexBuffer, vlknData.indices, vlknData.vlknDevice, vlknData.vlknPhysicalDevice, vlknData.vlknTransferCommandPool, vlknData.vlknGraphicsQueue);
    std::cout << "IndexBuffer Created" << std::endl;
    
    createUniformBuffers(vlknData.vlknUniformBuffers, vlknData.vlknUniformBuffersMemory, vlknData.vlknUniformBuffersMapped, vlknData.vlknDevice, vlknData.vlknPhysicalDevice);
    std::cout << "UniformBuffers Created" << std::endl;
    
    createDescriptorPool(vlknData.vlknDescriptorPool, vlknData.vlknDevice);
    std::cout << "DescriptorPool Created" << std::endl;
    
    createDescriptorSets(vlknData.vlknDescriptorSets, vlknData.vlknDescriptorSetLayout, vlknData.vlknDescriptorPool, vlknData.vlknUniformBuffers, vlknData.vlknDevice, vlknData.vlknTextureImageView, vlknData.vlknTextureSampler);
    std::cout << "DescriptorSets Created" << std::endl;
    
    createCommandBuffer(vlknData.vlknCommandBuffers, vlknData.vlknCommandPool, vlknData.vlknDevice);
    std::cout << "CommandBuffer Created" << std::endl;
    
    createSyncObjects(vlknData.vlknImageAvailableSemaphores, vlknData.vlknRenderFinishedSemaphores, vlknData.vlknInFlightFences, vlknData.vlknDevice);
    std::cout << "SyncObjects Created" << std::endl;
    
    std::cout << "size of des set" << vlknData.vlknDescriptorSets.size() << std::endl;
}

VulkanRenderer::VulkanRenderEngine::~VulkanRenderEngine()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(vlknData.vlknDevice, vlknData.vlknImageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(vlknData.vlknDevice, vlknData.vlknRenderFinishedSemaphores[i], nullptr);
        vkDestroyFence(vlknData.vlknDevice, vlknData.vlknInFlightFences[i], nullptr);
    }
    vkDestroyCommandPool(vlknData.vlknDevice, vlknData.vlknCommandPool, nullptr);
    vkDestroyCommandPool(vlknData.vlknDevice, vlknData.vlknTransferCommandPool, nullptr);

    cleanupSwapChain(vlknData.vlknSwapChain, vlknData.vlknSwapChainFramebuffers, vlknData.vlknSwapChainImageViews, vlknData.vlknDepthImage, vlknData.vlknDepthImageMemory, vlknData.vlknDepthImageView, vlknData.vlknDevice);

    vkDestroySampler(vlknData.vlknDevice, vlknData.vlknTextureSampler, nullptr);

    vkDestroyImageView(vlknData.vlknDevice, vlknData.vlknTextureImageView, nullptr);
    vkDestroyImage(vlknData.vlknDevice, vlknData.vlknTextureImage, nullptr);
    vkFreeMemory(vlknData.vlknDevice, vlknData.vlknTextureImageMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(vlknData.vlknDevice, vlknData.vlknUniformBuffers[i], nullptr);
        vkFreeMemory(vlknData.vlknDevice, vlknData.vlknUniformBuffersMemory[i], nullptr);
    }
    
    vkDestroyDescriptorPool(vlknData.vlknDevice, vlknData.vlknDescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(vlknData.vlknDevice, vlknData.vlknDescriptorSetLayout, nullptr);

    vkDestroyBuffer(vlknData.vlknDevice, vlknData.vlknIndexBuffer, nullptr);
    vkFreeMemory(vlknData.vlknDevice, vlknData.vlknIndexBufferMemory, nullptr);

    vkDestroyBuffer(vlknData.vlknDevice, vlknData.vlknVertexBuffer, nullptr);
    vkFreeMemory(vlknData.vlknDevice, vlknData.vlknVertexBufferMemory, nullptr);

    vkDestroyPipeline(vlknData.vlknDevice, vlknData.vlknGraphicsPipeline, nullptr);

    vkDestroyPipelineLayout(vlknData.vlknDevice, vlknData.vlknPipelineLayout, nullptr);

    vkDestroyRenderPass(vlknData.vlknDevice, vlknData.vlknRenderPass, nullptr);

    vkDestroyDevice(vlknData.vlknDevice, nullptr);

    if(enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(vlknData.vlknInstance, vlknData.debugMessenger, nullptr);
    }
    
    vkDestroySurfaceKHR(vlknData.vlknInstance, vlknData.vlknSurface, nullptr);

    vkDestroyInstance(vlknData.vlknInstance, nullptr);

    //glfwDestroyWindow(window);

    glfwTerminate();
}

void VulkanRenderer::VulkanRenderEngine::drawFrame()
{
    /* takes an array of fences and waits on the host for either any or all of the fences to be signaled before returning.
    The VK_TRUE we pass here indicates that we want to wait for all fences.
    UINT64_MAX, which effectively disables the timeout.*/
    vkWaitForFences(vlknData.vlknDevice, 1, &vlknData.vlknInFlightFences[vlknData.vlknCurrentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    /*- the logical device and the swap chain from which we wish to acquire an image.
    - maximum value of a 64 bit unsigned integer means we effectively disable the timeout
    - next two parameters specify synchronization objects that are to be signaled when the presentation engine is finished using the image
    - last parameter specifies a variable to output the index of the swap chain image that has become available.
      The index refers to the VkImage in our swapChainImages array. We're going to use that index to pick the VkFrameBuffer*/
    VkResult result = vkAcquireNextImageKHR(vlknData.vlknDevice, vlknData.vlknSwapChain, UINT64_MAX, vlknData.vlknImageAvailableSemaphores[vlknData.vlknCurrentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) { //check if swapchain needs to be updated
        std::cout << "recreating swap1!" << std::endl;
       recreateSwapChain(
            vlknData.vlknSwapChain, 
            vlknData.vlknSwapChainImages, 
            vlknData.vlknSwapChainImageFormat, 
            vlknData.vlknSwapChainExtent, 
            vlknData.VlknSwapChainSupportDetails,
            vlknData.vlknSwapChainFramebuffers, 
            vlknData.vlknSwapChainImageViews, 
            vlknData.vlknDepthImage,
            vlknData.vlknDepthImageMemory,
            vlknData.vlknDepthImageView,
            vlknData.vlknDepthFormat, 
            vlknData.vlknRenderPass,
            vlknData.vlknPhysicalDevice, 
            vlknData.vlknDevice, 
            vlknData.vlknSurface, 
            window, 
            vlknData.vlknQueueFamilyIndices
        );
        return;
    } else if (result != VK_SUCCESS  && result != VK_SUBOPTIMAL_KHR) {
        std::cout << "failed to acquire swap chain image1!" << std::endl;
        throw std::runtime_error("failed to acquire swap chain image1!");
    }
    updateCamera(vlknData.vlknCurrentFrame);

    vkResetFences(vlknData.vlknDevice, 1, &vlknData.vlknInFlightFences[vlknData.vlknCurrentFrame]);   //manually reset the fence to the unsignaled state
    
    vkResetCommandBuffer(vlknData.vlknCommandBuffers[vlknData.vlknCurrentFrame], 0);    //make sure the command buffer is able to be recorded
    //record the commands we want, notice recordCommandBuffer(...) is a function that is custom made
    recordCommandBuffer
    (
        vlknData.vlknCommandBuffers[vlknData.vlknCurrentFrame], 
        imageIndex, 
        vlknData.vlknSwapChainFramebuffers, 
        vlknData.vlknSwapChainExtent, 
        vlknData.vlknRenderPass, 
        vlknData.vlknGraphicsPipeline, 
        vlknData.vlknVertexBuffer, 
        vlknData.vlknIndexBuffer,
        vlknData.vlknDescriptorSets,
        vlknData.vlknCurrentFrame,
        vlknData.vlknPipelineLayout,
        static_cast<uint32_t>(vlknData.indices.size())
    );

    //Submitting the commad buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    /*The first three parameters specify which semaphores to wait on before execution begins and in which stage(s) of the pipeline to wait. */
    VkSemaphore waitSemaphores[] = {vlknData.vlknImageAvailableSemaphores[vlknData.vlknCurrentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    //specify which command buffers to actually submit for execution
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vlknData.vlknCommandBuffers[vlknData.vlknCurrentFrame];
    //the next parameters specify which semaphores to signal once the command buffer(s) have finished execution.
    VkSemaphore signalSemaphores[] = {vlknData.vlknRenderFinishedSemaphores[vlknData.vlknCurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    /*last parameter references an optional fence that will be signaled when the command buffers finish execution. 
    Now on the next frame, the CPU will wait for this command buffer to finish executing before it records new commands into it*/
    if (vkQueueSubmit(vlknData.vlknGraphicsQueue, 1, &submitInfo, vlknData.vlknInFlightFences[vlknData.vlknCurrentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    //Presenting the Frame
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    //next two fields specify which semaphores to wait on before presentation can happen
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    //next four parameters specify the swap chains to present images to and the index of the image for each swap chain
    VkSwapchainKHR swapChains[] = {vlknData.vlknSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional //specify an array of VkResult values to check for every individual swap chain if presentation was successful
    //Finally present the frame
    result = vkQueuePresentKHR(vlknData.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || *reSize) {
        std::cout << "recreating swap2!" << std::endl;
        *reSize = false;
        recreateSwapChain(
            vlknData.vlknSwapChain, 
            vlknData.vlknSwapChainImages, 
            vlknData.vlknSwapChainImageFormat, 
            vlknData.vlknSwapChainExtent, 
            vlknData.VlknSwapChainSupportDetails,
            vlknData.vlknSwapChainFramebuffers, 
            vlknData.vlknSwapChainImageViews, 
            vlknData.vlknDepthImage,
            vlknData.vlknDepthImageMemory,
            vlknData.vlknDepthImageView,
            vlknData.vlknDepthFormat, 
            vlknData.vlknRenderPass,
            vlknData.vlknPhysicalDevice, 
            vlknData.vlknDevice, 
            vlknData.vlknSurface, 
            window, 
            vlknData.vlknQueueFamilyIndices
        );
    } else if (result != VK_SUCCESS) {
        std::cout << "failed to acquire swap chain image2!" << std::endl;
        throw std::runtime_error("failed to present swap chain image2!");
    }

    vlknData.vlknCurrentFrame = (vlknData.vlknCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    //Show FPS
    unsigned __int64 time2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (time2 > time)
    {
        std::cout << "fps: "<< (1000/(time2 - time)) << std::endl;
    }
    time = time2;
}

void VulkanRenderer::VulkanRenderEngine::vlknDeviceWaitIdle()
{
    vkDeviceWaitIdle(vlknData.vlknDevice);
}

void VulkanRenderer::VulkanRenderEngine::resizeFrameBuffer()
{
    vlknData.framebufferResized = true;
}

void VulkanRenderer::VulkanRenderEngine::updateCamera(uint_least32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), vlknData.vlknSwapChainExtent.width / (float) vlknData.vlknSwapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    memcpy(vlknData.vlknUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}
