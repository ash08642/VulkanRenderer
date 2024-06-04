#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    enum CommandPoolType
    {
        IxCreateResetCmdBffBit = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        IxCreateTransferBit = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
    };

    void createCommandPool(VkCommandPool& vlkn_command_pool, const VlknQueueFamilyIndices& vlkn_queue_family_indices, const VkDevice& vlkn_device, CommandPoolType command_pool_type);
    void createCommandBuffer(std::vector<VkCommandBuffer>& vlkn_command_buffers, const VkCommandPool& vlkn_command_pool, const VkDevice& vlkn_device);
    void recordCommandBuffer
    (
        VkCommandBuffer& vlkn_command_buffer, 
        const uint32_t swap_chain_image_index, 
        const std::vector<VkFramebuffer>& vlkn_swap_chain_frame_buffers,
        const VkExtent2D& vlkn_swap_chain_extent,
        const VkRenderPass& vlkn_render_pass, 
        const VkPipeline vlkn_graphics_pipeline,
        const VkBuffer& vlkn_vertex_buffer,
        const VkBuffer& vlkn_index_buffer,
        const std::vector<VkDescriptorSet>& vlkn_descriptor_sets,
        uint32_t vlkn_current_frame,
        const VkPipelineLayout& vlkn_pipeline_layout,
        const uint32_t indicesSize
    );

    VkCommandBuffer beginSingleTimeCommands(const VkDevice& vlkn_device, const VkCommandPool& vlkn_command_pool);
    void endSingleTimeCommands(VkCommandBuffer commandBuffer, const VkCommandPool& vlkn_command_pool, const VkQueue& vlkn_graphics_queue, const VkDevice& vlkn_device);
}
/*
Commands in Vulkan, like drawing operations and memory transfers, are not executed directly using function calls. 
You have to record all of the operations you want to perform in command buffer objects.
In addition, this allows command recording to happen in multiple threads if so desired.

We have to create a command pool before we can create command buffers. Command pools manage the memory that is used 
to store the buffers and command buffers are allocated from them.
*/