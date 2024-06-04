#include "VlknTextureImage.hpp"

void VulkanRenderer::createTextureImage(
    const char* image_path,
    VkImage& vlkn_texture_image,
    VkDeviceMemory& vlkn_texture_image_memory,
    const VkDevice& vlkn_device, 
    const VkPhysicalDevice& vlkn_physical_device,
    const VkFormat& vlkn_swap_chain_image_format,
    const VkCommandPool& vlkn_command_pool,
    const VkQueue& vlkn_graphics_queue
)
{
    int texWidth, texHeight, texChannels;
    //orces the image to be loaded with an alpha channel for 
    //returns pointer to first element of pixel values
    stbi_uc* pixels = stbi_load(image_path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        std::cout << "failed to load image " << image_path << std::endl; 
        throw std::runtime_error("failed to load texture image!");
    }

    // host visible memory so that we can use vkMapMemory and copy the pixels to it.
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(
        imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory,
        vlkn_device,
        vlkn_physical_device
    );

    void* data;
    vkMapMemory(vlkn_device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(vlkn_device, stagingBufferMemory);

    stbi_image_free(pixels);    //clean up original pixel array

    createImage(
        vlkn_texture_image,
        vlkn_texture_image_memory,
        vlkn_device,
        vlkn_physical_device,
        texWidth,
        texHeight,
        vlkn_swap_chain_image_format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

     transitionImageLayout(vlkn_texture_image, vlkn_swap_chain_image_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vlkn_device, vlkn_command_pool, vlkn_graphics_queue);
        copyBufferToImage(stagingBuffer, vlkn_texture_image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), vlkn_device, vlkn_command_pool, vlkn_graphics_queue);
    transitionImageLayout(vlkn_texture_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vlkn_device, vlkn_command_pool, vlkn_graphics_queue);

    vkDestroyBuffer(vlkn_device, stagingBuffer, nullptr);
    vkFreeMemory(vlkn_device, stagingBufferMemory, nullptr);
}

void VulkanRenderer::createImage(
    VkImage &vlkn_texture_image,
    VkDeviceMemory &vlkn_texture_image_memory, 
    const VkDevice &vlkn_device, 
    const VkPhysicalDevice &vlkn_physical_device,
    uint32_t width, 
    uint32_t height, 
    const VkFormat& format,
    VkImageTiling tiling, 
    VkImageUsageFlags usage, 
    VkMemoryPropertyFlags properties
)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D; //tells Vulkan with what kind of coordinate system the texels in the image are going to be addressed
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1; //depth = 1 when .imageType = VK_IMAGE_TYPE_2D
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;

    /*  VK_IMAGE_TILING_LINEAR: Texels are laid out in row-major order like our pixels array
        VK_IMAGE_TILING_OPTIMAL: Texels are laid out in an implementation defined order for optimal access*/
    imageInfo.tiling = tiling;

    /*  VK_IMAGE_LAYOUT_UNDEFINED: Not usable by the GPU and the very first transition will discard the texels.
        VK_IMAGE_LAYOUT_PREINITIALIZED: Not usable by the GPU, but the first transition will preserve the texels.*/
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    /*  VK_IMAGE_USAGE_TRANSFER_DST_BIT: image is going to be used as destination for the buffer copy
        VK_IMAGE_USAGE_TRANSFER_DST_BIT: image will be accessed from the shader to color our mesh*/
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;  //image will be used by one queue family at a time(that supports graphics or transfer)
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;  //related to multisampling
    imageInfo.flags = 0; // Optional

    if (vkCreateImage(vlkn_device, &imageInfo, nullptr, &vlkn_texture_image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    /*same way as allocating memory for a buffer. Use vkGetImageMemoryRequirements instead of vkGetBufferMemoryRequirements, 
      and use vkBindImageMemory instead of vkBindBufferMemory*/
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vlkn_device, vlkn_texture_image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, vlkn_physical_device);

    if (vkAllocateMemory(vlkn_device, &allocInfo, nullptr, &vlkn_texture_image_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(vlkn_device, vlkn_texture_image, vlkn_texture_image_memory, 0);
}

void VulkanRenderer::transitionImageLayout(
    const VkImage& image,
    const VkFormat& format, 
    VkImageLayout oldLayout, 
    VkImageLayout newLayout,
    const VkDevice& vlkn_device,
    const VkCommandPool& vlkn_command_pool,
    const VkQueue& vlkn_graphics_queue
)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(vlkn_device, vlkn_command_pool);

        /* most common ways to perform layout transitions is using an image memory barrier. A pipeline barrier like that is generally 
        used to synchronize access to resources, like ensuring that a write to a buffer completes before reading from it, but it can 
        also be used to transition image layouts and transfer queue family ownership when VK_SHARING_MODE_EXCLUSIVE is used. There 
        is an equivalent buffer memory barrier to do this for buffers.*/
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;

        /*If you are using the barrier to transfer queue family ownership, then these two fields should be the indices of the queue families. 
        They must be set to VK_QUEUE_FAMILY_IGNORED if you don't want to do this (not the default value!).*/
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        /*The image and subresourceRange specify the image that is affected and the specific part of the image. Our image is not an 
        array and does not have mipmapping levels, so only one level and layer are specified.*/
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        barrier.srcAccessMask = 0; // TODO
        barrier.dstAccessMask = 0; // TODO

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage,   //specifies in which pipeline stage the operations occur that should happen before the barrier
            destinationStage,   //specifies the pipeline stage in which operations will wait on the barrier
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

    endSingleTimeCommands(commandBuffer, vlkn_command_pool, vlkn_graphics_queue, vlkn_device);
}

void VulkanRenderer::copyBufferToImage(
    VkBuffer buffer,
    VkImage image,
    uint32_t width,
    uint32_t height,
    const VkDevice& vlkn_device,
    const VkCommandPool& vlkn_command_pool,
    const VkQueue& vlkn_graphics_queue
)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(vlkn_device, vlkn_command_pool);

        VkBufferImageCopy region{};
        region.bufferOffset = 0;    //bufferOffset specifies the byte offset in the buffer at which the pixel values start.
        /*bufferRowLength and bufferImageHeight fields specify how the pixels are laid out in memory. For example, you could
          have some padding bytes between rows of the image. Specifying 0 for both indicates that the pixels are simply
          tightly packed like they are in our case*/ 
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        //The imageSubresource, imageOffset and imageExtent fields indicate to which part of the image we want to copy the pixels.
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );
    endSingleTimeCommands(commandBuffer, vlkn_command_pool, vlkn_graphics_queue, vlkn_device);
}

void VulkanRenderer::createTextureImageView(VkImageView& vlkn_texture_image_view, VkImage& vlkn_texture_image, const VkFormat& vlkn_swap_chain_image_format, const VkDevice& vlkn_device)
{
    vlkn_texture_image_view = createImageView(vlkn_texture_image, vlkn_swap_chain_image_format, vlkn_device, VK_IMAGE_ASPECT_COLOR_BIT);
}

void VulkanRenderer::createTextureSampler(VkSampler& vlkn_texture_sampler, const VkDevice &vlkn_device, const VkPhysicalDeviceProperties& vlkn_physical_device_properties, const VkPhysicalDeviceFeatures& vlkn_physical_device_features)
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    /*magFilter and minFilter fields specify how to interpolate texels that are magnified or minified. 
      Magnification concerns the oversampling problems, and minification concerns undersampling.
      choices are VK_FILTER_NEAREST and VK_FILTER_LINEAR*/
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    /*addressing mode can be specified per axis using the addressMode fields
        VK_SAMPLER_ADDRESS_MODE_REPEAT: Repeat the texture when going beyond the image dimensions.
        VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: Like repeat, but inverts the coordinates to mirror the image when going beyond the dimensions.
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE: Take the color of the edge closest to the coordinate beyond the image dimensions.
        VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: Like clamp to edge, but instead uses the edge opposite to the closest edge.
        VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER: Return a solid color when sampling beyond the dimensions of the image.*/
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    if(vlkn_physical_device_features.samplerAnisotropy == VK_TRUE)
    {
        samplerInfo.anisotropyEnable = VK_TRUE;
        //maxAnisotropy field limits the amount of texel samples that can be used to calculate the final color. A lower value results in better performance, but lower quality results.
        samplerInfo.maxAnisotropy = vlkn_physical_device_properties.limits.maxSamplerAnisotropy;
    }
    else
    {
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
    }

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; //specifies which color is returned when sampling beyond the image with clamp to border addressing mode.
    samplerInfo.unnormalizedCoordinates = VK_FALSE; //pecifies which coordinate system you want to use to address texels in an image.
    /*  VK_TRUE: [0, texWidth) and [0, texHeight) range.
        VK_FALSE: [0, 1) range on all axes.*/

    /*comparison function is enabled, then texels will first be compared to a value, and the result of that comparison is used in filtering operations.
    This is mainly used for percentage-closer filtering on shadow maps. */
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; //apply to minmapping   //another type of filter that can be applied
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(vlkn_device, &samplerInfo, nullptr, &vlkn_texture_sampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}
