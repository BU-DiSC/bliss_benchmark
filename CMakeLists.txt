cmake_minimum_required(VERSION 3.10)
project(bliss_benchmark VERSION 1.0
                        DESCRIPTION "Benchmark for sortedness on various indexes"
                        LANGUAGES CXX
)

include(ExternalProject)
message(STATUS "CXX : ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
endif()

# =============================================================================
# HEADER git-submodules
# =============================================================================
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

# =============================================================================
# HEADER external
#   add external libs (after git submodule init)
# =============================================================================
add_subdirectory(external)

# =============================================================================
# HEADER bliss
#   Bliss lib files
# =============================================================================
add_library(bliss OBJECT
    ${CMAKE_SOURCE_DIR}/src/bliss/util/timer.h
    ${CMAKE_SOURCE_DIR}/src/bliss/util/reader.h
    ${CMAKE_SOURCE_DIR}/src/bliss/util/args.h
    ${CMAKE_SOURCE_DIR}/src/bliss/util/config.h
    ${CMAKE_SOURCE_DIR}/src/bliss/bliss_index.h
    ${CMAKE_SOURCE_DIR}/src/bliss/bench_lipp.h
    ${CMAKE_SOURCE_DIR}/src/bliss/bench_alex.h
    ${CMAKE_SOURCE_DIR}/src/bliss/bench_btree.h
)

target_compile_features(bliss PUBLIC
    cxx_std_17
)

target_link_libraries(bliss PUBLIC
    spdlog::spdlog
    cxxopts
    alex
    lipp
    tlx
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
    "-march=native"
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
