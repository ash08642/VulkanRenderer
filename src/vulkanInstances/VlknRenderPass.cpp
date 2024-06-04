#include "VlknRenderPass.hpp"

void VulkanRenderer::createRenderPass(VkRenderPass& vlkn_render_pass, VkFormat& vlkn_depth_format, const VkFormat& vlkn_swap_chain_image_format, const VkDevice& vlkn_device, const VkPhysicalDevice& vlkn_physical_device)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vlkn_swap_chain_image_format;  //format of the color attachment should match the format of the swap chain images
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;    //we're not doing anything with multisampling yet, so we'll stick to 1 sample
    if (vlkn_swap_chain_image_format == VK_FORMAT_UNDEFINED)
    {
        std::cout << "swap format undefined" << std::endl;
    }
    
    /*The loadOp and storeOp determine what to do with the data in the attachment before rendering and after rendering. We have the following choices for loadOp:
        VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
        VK_ATTACHMENT_LOAD_OP_CLEAR: Clear the values to a constant at the start
        VK_ATTACHMENT_LOAD_OP_DONT_CARE: Existing contents are undefined; we don't care about them
    In our case we're going to use the clear operation to clear the framebuffer to black before drawing a new frame. 
    There are only two possibilities for the storeOp:
        VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memory and can be read later
        VK_ATTACHMENT_STORE_OP_DONT_CARE: Contents of the framebuffer will be undefined after the rendering operation*/
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    
    /*The loadOp and storeOp apply to color and depth data, and stencilLoadOp / stencilStoreOp apply to stencil data.
    Our application won't do anything with the stencil buffer, so the results of loading and storing are irrelevant.*/
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    /*Some of the most common layouts are:
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: Images to be presented in the swap chain
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: Images to be used as destination for a memory copy operation
        VK_IMAGE_LAYOUT_UNDEFINED for initialLayout means that we don't care what previous layout the image was in*/
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  //specifies which layout the image will have before the render pass begins
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  //specifies the layout to automatically transition to when the render pass finishes

    /*A single render pass can consist of multiple subpasses. Subpasses are subsequent rendering operations that depend on the contents 
    of framebuffers in previous passes, for example a sequence of post-processing effects that are applied one after another. */
    VkAttachmentReference colorAttachmentRef{};
    /*specifies which attachment to reference by its index in the attachment descriptions array.
    Our array consists of a single VkAttachmentDescription, so its index is 0*/
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;   //VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: use the referenced attachment as a color buffer with best performance

    vlkn_depth_format = findDepthFormat(vlkn_physical_device);

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = vlkn_depth_format; //should be the same as the depth image itself
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //it will not be used after drawing has finished
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  //we don't care about the previous depth contents
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;    //explicit about this being a graphics subpass, use VK_PIPELINE_BIND_POINT_COMPUTE for compute
    subpass.colorAttachmentCount = 1;
    /*The index of the attachment(colorAttachmentRef.attachment) in this array is directly referenced from the fragment shader with 
    the layout(location = 0) out vec4 outColor directive!
    More Attachment options
        pInputAttachments: Attachments that are read from a shader
        pResolveAttachments: Attachments used for multisampling color attachments
        pDepthStencilAttachment: Attachment for depth and stencil data
        pPreserveAttachments: Attachments that are not used by this subpass, but for which the data must be preserved*/
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;  //a subpass can use a single depth(+stencil) attachment unlike colorAttachment

    /*render pass object can be created by filling in the VkRenderPassCreateInfo structure with an array of attachments 
    and subpasses. The VkAttachmentReference objects reference attachments using the indices of this array*/
    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    /*render pass automatically take care of image layout transitions, e.g VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    transitions are controlled by subpass dependencies, which specify memory and execution dependencies between subpasses*/
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;    //VK_SUBPASS_EXTERNAL refers to the implicit subpass before or after the render pass depending on whether it is specified in srcSubpass or dstSubpass
    dependency.dstSubpass = 0;  //refers to subpass index
    //next two fields specify the operations to wait on and the stages in which these operations occur
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;    //VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}; in drawFrame()
    dependency.srcAccessMask = 0;
    //The operations that should wait on this are in the color attachment stage and involve the writing of the color attachment.
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(vlkn_device, &renderPassInfo, nullptr, &vlkn_render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}   
