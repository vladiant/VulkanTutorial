#include <vulkan/vulkan.h>

#include <iostream>

int main() {
  uint32_t extensionsCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
  std::cout << "Vulkan extension count: " << extensionsCount << '\n';

  return EXIT_SUCCESS;
}