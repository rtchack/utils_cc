cmake_minimum_required(VERSION 3.0)
project(test_utils_cc)

message("Enable " ${PROJECT_NAME})

file(GLOB_RECURSE
        ${PROJECT_NAME}_files
        ${PROJECT_SOURCE_DIR}/*.cc)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_files} buffer.cc base_pool.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC utils_cc GTest::GTest)
get_target_property(UCC_INCLUDE_DIRS utils_cc INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME} PUBLIC ${UCC_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
get_target_property(TEST_INCLUDES ${PROJECT_NAME} INCLUDE_DIRECTORIES)
