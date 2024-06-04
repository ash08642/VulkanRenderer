#pragma once

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void createSyncObjects
    (
        std::vector<VkSemaphore>& vlkn_image_available_semaphores,
        std::vector<VkSemaphore>& vlkn_render_finished_semaphores,
        std::vector<VkFence>& vlkn_in_flight_fences,
        const VkDevice& vlkn_device
    );
}

/*
VkSemaphore
A semaphore is used to add order between queue operations. Queue operations refer to the work we submit to a queue, 
either in a command buffer or from within a function. Examples of queues are the graphics queue 
and the presentation queue. Semaphores are used both to order work inside the same queue and between different queues

VkFence
A fence has a similar purpose, in that it is used to synchronize execution, but it is for ordering the execution on the CPU, 
otherwise known as the host. Simply put, if the host needs to know when the GPU has finished something, we use a fence.
*/