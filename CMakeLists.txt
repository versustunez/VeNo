## Setup base
cmake_minimum_required(VERSION 3.16)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "" FORCE)

project(VeNo)

# SET VARIABLES
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(JUCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/JUCE")

option(OSC_ENABLE_SIMD "Enable AVX2 SIMD acceleration" ON)
option(UniversalBinary "Build universal binary for mac" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

include(CMake/Sources.cmake)

if(CMAKE_CXX_COMPILER_ID MATCHES "^GNU$")
  message(STATUS "Add GNU Options for Compiling")
  add_compile_options(-Wno-free-nonheap-object -Wextra-semi)
endif()

if(UniversalBinary)
  set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif()

#removes MSVC warning: D9025
if (MSVC)
  string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif ()

#static linking in Windows:
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

## SETUP OPTIONS
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

add_subdirectory(vendor/fmt)
add_subdirectory(${JUCE_ROOT} JUCE)

set_property(TARGET fmt PROPERTY SYSTEM TRUE)
add_subdirectory(Plugin)
