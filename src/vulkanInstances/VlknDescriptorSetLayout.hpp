#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createDescriptorSetLayout(VkDescriptorSetLayout& vlkn_descriptor_set_layout, const VkDevice& vlkn_device);
    void createDescriptorPool(VkDescriptorPool& vlkn_descriptor_pool, const VkDevice& vlkn_device);
    void createDescriptorSets(
        std::vector<VkDescriptorSet>& vlkn_descriptor_sets, 
        const VkDescriptorSetLayout& vlkn_descriptor_set_layout, 
        const VkDescriptorPool& vlkn_descriptor_pool, 
        const std::vector<VkBuffer>& uniformBuffers, 
        const VkDevice& vlkn_device,
        const VkImageView& vlkn_texture_image_view,
        const VkSampler& vlkn_texture_sampler
    );
}

/*
The descriptor layout specifies the types of resources that are going to be accessed by the pipeline

A descriptor set specifies the actual buffer or image resources that will be bound to the descriptors
Descriptor sets can't be created directly, they must be allocated from a pool*/