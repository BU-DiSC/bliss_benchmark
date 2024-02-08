cmake_minimum_required(VERSION 3.10)
project(bliss_benchmark VERSION 1.0
                        DESCRIPTION "Benchmark for sortedness on various indexes"
                        LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(external)

option(DEBUG "Debug mode on or off" OFF)
if(${DEBUG})
    set(CMAKE_BUILD_TYPE "Debug")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

# =============================================================================
# HEADER bliss
#   Bliss lib files
# =============================================================================
add_library(bliss OBJECT
    ${CMAKE_SOURCE_DIR}/src/bliss/util/timer.h
    ${CMAKE_SOURCE_DIR}/src/bliss/util/reader.h
)

target_compile_features(bliss PUBLIC
    cxx_std_11
)

target_link_libraries(bliss PUBLIC
    spdlog::spdlog
    cxxopts
    alex
    lipp
)

target_include_directories(bliss PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_options(bliss PUBLIC
    "-Wall"
    "-Wextra"
    "-fstack-protector-strong"
    "-fexceptions"
    "-fasynchronous-unwind-tables"
    $<$<BOOL:${DEBUG}>:-g2>
    $<IF:$<BOOL:${DEBUG}>,-O0,-O2>
)

target_include_directories(bliss PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

# =============================================================================
# HEADER bliss
#   Executable and linking libraries
# =============================================================================
add_executable(bliss_bench src/bliss_bench.cpp)

target_link_libraries(bliss_bench PUBLIC
    bliss
)
