#pragma once

#include "VlknData.hpp"
#include "VlknDepthBuffer.hpp"

namespace VulkanRenderer
{
    void createRenderPass(VkRenderPass& vlkn_render_pass, VkFormat& vlkn_depth_format, const VkFormat& vlkn_swap_chain_image_format, const VkDevice& vlkn_device, const VkPhysicalDevice& vlkn_physical_device);
}
/*
Before we can finish creating the pipeline, we need to tell Vulkan about the framebuffer attachments that will be used while rendering.
We need to specify how many color and depth buffers there will be, how many samples to use for each of them and how their contents should
be handled throughout the rendering operations. All of this information is wrapped in a render pass object, for which we'll create a new
createRenderPass function. Call this function from initVulkan before createGraphicsPipeline.
*/