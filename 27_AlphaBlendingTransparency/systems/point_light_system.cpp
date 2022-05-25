#include "point_light_system.hpp"

#include <array>
#include <iostream>
#include <stdexcept>
#include <map>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

struct PointLightPushConstants {
  glm::vec4 position{};
  glm::vec4 color{};
  float radius;
};

PointLightSystem::PointLightSystem(LveDevice& device, VkRenderPass renderPass,
                                   VkDescriptorSetLayout globalSetLayout)
    : lveDevice{device} {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem() {
  vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

void PointLightSystem::createPipelineLayout(
    VkDescriptorSetLayout globalSetLayout) {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(PointLightPushConstants);

  std::vector<VkDescriptorSetLayout> descriptorSetlayouts{globalSetLayout};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount =
      static_cast<uint32_t>(descriptorSetlayouts.size());
  pipelineLayoutInfo.pSetLayouts = descriptorSetlayouts.data();
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

  if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout");
  }
}

void PointLightSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
  LvePipeline::enableAlphaBlending(pipelineConfig);
  pipelineConfig.attributeDesriptions.clear();
  pipelineConfig.bindingDesriptions.clear();
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  lvePipeline =
      std::make_unique<LvePipeline>(lveDevice, "shaders/point_light.vert",
                                    "shaders/point_light.frag", pipelineConfig);
}

void PointLightSystem::render(FrameInfo& frameInfo) {
  // Sort lights
  std::multimap<float, LveGameObject::id_t> sorted;
  for (const auto& kv : frameInfo.gameObjects) {
    const auto& obj = kv.second;
    if(obj.pointLight == nullptr) {
      continue;
    }

    // Calculate distance
    const auto offset = frameInfo.camera.getPosition() - obj.transform.translation;
    const float disSquared = glm::dot(offset, offset);
    sorted.insert(std::pair(disSquared, obj.getId()));
  }

  lvePipeline->bind(frameInfo.commandBuffer);

  vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                          &frameInfo.globalDescriptorSet, 0, nullptr);

  // Iterate through sorted lights in inverse order
  for (auto it = sorted.crbegin(); it != sorted.crend(); ++it) {
    // Use game obj id to find light object
    const auto& obj = frameInfo.gameObjects.at(it->second);

    PointLightPushConstants push{};
    push.position = glm::vec4(obj.transform.translation, 1.0f);
    push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
    push.radius = obj.transform.scale.x;

    vkCmdPushConstants(
        frameInfo.commandBuffer, pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
        sizeof(PointLightPushConstants), &push);
    vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
  }
}

void PointLightSystem::update(FrameInfo& frameInfo, GlobalUbo& ubo) {
  auto rotateLight = glm::rotate(
    glm::mat4(1.0f), frameInfo.frameTime,
    {0.0f, -1.0f, 0.0f});
  int lightIndex = 0;
  for (auto& kv : frameInfo.gameObjects) {
    auto& obj = kv.second;
    if (obj.pointLight == nullptr) {
      continue;
    }

    assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

    // Update light position
    obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.0f));

    // Copy light to ubo
    ubo.pointLights[lightIndex].position =
        glm::vec4(obj.transform.translation, 1.0f);
    ubo.pointLights[lightIndex].color =
        glm::vec4(obj.color, obj.pointLight->lightIntensity);

    lightIndex += 1;
  }

  ubo.numLights = lightIndex;
}

}  // namespace lve
