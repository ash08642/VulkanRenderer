#include "VlknCommandPool.hpp"

void VulkanRenderer::createCommandPool(VkCommandPool& vlkn_command_pool, const VlknQueueFamilyIndices &vlkn_queue_family_indices, const VkDevice &vlkn_device, CommandPoolType command_pool_type)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    /*two possible flags for command pools:
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow command buffers to be rerecorded individually, without this flag they all have to be reset together*/
    poolInfo.flags = command_pool_type; // VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = vlkn_queue_family_indices.graphicsFamily.value();

    if (vkCreateCommandPool(vlkn_device, &poolInfo, nullptr, &vlkn_command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void VulkanRenderer::createCommandBuffer(std::vector<VkCommandBuffer>& vlkn_command_buffers, const VkCommandPool &vlkn_command_pool, const VkDevice &vlkn_device)
{
    vlkn_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vlkn_command_pool;
    /*VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers.
    VK_COMMAND_BUFFER_LEVEL_SECONDARY: Cannot be submitted directly, but can be called from primary command buffers.*/
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) vlkn_command_buffers.size();;   //number of buffers to allocate

    if (vkAllocateCommandBuffers(vlkn_device, &allocInfo, vlkn_command_buffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void VulkanRenderer::recordCommandBuffer
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
)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional    //specifies how we're going to use the command buffer.
    beginInfo.pInheritanceInfo = nullptr; // Optional   //pInheritanceInfo parameter is only relevant for secondary command buffers. It specifies which state to inherit from the calling primary command buffers.

    if (vkBeginCommandBuffer(vlkn_command_buffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vlkn_render_pass;
        renderPassInfo.framebuffer = vlkn_swap_chain_frame_buffers[swap_chain_image_index];
        renderPassInfo.renderArea.offset = {0, 0};                  //define the size of the render area
        renderPassInfo.renderArea.extent = vlkn_swap_chain_extent;  //define the size of the render area

        std::array<VkClearValue, 2> clearValues{};  //define the clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, color to simply be black with 100% opacity
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        /*The first parameter for every command is always the command buffer to record the command to. 
        The second parameter specifies the details of the render pass we've just provided. 
        The final parameter controls how the drawing commands within the render pass will be provided. It can have one of two values:
            VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed.
            VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.*/
        vkCmdBeginRenderPass(vlkn_command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            //Basic Drawing Commands
            vkCmdBindPipeline(vlkn_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vlkn_graphics_pipeline);    //second parameter specifies if the pipeline object is a graphics or compute pipeline
            /*we did specify viewport and scissor state for this pipeline to be dynamic.
            So we need to set them in the command buffer before issuing our draw command:*/
            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(vlkn_swap_chain_extent.width);
            viewport.height = static_cast<float>(vlkn_swap_chain_extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(vlkn_command_buffer, 0, 1, &viewport);
            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = vlkn_swap_chain_extent;
            vkCmdSetScissor(vlkn_command_buffer, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = {vlkn_vertex_buffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(vlkn_command_buffer, 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(vlkn_command_buffer, vlkn_index_buffer, 0, VK_INDEX_TYPE_UINT32);
            /* It has the following parameters, aside from the command buffer:
                vertexCount: Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
                instanceCount: Used for instanced rendering, use 1 if you're not doing that.
                firstVertex: Used as an offset into the vertex buffer, defines the lowest value of gl_VertexIndex.
                firstInstance: Used as an offset for instanced rendering, defines the lowest value of gl_InstanceIndex.*/
            vkCmdBindDescriptorSets(vlkn_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vlkn_pipeline_layout, 0, 1, &vlkn_descriptor_sets[vlkn_current_frame], 0, nullptr);
            vkCmdDrawIndexed(vlkn_command_buffer, indicesSize, 1, 0, 0, 0);
            //vkCmdDraw(vlkn_command_buffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
        vkCmdEndRenderPass(vlkn_command_buffer);

    if (vkEndCommandBuffer(vlkn_command_buffer) != VK_SUCCESS){
        throw std::runtime_error("failed to record command buffer!");
    }
}

VkCommandBuffer VulkanRenderer::beginSingleTimeCommands(const VkDevice &vlkn_device, const VkCommandPool &vlkn_command_pool)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vlkn_command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(vlkn_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanRenderer::endSingleTimeCommands(VkCommandBuffer commandBuffer, const VkCommandPool &vlkn_command_pool, const VkQueue &vlkn_graphics_queue, const VkDevice& vlkn_device)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(vlkn_graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vlkn_graphics_queue);

    vkFreeCommandBuffers(vlkn_device, vlkn_command_pool, 1, &commandBuffer);
}
