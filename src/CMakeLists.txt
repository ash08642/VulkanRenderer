add_library(Application OBJECT
    Application.cpp
)

target_link_libraries(Application
    VulkanPipeline Window VulkanRenderer)
