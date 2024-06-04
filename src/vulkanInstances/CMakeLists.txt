add_library(VulkanPipeline OBJECT
    GraphicPipeline.cpp
)
add_library(VulkanRenderer OBJECT
    VulkanRenderer.cpp
)
add_library(VlknInstance OBJECT
    VlknInstance.cpp
)
add_library(VlknPhysicalDevice OBJECT
    VlknPhysicalDevice.cpp
)
add_library(VlknLogicalDevice OBJECT
    VlknLogicalDevice.cpp
)
add_library(VlknWindowSurface OBJECT
    VlknWindowSurface.cpp
)
add_library(VlknSwapChain OBJECT
    VlknSwapChain.cpp
)
add_library(VlknImageViews OBJECT
    VlknImageViews.cpp
)
add_library(VlknGraphicsPipeline OBJECT
    VlknGraphicsPipeline.cpp
)
add_library(VlknRenderPass OBJECT
    VlknRenderPass.cpp
)
add_library(VlknFrameBuffer OBJECT
    VlknFrameBuffer.cpp
)
add_library(VlknCommandPool OBJECT
    VlknCommandPool.cpp
)
add_library(VlknSyncObjects OBJECT
    VlknSyncObjects.cpp
)
add_library(VlknVertexBuffers OBJECT
    VlknVertexBuffers.cpp
)
add_library(VlknDescriptorSetLayout OBJECT
    VlknDescriptorSetLayout.cpp
)
add_library(VlknTextureImage OBJECT
    VlknTextureImage.cpp
)
add_library(VlknDepthBuffer OBJECT
    VlknDepthBuffer.cpp
)
add_library(VlknModelLoader OBJECT
    VlknModelLoader.cpp
)
add_library(VertexInfo OBJECT
    VertexInfo.cpp
)
add_library(VlknData OBJECT
    VlknData.cpp
)

target_link_libraries(VlknData Vulkan::Vulkan glfw3 VertexInfo)

target_link_libraries(Vulkan_Renderer
    VlknData 
    VulkanPipeline
    VulkanRenderer 
    VlknInstance 
    VlknPhysicalDevice 
    VlknLogicalDevice 
    VlknWindowSurface 
    VlknSwapChain 
    VlknImageViews 
    VlknGraphicsPipeline 
    VlknRenderPass 
    VlknFrameBuffer
    VlknCommandPool 
    VlknSyncObjects 
    VlknVertexBuffers
    VlknDescriptorSetLayout
    VlknTextureImage
    VlknDepthBuffer
    VlknModelLoader
    VertexInfo
)
