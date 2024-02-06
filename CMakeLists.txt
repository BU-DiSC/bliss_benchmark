cmake_minimum_required(VERSION 3.10)
project(bliss_benchmark VERSION 1.0
                        DESCRIPTION "Benchmark for sortedness on various indexes"
                        LANGUAGES CXX
)
add_executable(execute main.cpp)
