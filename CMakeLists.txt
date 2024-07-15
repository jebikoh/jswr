cmake_minimum_required(VERSION 3.28)
project(jswr VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -mavx2)

# Build type
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Test" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

# Library: glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ext/glfw)

# Library: assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(ext/assimp)

# Library: jtxlib
add_subdirectory(ext/jtxlib)

# Library: glad
include_directories(ext/glad/include)

# Executable
add_executable(jswr src/main.cpp
        src/camera.hpp
        src/model.cpp
        src/model.hpp
        src/buffers.hpp
        src/shader.hpp
        src/rasterizer.hpp
        src/renderer.hpp
        src/glad.c
        src/renderer.cpp
)
target_link_libraries(jswr PRIVATE jtxlib glfw assimp)
target_include_directories(jswr
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/jtxlib/src
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/stb
)

# Test assertion settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(jswr PRIVATE DEBUG_ASSERT)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    target_compile_definitions(jswr PRIVATE TEST_ASSERT)
endif()

if(WIN32)
    add_custom_command(TARGET jswr POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:assimp>
            $<TARGET_FILE_DIR:jswr>
    )
endif()