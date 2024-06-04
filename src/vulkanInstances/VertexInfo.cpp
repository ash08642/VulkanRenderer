#include "VertexInfo.hpp"

VkVertexInputBindingDescription VulkanRenderer::VlknVertex::getBindingDescription()
{
    /* A vertex binding describes at which rate to load data from memory throughout the vertices. It specifies the number of bytes 
    between data entries and whether to move to the next data entry after each vertex or after each instance.*/
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0; // specifies the index of the binding in the array of bindings
    bindingDescription.stride = sizeof(VlknVertex); // specifies the number of bytes from one entry to the next
    /*  VK_VERTEX_INPUT_RATE_VERTEX: Move to the next data entry after each vertex
        VK_VERTEX_INPUT_RATE_INSTANCE: Move to the next data entry after each instance*/
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> VulkanRenderer::VlknVertex::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;   //binding parameter tells Vulkan from which binding the per-vertex data comes
    attributeDescriptions[0].location = 0;  //location parameter references the location directive of the input in the vertex shader
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;  //format parameter describes the type of data for the attribute
    attributeDescriptions[0].offset = offsetof(VlknVertex, pos);    //offset parameter specifies the number of bytes since the start of the per-vertex data to read from

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VlknVertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(VlknVertex, texCoord);

    return attributeDescriptions;
}
