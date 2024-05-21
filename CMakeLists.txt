## CMakeLists file for adding this library to a cmake project
## How to use:
## If the folder is inside project tree use add_subdirectory(<path to this folder>)
## If the folder is outside project tree use add_subdirectory(<path to this folder> /deps/libdpp-mcu)

cmake_minimum_required(VERSION 3.13)

## Create project
project(raygui-widgets
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)

## Cmake modules helper
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(DMainHelper)
include(DPrintHelpers)
include(FeatureSummary)

## Handle options
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(STAND_ALONE_LIB ON)
endif()

# Handle STATIC/SHARED/INTERFACE build option (default SHARED)
include(DLibraryType)

## Add source
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

## Setup Target
if (${PROJECT_NAME}_BUILD_INTERFACE_LIB)
    # Build as interface
    message_c("Setup as INTERFACE")
    add_library(${PROJECT_NAME} INTERFACE)
elseif(${PROJECT_NAME}_BUILD_SHARED_LIBS)
    # Building shared library
    message_c("Setup as SHARED")
    if(MSVC)
        # Workaround for some projects
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
    add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
else()
    # Static library
    message_c("Setup as STATIC")
    add_library(${PROJECT_NAME} ${SRC_FILES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIRS})
endif()

# Set library version
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${CMAKE_PROJECT_VERSION})

## Get Raylib
set(GLFW_BUILD_WAYLAND OFF) # TODO: detect wayland_scanner error
message_c("Get Raylib...")
find_package(Raylib)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

message_c("Get dpplib...")
set(dpplib_BUILD_INTERFACE_LIBS ON)
set(dpplib_BUILD_NET OFF)
set(dpplib_BUILD_QT OFF)
find_package(dpplib)
#[[
get_filename_component(dpplib_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/dpplib ABSOLUTE)
add_subdirectory(${dpplib_ROOT} _deps/dpplib)
target_include_directories(${PROJECT_NAME} PUBLIC ${dpplib_ROOT}/include)
get_target_property(DPPLIB_SOURCES dpplib SOURCES)
target_sources(${PROJECT_NAME} PUBLIC ${DPPLIB_SOURCES})
]]#
## Doxygen support
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Only if stand-alone project
    set(DOXYGEN_THEME custom1)
    set(DOXYGEN_LANGUAGE Italian)
    #add_subdirectory(doxygen)
    list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/doxygen)
    include(Doxygen)
endif()

#include(DInstall)

message_c(${BOLD_CYAN} "Summary:")
message_c(${BOLD_CYAN} "Build as:            ${${PROJECT_NAME}_LIBRARY_TYPE} LIB")
message_c(${BOLD_CYAN} "Build examples:      ${BUILD_EXAMPLES}")
#message_c(${BOLD_CYAN} "Build tests:         ${BUILD_TESTS}")

#print_project_summary()
#feature_summary(WHAT ALL)


#[[
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

set(SRC_FILES ${SRC_FILES} ${FILES} PARENT_SCOPE)
set(SRC_DIRS ${SRC_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
]]#