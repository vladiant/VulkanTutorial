#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve {

class LveWindow {
 public:
  LveWindow(int w, int h, std::string  name);
  ~LveWindow();

  LveWindow(const LveWindow&) = delete;
  LveWindow& operator=(const LveWindow&) = delete;

  bool shouldClose();
  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(height), static_cast<uint32_t>(width)};
  }

  bool wasWindowResized() { return frameBufferResized; }
  void resetWindowResizedFlag() { frameBufferResized = false; }

  // TODO: This should be reworked!!!
  GLFWwindow* getGLFWwindow() const { return window; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface_);

 private:
  static void frameBufferResizedCallback(GLFWwindow* window, int width,
                                         int height);

  void initWindow();

  int width;
  int height;
  bool frameBufferResized = false;

  std::string windowName;

  GLFWwindow* window{};
};

}  // namespace lve
