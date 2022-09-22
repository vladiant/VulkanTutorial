cmake_minimum_required(VERSION 3.10)

project(10_2Dtransformations LANGUAGES CXX)

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
    main.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
         $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
         $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
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
)
