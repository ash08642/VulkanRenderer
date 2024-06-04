#pragma once

#include "VlknData.hpp"
#include "VlknCommandPool.hpp"

namespace VulkanRenderer
{
    enum BufferType
    {
        IxVertexBuffer = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        IxIndexBuffer = VK_BUFFER_USAGE_INDEX_BUFFER_BIT
    };

    template <typename T>
    void createVertexBuffer //implementation below, not in .cpp
    (
        VkBuffer& vlkn_vertex_buffer, 
        VkDeviceMemory& vlkn_vertex_buffer_memory,
        BufferType buffer_type,
        T customVec,
        const VkDevice& vlkn_device, 
        const VkPhysicalDevice& vlkn_physical_device, 
        const VkCommandPool& vlkn_command_pool,
        const VkQueue& vlkn_graphics_queue
    );
    void createUniformBuffers
    (
        std::vector<VkBuffer>& uniformBuffers, 
        std::vector<VkDeviceMemory>& uniformBuffersMemory,
        std::vector<void*>& uniformBuffersMapped,
        const VkDevice& vlkn_device,
        const VkPhysicalDevice& vlkn_physical_device
    );
    void createBuffer
    (
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& vlkn_vertex_buffer,
        VkDeviceMemory& vlkn_vertex_buffer_memory,
        const VkDevice& vlkn_device, 
        const VkPhysicalDevice& vlkn_physical_device
    );
    void copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size, const VkDevice& vlkn_device, const VkCommandPool& vlkn_command_pool, const VkQueue& vlkn_graphics_queue);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& vlkn_physical_device);
    
    template <typename T>
    void createVertexBuffer(VkBuffer &vlkn_vertex_buffer, VkDeviceMemory& vlkn_vertex_buffer_memory, BufferType buffer_type, T customVec, const VkDevice &vlkn_device, const VkPhysicalDevice& vlkn_physical_device, const VkCommandPool& vlkn_command_pool, const VkQueue& vlkn_graphics_queue)
    {
        VkDeviceSize bufferSize = sizeof(customVec[0]) * customVec.size();    //size, which specifies the size of the buffer in bytes
        
        VkBuffer stagingBuffer; //in CPU accessible memory to upload the data from the vertex array to
        VkDeviceMemory stagingBufferMemory;
        createBuffer
        (
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,   // Buffer can be used as source in a memory transfer operation.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory,
            vlkn_device,
            vlkn_physical_device
        );
        //Fill the vertex buffer
        void* data;
        vkMapMemory(vlkn_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, customVec.data(), (size_t) bufferSize);
        vkUnmapMemory(vlkn_device, stagingBufferMemory);

        createBuffer
        (
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | buffer_type,   //Buffer can be used as destination in a memory transfer operation.
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vlkn_vertex_buffer, //vertex buffer in device local memory
            vlkn_vertex_buffer_memory,
            vlkn_device,
            vlkn_physical_device
        );

        copyBuffer(stagingBuffer, vlkn_vertex_buffer, bufferSize, vlkn_device, vlkn_command_pool, vlkn_graphics_queue);

        vkDestroyBuffer(vlkn_device, stagingBuffer, nullptr);
        vkFreeMemory(vlkn_device, stagingBufferMemory, nullptr);
    }
}
/*
Buffers in Vulkan are regions of memory used for storing arbitrary data that can be read by the graphics card.
They can be used to store vertex data,
*/

/*Graphics cards can offer different types of memory to allocate from. Each type of memory varies in terms of allowed 
operations and performance characteristics. We need to combine the requirements of the buffer and our own application 
requirements to find the right type of memory to use.
Hence findMemoryType(...)*/