#include "first_app.hpp"

#include <array>
#include <chrono>
#include <iostream>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "keyboard_movement_controller.hpp"
#include "lve_buffer.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"

namespace lve {

struct GlobalUbo {
  alignas(16) glm::mat4 projectionView{1.0f};
  alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f,
                                                                  -1.0f});
};

FirstApp::FirstApp() {
  globalPool = LveDescriptorPool::Builder(lveDevice)
                   .setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
                   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                LveSwapChain::MAX_FRAMES_IN_FLIGHT)
                   .build();
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::loadGameObjects() {
  std::shared_ptr<LveModel> lveModel =
      LveModel::createmodelFromFile(lveDevice, "models/flat_vase.obj");
  auto flatVase = LveGameObject::createGameObject();
  flatVase.model = lveModel;
  flatVase.transform.translation = {-0.5f, 0.5f, 2.5f};
  flatVase.transform.scale = {3.0f, 1.5f, 3.0f};  // glm::vec3(3.0f);
  gameObjects.push_back(std::move(flatVase));

  lveModel = LveModel::createmodelFromFile(lveDevice, "models/smooth_vase.obj");
  auto smoothVase = LveGameObject::createGameObject();
  smoothVase.model = lveModel;
  smoothVase.transform.translation = {0.5f, 0.5f, 2.5f};
  smoothVase.transform.scale = {3.0f, 1.5f, 3.0f};  // glm::vec3(3.0f);
  gameObjects.push_back(std::move(smoothVase));
}

void FirstApp::run() {
  std::vector<std::unique_ptr<LveBuffer>> uboBuffers(
      LveSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (size_t i = 0; i < uboBuffers.size(); i++) {
    uboBuffers[i] = std::make_unique<LveBuffer>(
        lveDevice, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        lveDevice.properties.limits.minUniformBufferOffsetAlignment);
    uboBuffers[i]->map();
  }

  auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
                             .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                         VK_SHADER_STAGE_VERTEX_BIT)
                             .build();

  std::vector<VkDescriptorSet> globalDescriptorSets(
      LveSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (size_t i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    LveDescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
  }

  SimpleRenderSystem simpleRenderSystem{
      lveDevice, lveRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  LveCamera camera{};

  auto viewerObject = LveGameObject::createGameObject();
  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime -
                                                                   currentTime)
            .count();
    currentTime = newTime;

    // To limit the frame time
    // frameTime = std::min(frameTime, MAX_FRAME_TIME);

    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime,
                                   viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation,
                      viewerObject.transform.rotation);

    float aspect = lveRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      int frameIndex = lveRenderer.getFrameIndex();
      FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera,
                          globalDescriptorSets[frameIndex]};

      // update
      GlobalUbo ubo{};
      ubo.projectionView = camera.getProjection() * camera.getView();
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

}  // namespace lve
