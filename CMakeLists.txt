cmake_minimum_required(VERSION 3.10)
project(CalcX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(CalcX src/main.cpp)
target_include_directories(CalcX PRIVATE include)
