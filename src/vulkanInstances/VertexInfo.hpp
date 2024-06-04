#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan.h>

#include <array>
#include <vector>

namespace VulkanRenderer
{
    struct VlknVertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        //tell Vulkan how to pass this data format to the vertex shader once it's been uploaded into GPU memory
        static VkVertexInputBindingDescription getBindingDescription();
        //describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
    
        bool operator==(const VlknVertex& other) const {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };

    struct UniformBufferObject  //name should match in vertexShader
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    //data in the matrices is binary compatible with the way the shader expects it, so we can later just memcpy a UniformBufferObject to a VkBuffer
}

namespace std {
    template<> struct hash<VulkanRenderer::VlknVertex> {
        size_t operator()(VulkanRenderer::VlknVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}
