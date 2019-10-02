cmake_minimum_required(VERSION 2.8.12)
project(erspy)

add_subdirectory(pybind11)
add_subdirectory(numpy)
pybind11_add_module(erspy vc/erspy.cpp)