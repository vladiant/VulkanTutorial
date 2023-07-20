#pragma once

#include <string>
#include <vector>

#include "lve_device.hpp"

namespace lve {

struct PipelineConfigInfo {};

class LvePipeline {
 public:
  LvePipeline(LveDevice& device, const std::string& vertFilepath,
              const std::string& fragFilepath,
              const PipelineConfigInfo& configInfo);

  ~LvePipeline() = default;

  LvePipeline(const LvePipeline&) = delete;
  LvePipeline& operator=(const LvePipeline&) = delete;

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);

 private:
  static std::vector<char> readFile(const std::string& filepath);

  void createGraphicsPipeline(const std::string& vertFilepath,
                              const std::string& fragFilepath,
                              const PipelineConfigInfo& configInfo);

  void createShaderModule(const std::vector<char>& code,
                          VkShaderModule* shaderModule);

  LveDevice& lveDevice;
  VkPipeline graphicsPipeline{};
  VkShaderModule vertShaderModule{};
  VkShaderModule fragShaderModule{};
};

}  // namespace lve
