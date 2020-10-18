cmake_minimum_required(VERSION 3.15)
project(untitled2)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled2 main.cpp ProcessUtils.cpp)
#[[add_executable(untitled2 plus.cpp ProcessUtils.cpp)]]

set(CMAKE_CXX_FLAGS "-lpthread -lrt ")


