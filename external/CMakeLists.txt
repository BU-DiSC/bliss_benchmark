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
    alex
    GIT_REPOSITORY https://github.com/microsoft/ALEX.git
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
    GIT_REPOSITORY https://github.com/Jiacheng-WU/lipp
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