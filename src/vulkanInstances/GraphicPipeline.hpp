#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

namespace LiveWindow
{
    class GraphicPipeline
    {
    public:
        GraphicPipeline(const std::string& vert_file_path, const std::string& frag_file_path);
    private:
        static std::vector<char> readFile(const std::string& file_path);
        void createGraphicPipeline(const std::string& vert_file_path, const std::string& frag_file_path);
    };
}