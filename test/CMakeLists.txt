cmake_minimum_required(VERSION 3.0)
project(test_utils_cpp)

message("Enable " ${PROJECT_NAME})

file(GLOB_RECURSE
        ${PROJECT_NAME}_files
        ${PROJECT_SOURCE_DIR}/*.cc)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_files} buffer.cc base_pool.cc)
target_link_libraries(${PROJECT_NAME} PUBLIC utils_cpp GTest::GTest)
get_target_property(Utils_cpp_INCLUDE_DIRS utils_cpp INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        Utils_cpp_INCLUDE_DIRS
        GTest_INCLUDE_DIRS)
