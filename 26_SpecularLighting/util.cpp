#include "util.hpp"

// https://lxjk.github.io/2020/03/10/Translate-GLSL-to-SPIRV-for-Vulkan-at-Runtime.html
// https://github.com/KhronosGroup/glslang/pull/2038

void SpirvHelper::Init() { glslang::InitializeProcess(); }

void SpirvHelper::Finalize() { glslang::FinalizeProcess(); }

void SpirvHelper::InitResources(TBuiltInResource &Resources) {
  Resources.maxLights = 32;
  Resources.maxClipPlanes = 6;
  Resources.maxTextureUnits = 32;
  Resources.maxTextureCoords = 32;
  Resources.maxVertexAttribs = 64;
  Resources.maxVertexUniformComponents = 4096;
  Resources.maxVaryingFloats = 64;
  Resources.maxVertexTextureImageUnits = 32;
  Resources.maxCombinedTextureImageUnits = 80;
  Resources.maxTextureImageUnits = 32;
  Resources.maxFragmentUniformComponents = 4096;
  Resources.maxDrawBuffers = 32;
  Resources.maxVertexUniformVectors = 128;
  Resources.maxVaryingVectors = 8;
  Resources.maxFragmentUniformVectors = 16;
  Resources.maxVertexOutputVectors = 16;
  Resources.maxFragmentInputVectors = 15;
  Resources.minProgramTexelOffset = -8;
  Resources.maxProgramTexelOffset = 7;
  Resources.maxClipDistances = 8;
  Resources.maxComputeWorkGroupCountX = 65535;
  Resources.maxComputeWorkGroupCountY = 65535;
  Resources.maxComputeWorkGroupCountZ = 65535;
  Resources.maxComputeWorkGroupSizeX = 1024;
  Resources.maxComputeWorkGroupSizeY = 1024;
  Resources.maxComputeWorkGroupSizeZ = 64;
  Resources.maxComputeUniformComponents = 1024;
  Resources.maxComputeTextureImageUnits = 16;
  Resources.maxComputeImageUniforms = 8;
  Resources.maxComputeAtomicCounters = 8;
  Resources.maxComputeAtomicCounterBuffers = 1;
  Resources.maxVaryingComponents = 60;
  Resources.maxVertexOutputComponents = 64;
  Resources.maxGeometryInputComponents = 64;
  Resources.maxGeometryOutputComponents = 128;
  Resources.maxFragmentInputComponents = 128;
  Resources.maxImageUnits = 8;
  Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
  Resources.maxCombinedShaderOutputResources = 8;
  Resources.maxImageSamples = 0;
  Resources.maxVertexImageUniforms = 0;
  Resources.maxTessControlImageUniforms = 0;
  Resources.maxTessEvaluationImageUniforms = 0;
  Resources.maxGeometryImageUniforms = 0;
  Resources.maxFragmentImageUniforms = 8;
  Resources.maxCombinedImageUniforms = 8;
  Resources.maxGeometryTextureImageUnits = 16;
  Resources.maxGeometryOutputVertices = 256;
  Resources.maxGeometryTotalOutputComponents = 1024;
  Resources.maxGeometryUniformComponents = 1024;
  Resources.maxGeometryVaryingComponents = 64;
  Resources.maxTessControlInputComponents = 128;
  Resources.maxTessControlOutputComponents = 128;
  Resources.maxTessControlTextureImageUnits = 16;
  Resources.maxTessControlUniformComponents = 1024;
  Resources.maxTessControlTotalOutputComponents = 4096;
  Resources.maxTessEvaluationInputComponents = 128;
  Resources.maxTessEvaluationOutputComponents = 128;
  Resources.maxTessEvaluationTextureImageUnits = 16;
  Resources.maxTessEvaluationUniformComponents = 1024;
  Resources.maxTessPatchComponents = 120;
  Resources.maxPatchVertices = 32;
  Resources.maxTessGenLevel = 64;
  Resources.maxViewports = 16;
  Resources.maxVertexAtomicCounters = 0;
  Resources.maxTessControlAtomicCounters = 0;
  Resources.maxTessEvaluationAtomicCounters = 0;
  Resources.maxGeometryAtomicCounters = 0;
  Resources.maxFragmentAtomicCounters = 8;
  Resources.maxCombinedAtomicCounters = 8;
  Resources.maxAtomicCounterBindings = 1;
  Resources.maxVertexAtomicCounterBuffers = 0;
  Resources.maxTessControlAtomicCounterBuffers = 0;
  Resources.maxTessEvaluationAtomicCounterBuffers = 0;
  Resources.maxGeometryAtomicCounterBuffers = 0;
  Resources.maxFragmentAtomicCounterBuffers = 1;
  Resources.maxCombinedAtomicCounterBuffers = 1;
  Resources.maxAtomicCounterBufferSize = 16384;
  Resources.maxTransformFeedbackBuffers = 4;
  Resources.maxTransformFeedbackInterleavedComponents = 64;
  Resources.maxCullDistances = 8;
  Resources.maxCombinedClipAndCullDistances = 8;
  Resources.maxSamples = 4;
  Resources.maxMeshOutputVerticesNV = 256;
  Resources.maxMeshOutputPrimitivesNV = 512;
  Resources.maxMeshWorkGroupSizeX_NV = 32;
  Resources.maxMeshWorkGroupSizeY_NV = 1;
  Resources.maxMeshWorkGroupSizeZ_NV = 1;
  Resources.maxTaskWorkGroupSizeX_NV = 32;
  Resources.maxTaskWorkGroupSizeY_NV = 1;
  Resources.maxTaskWorkGroupSizeZ_NV = 1;
  Resources.maxMeshViewCountNV = 4;
  Resources.limits.nonInductiveForLoops = true;
  Resources.limits.whileLoops = true;
  Resources.limits.doWhileLoops = true;
  Resources.limits.generalUniformIndexing = true;
  Resources.limits.generalAttributeMatrixVectorIndexing = true;
  Resources.limits.generalVaryingIndexing = true;
  Resources.limits.generalSamplerIndexing = true;
  Resources.limits.generalVariableIndexing = true;
  Resources.limits.generalConstantMatrixVectorIndexing = true;
}

EShLanguage SpirvHelper::FindLanguage(
    const vk::ShaderStageFlagBits shader_type) {
  switch (shader_type) {
    case vk::ShaderStageFlagBits::eVertex:
      return EShLangVertex;
    case vk::ShaderStageFlagBits::eTessellationControl:
      return EShLangTessControl;
    case vk::ShaderStageFlagBits::eTessellationEvaluation:
      return EShLangTessEvaluation;
    case vk::ShaderStageFlagBits::eGeometry:
      return EShLangGeometry;
    case vk::ShaderStageFlagBits::eFragment:
      return EShLangFragment;
    case vk::ShaderStageFlagBits::eCompute:
      return EShLangCompute;
    default:
      return EShLangVertex;
  }
}

bool SpirvHelper::GLSLtoSPV(const vk::ShaderStageFlagBits shader_type,
                            const char *pshader,
                            std::vector<unsigned int> &spirv) {
  EShLanguage stage = FindLanguage(shader_type);
  glslang::TShader shader(stage);
  glslang::TProgram program;
  const char *shaderStrings[1];
  TBuiltInResource Resources = {};
  InitResources(Resources);

  // Enable SPIR-V and Vulkan rules when parsing GLSL
  auto messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

  shaderStrings[0] = pshader;
  shader.setStrings(shaderStrings, 1);

  if (!shader.parse(&Resources, 100, false, messages)) {
    puts(shader.getInfoLog());
    puts(shader.getInfoDebugLog());
    return false;  // something didn't work
  }

  program.addShader(&shader);

  //
  // Program-level processing...
  //

  if (!program.link(messages)) {
    puts(shader.getInfoLog());
    puts(shader.getInfoDebugLog());
    fflush(stdout);
    return false;
  }

  glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
  return true;
}
