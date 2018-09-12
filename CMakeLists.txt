cmake_minimum_required(VERSION 3.0)
project(utilscpp)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE ${PROJECT_NAME}_files
    *.h
    *.cc)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_files})