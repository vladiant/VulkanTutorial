#pragma once

#include <string>
#include <vector>

#include "lve_device.hpp"

namespace lve {

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class LvePipeline {
 public:
  LvePipeline(LveDevice& device, const std::string& vertFilepath,
              const std::string& fragFilepath,
              const PipelineConfigInfo& configInfo);

  ~LvePipeline();

  LvePipeline(const LvePipeline&) = delete;
  LvePipeline& operator=(const LvePipeline&) = delete;

  static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo,
                                        uint32_t width, uint32_t height);

  void bind(VkCommandBuffer commandBuffer);

 private:
  static std::vector<char> readFile(const std::string& filepath);

  void createGraphicsPipeline(const std::string& vertFilepath,
                              const std::string& fragFilepath,
                              const PipelineConfigInfo& configInfo);

  std::vector<uint32_t> compileVertexShader(
      const std::vector<char>& shaderCodeVertex);

  std::vector<uint32_t> compileFragmentShader(
      const std::vector<char>& shaderCodeVertex);

  void createShaderModule(const std::vector<uint32_t>& code,
                          VkShaderModule* shaderModule);

  LveDevice& lveDevice;
  VkPipeline graphicsPipeline{};
  VkShaderModule vertShaderModule{};
  VkShaderModule fragShaderModule{};
};

}  // namespace lve
