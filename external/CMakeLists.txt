# =============================================================================
# HEADER FetchContent
#   Dependencies for bliss including logging and index structures
# =============================================================================
include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.x
)
FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif()



FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG        v3.1.1
    GIT_SHALLOW    TRUE
)
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_WARNINGS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cxxopts)



FetchContent_Declare(
    alex
    GIT_REPOSITORY https://github.com/microsoft/ALEX
    GIT_TAG master
)
FetchContent_GetProperties(alex)
if (NOT alex_POPULATED)
    FetchContent_Populate(alex)
endif()

add_library(alex INTERFACE)
target_include_directories(alex INTERFACE ${alex_SOURCE_DIR}/src/core)



FetchContent_Declare(
    lipp
    GIT_REPOSITORY https://github.com/Jiacheng-WU/lipp.git
    GIT_TAG main
)
FetchContent_GetProperties(lipp)
if (NOT lipp_POPULATED)
    FetchContent_Populate(lipp)
endif()

add_library(lipp INTERFACE)
target_include_directories(lipp INTERFACE ${lipp_SOURCE_DIR}/src/core)

FetchContent_Declare(
        tlx
        GIT_REPOSITORY https://github.com/tlx/tlx.git
        GIT_TAG master
)
FetchContent_GetProperties(tlx)
if (NOT tlx_POPULATED)
    FetchContent_Populate(tlx)
endif()

add_library(tlx INTERFACE)
target_include_directories(tlx INTERFACE ${tlx_SOURCE_DIR}/)


FetchContent_Declare(
        byteslice
        GIT_REPOSITORY https://github.com/altramarine/ByteSlice.git
        GIT_TAG master
)
FetchContent_GetProperties(byteslice)
if (NOT byteslice_POPULATED)
    FetchContent_Populate(byteslice)
endif()

add_compile_options(-mavx2 -m64 -fopenmp)
# FetchContent_MakeAvailable(byteslice)
file(GLOB_RECURSE byteslice_SOURCES
   "${byteslice_SOURCE_DIR}/src/*.cpp"
)

add_library(byteslice STATIC ${byteslice_SOURCES})
target_include_directories(byteslice PUBLIC ${byteslice_SOURCE_DIR}/)
# OpenMP is required for byteslice
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(byteslice PRIVATE OpenMP::OpenMP_CXX)
endif()
# add_library(byteslice STATIC ${byteslice_Sources})
FetchContent_Declare(
    skiplist
    GIT_REPOSITORY https://github.com/petegoodliffe/skip_list
    GIT_TAG master
)
FetchContent_GetProperties(skiplist)
if (NOT skiplist_POPULATED)
    FetchContent_Populate(skiplist)
endif()

add_library(skiplist INTERFACE)
target_include_directories(skiplist INTERFACE ${skiplist_SOURCE_DIR})


# Adaptive Radix Tree 
FetchContent_Declare(
    art
    GIT_REPOSITORY https://github.com/BU-DiSC/ART
    GIT_TAG main
)
FetchContent_GetProperties(art)
if (NOT art_POPULATED)
    FetchContent_Populate(art)
endif()

add_library(art INTERFACE)
target_include_directories(art INTERFACE ${art_SOURCE_DIR})



FetchContent_Declare(
    pgm
    GIT_REPOSITORY https://github.com/gvinciguerra/PGM-index
    GIT_TAG master
)
FetchContent_GetProperties(pgm)
if (NOT pgm_POPULATED)
    FetchContent_Populate(pgm)
endif()

add_library(pgm INTERFACE)
target_include_directories(pgm INTERFACE ${pgm_SOURCE_DIR})
