# Vulkan Tutorial

Vulkan tutorial samples based on https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine/ and [Vulkan (c++) Game Engine Tutorials](https://www.youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR) from [Brendan Galea](https://www.youtube.com/c/BrendanGalea)

## Code improvement commands
```bash
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
run-clang-tidy -checks='cppcoreguidelines-*,-cppcoreguidelines-prefer-member-initializer,readibility-*,modernize-*,-modernize-use-trailing-return-type,misc-*,clang-analyzer-*' -fix
```
