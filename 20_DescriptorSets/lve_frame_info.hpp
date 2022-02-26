#pragma once

#include <vulkan/vulkan.h>

#include "lve_camera.hpp"

namespace lve {

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LveCamera& camera;
  VkDescriptorSet globalDescriptorSet;
};

}  // namespace lve
