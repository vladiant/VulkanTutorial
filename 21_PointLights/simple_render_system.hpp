#pragma once

#include <memory>
#include <vector>

#include "lve_camera.hpp"
#include "lve_device.hpp"
#include "lve_frame_info.hpp"
#include "lve_game_object.hpp"
#include "lve_pipeline.hpp"

namespace lve {

class SimpleRenderSystem {
 public:
  SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass,
                     VkDescriptorSetLayout globalSetLayout);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem&) = delete;
  SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

  void renderGameObjects(FrameInfo& frameInfo,
                         std::vector<LveGameObject>& gameObjects);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  LveDevice& lveDevice;
  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout{};
};

}  // namespace lve