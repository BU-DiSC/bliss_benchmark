get_filename_component(EXEC ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE CPP_TESTS "*_tests.cpp")
add_executable(${EXEC} ${CPP_TESTS})
target_link_libraries(${EXEC} PRIVATE 
bliss
bliss_test_infra 
GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${EXEC})