cmake_minimum_required(VERSION 2.8.12)
project(erspy)

add_subdirectory(pybind11)
add_subdirectory(ERS)


# target_link_libraries(erspy ERS)
add_executable(erspy vc/erspy.cpp)
target_link_libraries(erspy pybind11 ERS)
