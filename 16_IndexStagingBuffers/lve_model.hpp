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

  struct Data {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
  };

  LveModel(LveDevice& lveDevice, const Data& data);
  ~LveModel();

  LveModel(const LveModel&) = delete;
  LveModel& operator=(const LveModel&) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

 private:
  void createVertexBuffers(const std::vector<Vertex>& vertices);
  void createIndexBuffers(const std::vector<uint32_t>& indices);

  LveDevice& lveDevice;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;

  bool hasIndexBuffer{false};
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  uint32_t indexCount;
};

}  // namespace lve
