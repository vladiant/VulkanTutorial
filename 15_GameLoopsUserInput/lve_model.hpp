#pragma once

#include <vector>

#include "lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve {

class LveDevice;

class LveModel {
 public:
  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription> getBindingDescription();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescription();
  };

  LveModel(LveDevice& lveDevice, const std::vector<Vertex>& vertices);
  ~LveModel();

  LveModel(const LveModel&) = delete;
  LveModel& operator=(const LveModel&) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

 private:
  void createVertexBuffers(const std::vector<Vertex>& vertices);

  LveDevice& lveDevice;
  VkBuffer vertexBuffer{};
  VkDeviceMemory vertexBufferMemory{};
  uint32_t vertexCount{};
};

}  // namespace lve
