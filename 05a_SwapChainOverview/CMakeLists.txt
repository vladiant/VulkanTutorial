cmake_minimum_required(VERSION 3.10)

project(5a_SwapChainOverview LANGUAGES CXX)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(PkgConfig)

pkg_check_modules(GLSLANG REQUIRED glslang)

find_program(GLSLANG_VALIDATOR NAMES glslangValidator)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)

add_custom_command(COMMENT "Compiling vertex shader"
                   OUTPUT simple_shader.vert.spv
                   COMMAND ${GLSLANG_VALIDATOR} -V -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.vert.spv
                           ${PROJECT_SOURCE_DIR}/shaders/simple_shader.vert
                   MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/shaders/simple_shader.vert
                   DEPENDS ${PROJECT_SOURCE_DIR}/shaders/simple_shader.vert ${GLSLANG_VALIDATOR})
add_custom_target(VertexShader ALL DEPENDS simple_shader.vert.spv)

add_custom_command(COMMENT "Compiling fragment shader"
                   OUTPUT simple_shader.frag.spv
                   COMMAND ${GLSLANG_VALIDATOR} -V -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.frag.spv
                           ${PROJECT_SOURCE_DIR}/shaders/simple_shader.frag
                   MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/shaders/simple_shader.frag
                   DEPENDS ${PROJECT_SOURCE_DIR}/shaders/simple_shader.frag ${GLSLANG_VALIDATOR})
add_custom_target(FragmentShader ALL DEPENDS simple_shader.frag.spv)

add_executable(
    ${PROJECT_NAME}
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

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        glfw
        Vulkan::Vulkan
)

add_dependencies(${PROJECT_NAME} FragmentShader VertexShader)
