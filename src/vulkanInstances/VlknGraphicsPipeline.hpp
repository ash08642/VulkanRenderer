#pragma once

#include <fstream>

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createGraphicsPipeline(VkPipeline& vlkn_graphics_pipeline, VkPipelineLayout& vlkn_pipeline_layout, const VkRenderPass& vlkn_render_pass, const VkDevice& vlkn_device, const VkExtent2D& vlkn_swap_chain_extent, const VkDescriptorSetLayout& vlkn_descriptor_set_layout);
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code, const VkDevice& vlkn_device);
}

/*  Steps taken to Create Graphics Pipline
Shader stages: the shader modules that define the functionality of the programmable stages of the graphics pipeline
Fixed-function state: all of the structures that define the fixed-function stages of the pipeline, like input assembly, rasterizer, viewport and color blending
Pipeline layout: the uniform and push values referenced by the shader that can be updated at draw time
Render pass: the attachments referenced by the pipeline stages and their usage (See VlknRenderPass.hpp)
*/