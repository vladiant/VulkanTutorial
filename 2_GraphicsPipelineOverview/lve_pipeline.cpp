#include "lve_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {

LvePipeline::LvePipeline(const std::string& vertFilepath,
                         const std::string& fragFilepath) {
  createGraphicsPipeline(vertFilepath, fragFilepath);
}

std::vector<char> LvePipeline::readFile(const std::string& filepath) {
  std::ifstream file{filepath, std::ios::binary};

  if (!file) {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  std::vector<char> result{std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>()};
  return result;
}

void LvePipeline::createGraphicsPipeline(const std::string& vertFilepath,
                                         const std::string& fragFilepath) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}

}  // namespace lve