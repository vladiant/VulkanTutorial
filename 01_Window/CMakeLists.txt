cmake_minimum_required(VERSION 3.10)

project(1_Window LANGUAGES CXX)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(
    ${PROJECT_NAME}
    lve_window.hpp
    lve_window.cpp
    first_app.hpp
    first_app.cpp
    main.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        glfw
        Vulkan::Vulkan
)
