#pragma once

#include "vulkanInstances/GraphicPipeline.hpp"
#include "glfwWindow/Window.hpp"
#include "vulkanInstances/VulkanRenderer.hpp"

namespace LiveWindow
{
    class Application
    {
    public:
        static constexpr unsigned int WIDHT = 800;
        static constexpr unsigned int HEIGHT = 800;

        void run();
    private:
        bool reSize = false;
        Window glfwWindow{"Bild-er", WIDHT, HEIGHT, &reSize};  
        GraphicPipeline graphicsPipeline{"src/shaders/simpleShader.vert.spv", "src/shaders/simpleShader.frag.spv"};
        VulkanRenderer::VulkanRenderEngine vulkanRenderer{glfwWindow.getWindow(), &reSize};
    };
}