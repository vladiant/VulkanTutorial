#pragma once

#include <string>
#include <vector>

#include "lve_device.hpp"

namespace lve {

struct PipelineConfigInfo {
  PipelineConfigInfo(const PipelineConfigInfo&) = delete;
  PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

  VkPipelineViewportStateCreateInfo viewPortInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStatesEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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

  void bind(VkCommandBuffer commandBuffer);

  static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

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
