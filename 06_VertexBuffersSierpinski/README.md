## Video
* [Vertex Buffers - Vulkan Game Engine Tutorial 06](https://www.youtube.com/watch?v=mnKp501RXDc)

## Code
```cpp
void FirstApp::sierpinski(
    std::vector<LveModel::Vertex> &vertices,
    int depth,
    glm::vec2 left,
    glm::vec2 right,
    glm::vec2 top) {
  if (depth <= 0) {
    vertices.push_back({top});
    vertices.push_back({right});
    vertices.push_back({left});
  } else {
    auto leftTop = 0.5f * (left + top);
    auto rightTop = 0.5f * (right + top);
    auto leftRight = 0.5f * (left + right);
    sierpinski(vertices, depth - 1, left, leftRight, leftTop);
    sierpinski(vertices, depth - 1, leftRight, right, rightTop);
    sierpinski(vertices, depth - 1, leftTop, rightTop, top);
  }
}
void FirstApp::loadModels() {
  std::vector<LveModel::Vertex> vertices{};
  sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
  lveModel = std::make_unique<LveModel>(lveDevice, vertices);
}
```

## Links
* [A Simple Device Memory Allocator For Vulkan](http://kylehalladay.com/blog/tutorial/2017/12/13/Custom-Allocators-Vulkan.html)
* [VkFormat(3) Manual Page](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html)
* [maxMemoryAllocationCount - VkPhysicalDeviceLimits(3) Manual Page](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceLimits.html)
* [Vulkan® Memory Allocator](https://gpuopen.com/vulkan-memory-allocator/)
