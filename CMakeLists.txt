cmake_minimum_required(VERSION 3.31)
project(analizaHeap)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(analizaHeap main.cpp)
add_executable(simple_test_generator generator.cpp)
