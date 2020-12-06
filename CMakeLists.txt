cmake_minimum_required(VERSION 3.0)
project(utils_cpp)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE
        ${PROJECT_NAME}_files
        ${PROJECT_SOURCE_DIR}/source/*.cc)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_files} test/main.cc)
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include
        INTERFACE_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include)

find_package(GTest REQUIRED)
if (GTEST_FOUND)
    add_subdirectory(test)
endif ()
