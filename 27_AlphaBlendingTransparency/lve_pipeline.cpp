#include "lve_pipeline.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "lve_model.hpp"
#include "util.hpp"

namespace lve {

LvePipeline::LvePipeline(LveDevice& device, const std::string& vertFilepath,
                         const std::string& fragFilepath,
                         const PipelineConfigInfo& configInfo)
    : lveDevice{device} {
  SpirvHelper::Init();
  createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

LvePipeline::~LvePipeline() {
  vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
  vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);
  vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
  SpirvHelper::Finalize();
}

void LvePipeline::bind(VkCommandBuffer commandBuffer) {
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline);
}

void LvePipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo) {
  configInfo.inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  configInfo.viewPortInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  configInfo.viewPortInfo.viewportCount = 1;
  configInfo.viewPortInfo.pViewports = nullptr;
  configInfo.viewPortInfo.scissorCount = 1;
  configInfo.viewPortInfo.pScissors = nullptr;

  configInfo.rasterizationInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
  configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  configInfo.rasterizationInfo.lineWidth = 1.0f;
  configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
  configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
  configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
  configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
  configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
  configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

  configInfo.multisampleInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
  configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
  configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
  configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
  configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

  configInfo.colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
  configInfo.colorBlendAttachment.srcColorBlendFactor =
      VK_BLEND_FACTOR_ONE;  // Optional
  configInfo.colorBlendAttachment.dstColorBlendFactor =
      VK_BLEND_FACTOR_ZERO;                                        // Optional
  configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;  // Optional
  configInfo.colorBlendAttachment.srcAlphaBlendFactor =
      VK_BLEND_FACTOR_ONE;  // Optional
  configInfo.colorBlendAttachment.dstAlphaBlendFactor =
      VK_BLEND_FACTOR_ZERO;                                        // Optional
  configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;  // Optional

  configInfo.colorBlendInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
  configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
  configInfo.colorBlendInfo.attachmentCount = 1;
  configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
  configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
  configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
  configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
  configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

  configInfo.depthStencilInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
  configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
  configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
  configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.front = {};  // Optional
  configInfo.depthStencilInfo.back = {};   // Optional

  configInfo.dynamicStatesEnables = {VK_DYNAMIC_STATE_VIEWPORT,
                                     VK_DYNAMIC_STATE_SCISSOR};
  configInfo.dynamicStateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  configInfo.dynamicStateInfo.pDynamicStates =
      configInfo.dynamicStatesEnables.data();
  configInfo.dynamicStateInfo.dynamicStateCount =
      static_cast<uint32_t>(configInfo.dynamicStatesEnables.size());
  configInfo.dynamicStateInfo.flags = 0;

  configInfo.bindingDesriptions = LveModel::Vertex::getBindingDescription();
  configInfo.attributeDesriptions = LveModel::Vertex::getAttributeDescription();
}

void LvePipeline::enableAlphaBlending(PipelineConfigInfo& configInfo) {
  configInfo.colorBlendAttachment.blendEnable = VK_TRUE;

  configInfo.colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  configInfo.colorBlendAttachment.srcColorBlendFactor =
      VK_BLEND_FACTOR_SRC_ALPHA;
  configInfo.colorBlendAttachment.dstColorBlendFactor =
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
}

std::vector<char> LvePipeline::readFile(const std::string& filepath) {
  std::ifstream file{filepath, std::ios::binary};

  if (!file) {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  std::vector<char> result{std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>()};

  result.push_back('\0');
  return result;
}

void LvePipeline::createGraphicsPipeline(const std::string& vertFilepath,
                                         const std::string& fragFilepath,
                                         const PipelineConfigInfo& configInfo) {
  assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
         "Cannot create graphics pipeline: no pipelineLayout provided in "
         "configInfo");
  assert(
      configInfo.renderPass != VK_NULL_HANDLE &&
      "Cannot create graphics pipeline: no renderPass provided in configInfo");
  auto vertSource = readFile(vertFilepath);
  auto fragSource = readFile(fragFilepath);

  const auto vertCode = compileVertexShader(vertSource);
  const auto fragCode = compileFragmentShader(fragSource);

  createShaderModule(vertCode, &vertShaderModule);
  createShaderModule(fragCode, &fragShaderModule);

  VkPipelineShaderStageCreateInfo shaderStages[2];
  shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  shaderStages[0].module = vertShaderModule;
  shaderStages[0].pName = "main";
  shaderStages[0].flags = 0;
  shaderStages[0].pNext = nullptr;
  shaderStages[0].pSpecializationInfo = nullptr;

  shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  shaderStages[1].module = fragShaderModule;
  shaderStages[1].pName = "main";
  shaderStages[1].flags = 0;
  shaderStages[1].pNext = nullptr;
  shaderStages[1].pSpecializationInfo = nullptr;

  auto& bindingDescriptions = configInfo.bindingDesriptions;
  auto& attributeDescriptions = configInfo.attributeDesriptions;
  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.vertexBindingDescriptionCount =
      static_cast<uint32_t>(bindingDescriptions.size());
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
  vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

  VkGraphicsPipelineCreateInfo pipleineInfo{};
  pipleineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipleineInfo.stageCount = 2;
  pipleineInfo.pStages = shaderStages;
  pipleineInfo.pVertexInputState = &vertexInputInfo;
  pipleineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
  pipleineInfo.pViewportState = &configInfo.viewPortInfo;
  pipleineInfo.pRasterizationState = &configInfo.rasterizationInfo;
  pipleineInfo.pMultisampleState = &configInfo.multisampleInfo;
  pipleineInfo.pColorBlendState = &configInfo.colorBlendInfo;
  pipleineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
  pipleineInfo.pDynamicState = &configInfo.dynamicStateInfo;

  pipleineInfo.layout = configInfo.pipelineLayout;
  pipleineInfo.renderPass = configInfo.renderPass;
  pipleineInfo.subpass = configInfo.subpass;

  pipleineInfo.basePipelineIndex = -1;
  pipleineInfo.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1,
                                &pipleineInfo, nullptr,
                                &graphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics pipeline");
  }
}

void LvePipeline::createShaderModule(const std::vector<uint32_t>& code,
                                     VkShaderModule* shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size() * 4;
  createInfo.pCode = code.data();
  createInfo.pNext = nullptr;
  createInfo.flags = 0;

  if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module");
  }
}

std::vector<uint32_t> LvePipeline::compileVertexShader(
    const std::vector<char>& shaderCodeVertex) {
  const vk::ShaderStageFlagBits stage = vk::ShaderStageFlagBits::eVertex;
  std::vector<uint32_t> shaderCodeSpirV;
  const bool success =
      SpirvHelper::GLSLtoSPV(stage, shaderCodeVertex.data(), shaderCodeSpirV);

  if (!success) {
    throw std::runtime_error("failed to compile vertex shader");
  }

  return shaderCodeSpirV;
}

std::vector<uint32_t> LvePipeline::compileFragmentShader(
    const std::vector<char>& shaderCodeVertex) {
  const vk::ShaderStageFlagBits stage = vk::ShaderStageFlagBits::eFragment;
  std::vector<uint32_t> shaderCodeSpirV;
  bool success =
      SpirvHelper::GLSLtoSPV(stage, shaderCodeVertex.data(), shaderCodeSpirV);

  if (!success) {
    throw std::runtime_error("failed to compile fragment shader");
  }

  return shaderCodeSpirV;
}

}  // namespace lve
