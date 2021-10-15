#include "lve_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {

LvePipeline::LvePipeline(LveDevice& device, const std::string& vertFilepath,
                         const std::string& fragFilepath,
                         const PipelineConfigInfo& configInfo)
    : lveDevice{device} {
  createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width,
                                                          uint32_t height) {
  PipelineConfigInfo configInfo{};
  return configInfo;
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
                                         const std::string& fragFilepath,
                                         const PipelineConfigInfo& configInfo) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}

void LvePipeline::createShaderModule(const std::vector<char>& code,
                                     VkShaderModule* shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module");
  }
}

}  // namespace lve