cmake_minimum_required(VERSION 3.21)
project(cpp_slide_10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(FP FP.cpp)