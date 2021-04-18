## Setup base
cmake_minimum_required(VERSION 3.16)
project(VeNo)

# SET VARIABLES
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

add_library(sse4_1 INTERFACE IMPORTED)
if (MSVC)
    target_compile_options(sse4_1 INTERFACE /arch:SSE4.1)
else ()
    target_compile_options(sse4_1 INTERFACE -msse4.1)
endif ()

option(UniversalBinary "Build universal binary for mac" OFF)

if (UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif ()
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

# SETUP JUCE
include(CMake/GitFunctions.cmake)
update_from_git(juce "https://github.com/juce-framework/JUCE" master)

## SETUP OPTIONS
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

add_subdirectory(${juce_SOURCE_DIR})
add_subdirectory(Plugin)