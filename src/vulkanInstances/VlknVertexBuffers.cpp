#include "VlknVertexBuffers.hpp"

void VulkanRenderer::createUniformBuffers(std::vector<VkBuffer>& uniformBuffers, std::vector<VkDeviceMemory>& uniformBuffersMemory, std::vector<void *>& uniformBuffersMapped, const VkDevice &vlkn_device, const VkPhysicalDevice& vlkn_physical_device)
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);  //get a pointer to which we can write the data later on

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
    {
        createBuffer
        (
            bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,   // Buffer can be used as source in a memory transfer operation.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[i],
            uniformBuffersMemory[i],
            vlkn_device,
            vlkn_physical_device
        );
        vkMapMemory(vlkn_device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void VulkanRenderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &vlkn_vertex_buffer, VkDeviceMemory &vlkn_vertex_buffer_memory, const VkDevice &vlkn_device, const VkPhysicalDevice &vlkn_physical_device)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;    //size, which specifies the size of the buffer in bytes
    bufferInfo.usage = usage;   //indicates for which purposes the data in the buffer is going to be used
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //buffers can also be owned by a specific queue family or be shared between multiple at the same time.

    if (vkCreateBuffer(vlkn_device, &bufferInfo, nullptr, &vlkn_vertex_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    //Assign Memory to buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vlkn_device, vlkn_vertex_buffer, &memRequirements);
    /* VkMemoryRequirements struct has three fields:
        size: The size of the required amount of memory in bytes, may differ from bufferInfo.size.
        alignment: The offset in bytes where the buffer begins in the allocated region of memory, depends on bufferInfo.usage and bufferInfo.flags.
        memoryTypeBits: Bit field of the memory types that are suitable for the buffer.*/

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, vlkn_physical_device);

    if (vkAllocateMemory(vlkn_device, &allocInfo, nullptr, &vlkn_vertex_buffer_memory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }
    vkBindBufferMemory(vlkn_device, vlkn_vertex_buffer, vlkn_vertex_buffer_memory, 0);    //If memory allocation was successful,then associate this memory with the buffer 
    //fourth parameter is the offset within the region of memory
    //If the offset is non-zero, then it is required to be divisible by memRequirements.alignment
}

void VulkanRenderer::copyBuffer(VkBuffer &srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size, const VkDevice &vlkn_device, const VkCommandPool &vlkn_command_pool, const VkQueue& vlkn_graphics_queue)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(vlkn_device, vlkn_command_pool);
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);   //Contents of buffers are transferred using the vkCmdCopyBuffer command
    endSingleTimeCommands(commandBuffer, vlkn_command_pool, vlkn_graphics_queue, vlkn_device);
}

uint32_t VulkanRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& vlkn_physical_device)
{
    /* VkPhysicalDeviceMemoryProperties structure has two arrays memoryTypes and memoryHeaps. Memory heaps are distinct memory resources 
    like dedicated VRAM and swap space in RAM for when VRAM runs out. The different types of memory exist within these heaps*/
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vlkn_physical_device, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    std::cout << "failed to fine suitable memory type" << std::endl;
    throw std::runtime_error("failed to find suitable memory type");
}
