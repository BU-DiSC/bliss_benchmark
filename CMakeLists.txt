cmake_minimum_required(VERSION 3.10)
project(bliss_benchmark VERSION 1.0
                        DESCRIPTION "Benchmark for sortedness on various indexes"
                        LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/spdlog)
add_subdirectory(external/cxxopts)

add_executable(bliss src/bliss.cpp)
target_link_libraries(bliss
    spdlog
    cxxopts
)
