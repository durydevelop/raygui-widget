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

## TODO:
# _ninja color CMAKE_CXX_FLAGS=-fdiagnostics-color=always
# _colori in shell vscode
# _Visualizza opzioni
# _Scarica e compila zlib in modalità Web
# _sudo apt-get install libdrm-dev libegl1-mesa-dev libgles2-mesa-dev libgbm-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

## Set default build type if needed
#if (NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
#    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
#endif()

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-tautological-constant-out-of-range-compare -Wno-type-limits -Wno-attributes)

## Setup Target
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(${PROJECT_NAME} INTERFACE) # TODO ${SRC_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${SRC_DIRS})

set(SRC_FILES ${SRC_FILES} ${FILES} PARENT_SCOPE)
set(SRC_DIRS ${SRC_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
