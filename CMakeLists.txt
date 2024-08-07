cmake_minimum_required(VERSION 3.10)

project(Tarea1_D2)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(generator src/generator.cpp)
add_executable(sorter src/sorter.cpp src/PagedArray.cpp)
