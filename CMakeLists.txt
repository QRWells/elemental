cmake_minimum_required(VERSION 3.22)

# Project information
project(elemental
    VERSION 0.0.1
    DESCRIPTION "elemental is a simple LMNtal runtime."
    LANGUAGES C CXX)

# ================================

# Global configuration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Include directories
add_subdirectory(src)