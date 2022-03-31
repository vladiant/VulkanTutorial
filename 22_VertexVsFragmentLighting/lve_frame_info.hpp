#pragma once

#include <vulkan/vulkan.h>

#include "lve_camera.hpp"
#include "lve_game_object.hpp"

namespace lve {

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LveCamera& camera;
  VkDescriptorSet globalDescriptorSet;
  LveGameObject::Map& gameObjects;
};

}  // namespace lve
