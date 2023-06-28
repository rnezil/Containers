#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(containers LANGUAGES CXX)

#Find packages
find_package(Catch2 REQUIRED)

#Include Catch2 header
include(Catch)

#Globally include 'include' directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Add executable targets
add_executable(test_sv_set ${CMAKE_CURRENT_SOURCE_DIR}/app/test_sv_set.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/sv_set.hpp)

#Link libraries and include target-specific directories
target_include_directories(test_sv_set PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
