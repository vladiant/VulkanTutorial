cmake_minimum_required(VERSION 3.10)

project(17_Loading3DModels LANGUAGES CXX)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(PkgConfig)
find_package(glm REQUIRED)

pkg_check_modules(GLSLANG REQUIRED glslang)
pkg_check_modules(SPIRV_TOOLS REQUIRED SPIRV-Tools)

find_program(GLSLANG_VALIDATOR NAMES glslangValidator)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)
configure_file(shaders/simple_shader.frag ./shaders/simple_shader.frag COPYONLY)
configure_file(shaders/simple_shader.vert ./shaders/simple_shader.vert COPYONLY)

# Models
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/models)
configure_file(models/colored_cube.obj ./models/colored_cube.obj COPYONLY)
configure_file(models/cube.obj ./models/cube.obj COPYONLY)
configure_file(models/flat_vase.obj ./models/flat_vase.obj COPYONLY)
configure_file(models/smooth_vase.obj ./models/smooth_vase.obj COPYONLY)

# tinyobjloader
add_library(
    TinyObjLoaderLib
    tinyobjloader/tiny_obj_loader.h
)

set_target_properties(
    TinyObjLoaderLib
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        LINKER_LANGUAGE CXX
)

target_include_directories(
    TinyObjLoaderLib
    PUBLIC
        tinyobjloader
)

add_executable(
    ${PROJECT_NAME}
    util.hpp
    util.cpp
    lve_window.hpp
    lve_window.cpp
    first_app.hpp
    first_app.cpp
    lve_pipeline.hpp
    lve_pipeline.cpp
    lve_device.hpp
    lve_device.cpp
    lve_swap_chain.hpp
    lve_swap_chain.cpp
    lve_model.hpp
    lve_model.cpp
    lve_game_object.hpp
    lve_renderer.hpp
    lve_renderer.cpp
    simple_render_system.hpp
    simple_render_system.cpp
    lve_camera.hpp
    lve_camera.cpp
    keyboard_movement_controller.hpp
    keyboard_movement_controller.cpp
    lve_utils.hpp
    main.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${GLSLANG_INCLUDE_DIRS}
        ${SPIRV_TOOLS_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        ${GLSLANG_LIBRARIES}
        SPIRV
        ${SPIRV_TOOLS_LIBRARIES}
        pthread
        glfw
        Vulkan::Vulkan
        TinyObjLoaderLib
)
