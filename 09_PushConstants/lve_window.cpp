#include "lve_window.hpp"

#include <stdexcept>

namespace lve {

LveWindow::LveWindow(int w, int h, const std::string& name)
    : width{w}, height{h}, windowName{name} {
  initWindow();
}

LveWindow::~LveWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void LveWindow::frameBufferResizedCallback(GLFWwindow* window, int width,
                                           int height) {
  auto lveWindow =
      reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
  lveWindow->frameBufferResized = true;
  lveWindow->width = width;
  lveWindow->height = height;
}

void LveWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetWindowSizeCallback(window, frameBufferResizedCallback);
}

bool LveWindow::shouldClose() { return glfwWindowShouldClose(window); }

void LveWindow::createWindowSurface(VkInstance instance,
                                    VkSurfaceKHR* surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }
}

}  // namespace lve
