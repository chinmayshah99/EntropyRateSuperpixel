cmake_minimum_required(VERSION 2.8.12)

project(ERS)

file(GLOB SOURCES "*.cpp")
add_library(ERS ${SOURCES})

target_include_directories(ERS PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)
