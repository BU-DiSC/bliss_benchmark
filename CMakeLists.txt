cmake_minimum_required(VERSION 3.10)
project(bliss_benchmark VERSION 1.0
                        DESCRIPTION "Benchmark for sortedness on various indexes"
                        LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(external)

# =============================================================================
# HEADER bliss
#   Executable and linking libraries
# =============================================================================
add_executable(bliss src/bliss.cpp)
target_link_libraries(bliss
    spdlog::spdlog
    cxxopts
    alex
    lipp
)
