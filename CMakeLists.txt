cmake_minimum_required(VERSION 3.28)
project(jswr VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -mavx2)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Test" CACHE STRING "" FORCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

add_subdirectory(ext/jtxlib)

add_executable(jswr src/main.cpp
        src/camera.hpp
        src/model.cpp
        src/model.hpp
        src/buffers.hpp)

target_link_libraries(jswr PRIVATE jtxlib)

target_include_directories(jswr
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/jtxlib/src
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(jswr PRIVATE DEBUG_ASSERT)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    target_compile_definitions(jswr PRIVATE TEST_ASSERT)
endif()
