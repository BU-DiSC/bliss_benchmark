include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_library(bliss_test_infra OBJECT 
${CMAKE_CURRENT_SOURCE_DIR}/bliss_index_tests.h)

target_include_directories(bliss_test_infra PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(test_alex)
add_subdirectory(test_lipp)
add_subdirectory(test_btree)
<<<<<<< HEAD
add_subdirectory(test_imprints)
=======
add_subdirectory(test_skiplist)
add_subdirectory(test_art)
>>>>>>> 92615345b27bee160b0af305c6ffc0870d9725b1
