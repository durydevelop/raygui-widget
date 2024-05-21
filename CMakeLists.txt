## CMakeLists file for adding this library to a cmake project
## How to use:
## If the folder is inside project tree use add_subdirectory(<path to this folder>)
## If the folder is outside project tree use add_subdirectory(<path to this folder> /deps/libdpp-mcu)

cmake_minimum_required(VERSION 3.13)

## Create project
project(raygui-widgets
    VERSION 0.0.1
    LANGUAGES C CXX
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)

## Cmake modules helper
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(DMainHelper)
include(DPrintHelpers)

## Setup Target
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(${PROJECT_NAME} INTERFACE) # TODO ${SRC_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${SRC_DIRS})

if (NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    # Added as external project
    set(SRC_FILES ${SRC_FILES} ${FILES} PARENT_SCOPE)
    set(SRC_DIRS ${SRC_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
endif()
