#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createFrameBuffers(std::vector<VkFramebuffer>& vlkn_swap_chain_frame_buffers, const std::vector<VkImageView>& vlkn_swap_chain_image_views, const VkImageView& vlkn_depth_image_view, const VkRenderPass& vlkn_render_pass, const VkDevice& vlkn_device, const VkExtent2D& vlkn_swap_chain_extent);
}
/*
A framebuffer object references all of the VkImageView objects that represent the attachments. In our case 
that will be only a single one: the color attachment. However, the image that we have to use for the attachment 
depends on which image the swap chain returns when we retrieve one for presentation. That means that we have 
to create a framebuffer for all of the images in the swap chain and use the one that corresponds to the retrieved 
image at drawing time.
*/