#pragma once

#include <memory>
#include <vector>

#include "lve_device.hpp"
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

namespace lve {

class FirstApp {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 800;

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp&) = delete;
  FirstApp& operator=(const FirstApp&) = delete;

  void run();

 private:
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

  void sierpinski(std::vector<LveModel::Vertex>& vertices, int depth,
                  glm::vec2 left, glm::vec2 right, glm::vec2 top);

  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  LveDevice lveDevice{lveWindow};
  LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout{};
  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<LveModel> lveModel;
};

}  // namespace lve