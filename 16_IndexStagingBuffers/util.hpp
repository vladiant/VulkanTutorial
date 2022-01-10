#pragma once

// https://lxjk.github.io/2020/03/10/Translate-GLSL-to-SPIRV-for-Vulkan-at-Runtime.html

#include <glslang/SPIRV/GlslangToSpv.h>

#include <vulkan/vulkan.hpp>

struct SpirvHelper {
  static void Init();

  static void Finalize();

  static void InitResources(TBuiltInResource &Resources);

  static EShLanguage FindLanguage(const vk::ShaderStageFlagBits shader_type);

  static bool GLSLtoSPV(const vk::ShaderStageFlagBits shader_type,
                        const char *pshader, std::vector<unsigned int> &spirv);
};