#include "VlknSyncObjects.hpp"

void VulkanRenderer::createSyncObjects
(
    std::vector<VkSemaphore>& vlkn_image_available_semaphores,
    std::vector<VkSemaphore>& vlkn_render_finished_semaphores,
    std::vector<VkFence>& vlkn_in_flight_fences,
    const VkDevice& vlkn_device
)
{
    vlkn_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    vlkn_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    vlkn_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    /* Create the fence in the signaled state, so that the first call to vkWaitForFences() returns immediately 
    since the fence is already signaled*/
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if(
            vkCreateSemaphore(vlkn_device, &semaphoreInfo, nullptr, &vlkn_image_available_semaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vlkn_device, &semaphoreInfo, nullptr, &vlkn_render_finished_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vlkn_device, &fenceInfo, nullptr, &vlkn_in_flight_fences[i]) != VK_SUCCESS
        )
        {
            throw std::runtime_error("failed to create semaphores!");
        }
    }
}
