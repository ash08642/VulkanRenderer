#pragma once

#include <unordered_map>

#include "VlknData.hpp"

namespace VulkanRenderer
{
    void loadModel(const char* model_path, std::vector<VlknVertex>& vertices, std::vector<uint32_t>& indices);
}