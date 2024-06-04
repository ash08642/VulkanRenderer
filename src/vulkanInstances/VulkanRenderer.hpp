#pragma once

#include "VlknData.hpp"
#include "VlknInstance.hpp"
#include "VlknPhysicalDevice.hpp"
#include "VlknLogicalDevice.hpp"
#include "VlknWindowSurface.hpp"
#include "VlknSwapChain.hpp"
#include "VlknImageViews.hpp"
#include "VlknGraphicsPipeline.hpp"
#include "VlknRenderPass.hpp"
#include "VlknFrameBuffer.hpp"
#include "VlknCommandPool.hpp"
#include "VlknSyncObjects.hpp"
#include "VlknVertexBuffers.hpp"
#include "VlknDescriptorSetLayout.hpp"
#include "VlknTextureImage.hpp"
#include "VlknDepthBuffer.hpp"
#include "VlknModelLoader.hpp"

namespace VulkanRenderer
{
    class VulkanRenderEngine
    {
    private:
        VlknData vlknData{};
        GLFWwindow* window;
        bool* reSize;
        unsigned __int64 time;    //for fps

    public:
        VulkanRenderEngine(GLFWwindow* _window, bool* re_size);
        ~VulkanRenderEngine();

        void drawFrame();
        void vlknDeviceWaitIdle();
        void resizeFrameBuffer();
        void updateCamera(uint_least32_t currentImage); //updateUniformBuffer()
    };
}
