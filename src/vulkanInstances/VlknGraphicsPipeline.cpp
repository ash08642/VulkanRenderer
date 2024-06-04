#include "VlknGraphicsPipeline.hpp"

void VulkanRenderer::createGraphicsPipeline(VkPipeline& vlkn_graphics_pipeline, VkPipelineLayout& vlkn_pipeline_layout, const VkRenderPass& vlkn_render_pass, const VkDevice& vlkn_device, const VkExtent2D& vlkn_swap_chain_extent, const VkDescriptorSetLayout& vlkn_descriptor_set_layout)
{
    auto vertShaderCode = readFile("src/shaders/simpleShader.vert.spv");
    auto fragShaderCode = readFile("src/shaders/simpleShader.frag.spv");

    //Shader modules are just a thin wrapper around the shader bytecode
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, vlkn_device);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, vlkn_device);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; // tell Vulkan in which pipeline stage the shader is going to be used
    vertShaderStageInfo.module = vertShaderModule;  // shader module containing the code
    vertShaderStageInfo.pName = "main"; // function to invoke
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};    // array that contains these two shadersCreatInfos

    VkVertexInputBindingDescription vlknVertexInputBindingDescription = VlknVertex::getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 3> vlknVertexInputAttributeDescription = VlknVertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; // describes the format of the vertex data that will be passed to the vertex shader
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &vlknVertexInputBindingDescription; // Optional only when vertices are hardcoded in shader)
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vlknVertexInputAttributeDescription.size());
    vertexInputInfo.pVertexAttributeDescriptions = vlknVertexInputAttributeDescription.data();
    /*
    - pVertexBindingDescriptions and pVertexAttributeDescriptions members point to an array of structs that describe the 
      aforementioned details for loading vertex data
    - Bindings: spacing between data and whether the data is per-vertex or per-instance (see instancing)
    - Attribute descriptions: type of the attributes passed to the vertex shader, which binding to load them from and at
      which offset
    */

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};  //what kind of geometry will be drawn from the vertices and if primitive restart should be enabled
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;   // 'VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST': triangle from every 3 vertices without reuse
    inputAssembly.primitiveRestartEnable = VK_FALSE;    //VK_TRUE, then it's possible to break up lines and triangles in the _STRIP topology modes

    VkViewport viewport{};  //describes the region of the framebuffer that the output will be rendered to
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) vlkn_swap_chain_extent.width;
    viewport.height = (float) vlkn_swap_chain_extent.height;
    viewport.minDepth = 0.0f;   //specify the range of depth values to use for the framebuffer
    viewport.maxDepth = 1.0f;   //specify the range of depth values to use for the framebuffer

    VkRect2D scissor{}; //Any pixels outside the scissor rectangles will be discarded by the rasterizer
    scissor.offset = {0, 0};
    scissor.extent = vlkn_swap_chain_extent;   //draw to the entire framebuffer, we would specify a scissor rectangle that covers it entirely

    std::vector<VkDynamicState> dynamicStates = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    /* The rasterizer takes the geometry that is shaped by the vertices from the vertex shader and turns it into fragments to
    be colored by the fragment shader. It also performs depth testing, face culling and the scissor test, and it can be 
    configured to output fragments that fill entire polygons or just the edges (wireframe rendering).*/
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    /* If depthClampEnable is set to VK_TRUE, then fragments that are beyond the near and far planes are clamped to them as 
    opposed to discarding them. This is useful in some special cases like shadow maps. Using this requires enabling a GPU feature.*/
    rasterizer.depthClampEnable = VK_FALSE;
     /* If rasterizerDiscardEnable is set to VK_TRUE, then geometry never passes through the rasterizer stage.
    This basically disables any output to the framebuffer.*/
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    /*The polygonMode determines how fragments are generated for geometry. The following modes are available:
        VK_POLYGON_MODE_FILL: fill the area of the polygon with fragments
        VK_POLYGON_MODE_LINE: polygon edges are drawn as lines
        VK_POLYGON_MODE_POINT: polygon vertices are drawn as points
    Using any mode other than fill requires enabling a GPU feature.*/
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    /*The lineWidth member is straightforward, it describes the thickness of lines in terms of number of fragments. 
    The maximum line width that is supported depends on the hardware and any line thicker than 1.0f requires you 
    to enable the wideLines GPU feature.*/
    rasterizer.lineWidth = 1.0f;
    /*The cullMode variable determines the type of face culling to use. You can disable culling, cull the front faces, 
    cull the back faces or both. The frontFace variable specifies the vertex order for faces to be considered front-facing 
    and can be clockwise or counterclockwise.*/
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    /*The rasterizer can alter the depth values by adding a constant value or biasing them based on a fragment's slope. This 
    is sometimes used for shadow mapping, but we won't be using it. Just set depthBiasEnable to VK_FALSE.*/
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    /*The VkPipelineMultisampleStateCreateInfo struct configures multisampling, which is one of the ways to perform anti-aliasing. 
    It works by combining the fragment shader results of multiple polygons that rasterize to the same pixel. This mainly occurs 
    along edges, which is also where the most noticeable aliasing artifacts occur. Because it doesn't need to run the fragment 
    shader multiple times if only one polygon maps to a pixel, it is significantly less expensive than simply rendering to a 
    higher resolution and then downscaling. Enabling it requires enabling a GPU feature.
    Complete this later to implement multisampling*/
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    /*After a fragment shader has returned a color, it needs to be combined with the color that is already in the framebuffer. 
    This transformation is known as color blending and there are two ways to do it:
        Mix the old and new value to produce a final color
        Combine the old and new value using a bitwise operation*/
    VkPipelineColorBlendAttachmentState colorBlendAttachment{}; //contains the configuration per attached framebuffer
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;    //blendEnable is set to VK_FALSE, then the new color from the fragment shader is passed through unmodified
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    /*  colorBlendAttachment.blendEnable = VK_TRUE; //implementation when .blendEnable is TRUE
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    */

   VkPipelineColorBlendStateCreateInfo colorBlending{}; //contains the global color blending settings.
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE; //VK_TRUE if implemented .blendEnable = VK_TRUE in VkPipelineColorBlendAttachmentState
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE; // specifies if the depth of new fragments should be compared to the depth buffer to see if they should be discarded
    depthStencil.depthWriteEnable = VK_TRUE;    //specifies if the new depth of fragments that pass the depth test should actually be written to the depth buffer
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;   //specifies the comparison that is performed to keep or discard fragments.
    
    /*The depthBoundsTestEnable, minDepthBounds and maxDepthBounds fields are used for the optional depth bound test. Basically, this 
    allows you to only keep fragments that fall within the specified depth range.*/
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional

    /* configure stencil buffer operations, not being used currently*/
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional

    /*uniform values need to be specified during pipeline creation by creating a VkPipelineLayout object
        Uniform values in shaders, which are globals similar to dynamic state variables that can be changed at drawing 
        time to alter the behavior of your shaders without having to recreate them*/
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1; // Optional
    pipelineLayoutInfo.pSetLayouts = &vlkn_descriptor_set_layout; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional   // push constants: are another way of passing dynamic values to shaders

    if (vkCreatePipelineLayout(vlkn_device, &pipelineLayoutInfo, nullptr, &vlkn_pipeline_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    //start by referencing the array of VkPipelineShaderStageCreateInfo structs.
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    //Then we reference all of the structures describing the fixed-function stage.
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.pDepthStencilState = &depthStencil;
    // the pipeline layout, which is a Vulkan handle rather than a struct pointer
    pipelineInfo.layout = vlkn_pipeline_layout;
    pipelineInfo.renderPass = vlkn_render_pass; //reference to the render pass
    pipelineInfo.subpass = 0;   // index of the sub pass where this graphics pipeline will be used.
    //Vulkan allows you to create a new graphics pipeline by deriving from an existing pipeline
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional
    
    if (vkCreateGraphicsPipelines(vlkn_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vlkn_graphics_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(vlkn_device, fragShaderModule, nullptr);
    vkDestroyShaderModule(vlkn_device, vertShaderModule, nullptr);
}

std::vector<char> VulkanRenderer::readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    // ate: Start reading at the end of the file
    // binary: Read the file as binary file (avoid text transformations)
    // advantage of starting to read at the end of the file is that we can use the read position to determine the size of the file and allocate a buffer

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    // seek back to the beginning of the file and read all of the bytes at once:
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    std::cout << filename << ": " << fileSize << std::endl;

    file.close();
    return buffer;
}

VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char> &code, const VkDevice& vlkn_device)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(vlkn_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}
