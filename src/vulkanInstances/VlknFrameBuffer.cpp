#include "VlknFrameBuffer.hpp"

void VulkanRenderer::createFrameBuffers(std::vector<VkFramebuffer> &vlkn_swap_chain_frame_buffers, const std::vector<VkImageView> &vlkn_swap_chain_image_views, const VkImageView& vlkn_depth_image_view, const VkRenderPass &vlkn_render_pass, const VkDevice &vlkn_device, const VkExtent2D& vlkn_swap_chain_extent)
{
    vlkn_swap_chain_frame_buffers.resize(vlkn_swap_chain_image_views.size());
    for (size_t i = 0; i < vlkn_swap_chain_image_views.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {
            vlkn_swap_chain_image_views[i],
            vlkn_depth_image_view
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vlkn_render_pass;  // specify with which renderPass the framebuffer needs to be compatible
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data(); //specify the VkImageView objects that should be bound to the respective attachment descriptions in the render pass pAttachment array
        framebufferInfo.width = vlkn_swap_chain_extent.width;
        framebufferInfo.height = vlkn_swap_chain_extent.height;
        framebufferInfo.layers = 1; // Our swap chain images are single images, so the number of layers is 1

        if (vkCreateFramebuffer(vlkn_device, &framebufferInfo, nullptr, &vlkn_swap_chain_frame_buffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    
}
