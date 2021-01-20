cmake_minimum_required(VERSION 3.0)
project(utils_cpp)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    add_definitions(-DOS_WIN32=1)
else ()
    if (APPLE)
        # Cmake on Apple would not search packages from home brew by default
        find_program(brew_bin_path brew)
        if (brew_bin_path)
            string(REGEX MATCH "^.+homebrew" brew_prefix ${brew_bin_path})
            list(APPEND CMAKE_SYSTEM_PREFIX_PATH ${brew_prefix})
            link_directories(${brew_prefix}/lib)
            include_directories(${brew_prefix}/include)
        endif ()
        add_definitions(-DOS_UNIX=1)
        add_definitions(-DOS_XNU=1)

        if (IOS)
            #cmake_minimum_required(VERSION 3.15)
            add_definitions(-DOS_IOS=1)
        else ()
            add_definitions(-DOS_MACOSX=1)
        endif ()

    elseif (UNIX)
        add_definitions(-DOS_UNIX=1)
        add_definitions(-DOS_LINUX=1)
        if (ANDROID)
            add_definitions(-DOS_ANDROID=1)
        endif ()
    else ()
    endif ()
endif ()


file(GLOB_RECURSE
        ${PROJECT_NAME}_files
        ${PROJECT_SOURCE_DIR}/source/*.c
        ${PROJECT_SOURCE_DIR}/source/*.cc)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_files})
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include
        INTERFACE_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include)

find_package(GTest QUIET)
if (GTEST_FOUND)
    add_subdirectory(test)
endif ()
