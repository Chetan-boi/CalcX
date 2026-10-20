cmake_minimum_required(VERSION 3.10)
project(CalcX 
    VERSION 1.0.0 
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build timestamps
string(TIMESTAMP BUILD_DATE "%Y-%m-%d" UTC)
string(TIMESTAMP BUILD_TIME "%H:%M:%S UTC" UTC)

set(PROJECT_HOMEPAGE "https://github.com/Chetan-boi/CalcX")

if(NOT DEFINED PROJECT_VERSION_PATCH OR PROJECT_VERSION_PATCH STREQUAL "")
    set(PROJECT_VERSION_PATCH 0)
endif()

# Configure version.h from version.h.in
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/version.h
    @ONLY
)

file(GLOB SOURCES "src/*")
add_executable(CalcX ${SOURCES})

target_include_directories(CalcX PRIVATE 
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)
