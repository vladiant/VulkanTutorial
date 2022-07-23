## Video
* [Loading 3D Models - Vulkan Game Engine Tutorial 17](https://www.youtube.com/watch?v=jdiPVfIHmEA)

## Code
```cpp
#pragma once
 
#include <functional>
 
namespace lve {
 
// from: https://stackoverflow.com/a/57595105
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  (hashCombine(seed, rest), ...);
};
 
}  // namespace lve
```

## Links
* <https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h>
* [Models](https://drive.google.com/drive/folders/1QuvSRG4HCxCfC5k0F5G03tNwUQoqE8f3)
* [VkMemoryPropertyFlagBits(3) Manual Page](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkMemoryPropertyFlagBits.html)
