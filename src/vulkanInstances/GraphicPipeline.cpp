#include "GraphicPipeline.hpp"

LiveWindow::GraphicPipeline::GraphicPipeline(const std::string &vert_file_path, const std::string &frag_file_path)
{
    createGraphicPipeline(vert_file_path, frag_file_path);
}

std::vector<char> LiveWindow::GraphicPipeline::readFile(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::ate | std::ios::binary);    //ate -> go to end imediatly, binary -> read as binary
    if(!file.is_open())
    {
        throw std::runtime_error("failed to open file: " + file_path);
    }
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

void LiveWindow::GraphicPipeline::createGraphicPipeline(const std::string &vert_file_path, const std::string &frag_file_path)
{
    std::vector<char> vertCode = readFile(vert_file_path);
    std::vector<char> fragCode = readFile(frag_file_path);

    std::cout << "Vertex shader code size: " << vertCode.size() << std::endl;
    std::cout << "Fragement shader code size: " << fragCode.size() << std::endl;
}
