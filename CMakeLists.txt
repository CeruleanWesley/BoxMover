cmake_minimum_required(VERSION 3.14)

project(BoxMover)
include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY : " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory(src)
add_subdirectory(test)