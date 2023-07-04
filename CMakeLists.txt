#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(containers LANGUAGES CXX)

#Disable multithreading
set(Boost_USE_MULTITHREADED FALSE)

#Find packages
find_package(Catch2 REQUIRED)
find_package(Boost REQUIRED)

#Include Catch2 header
include(Catch)

#Include code sanitizers
include(${CMAKE_CURRENT_SOURCE_DIR}/Sanitizers.cmake)

#Globally include 'include' directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Add executable targets
add_executable(test_sv_set ${CMAKE_CURRENT_SOURCE_DIR}/app/test_sv_set.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/sv_set.hpp)
add_executable(test_intrusive_list ${CMAKE_CURRENT_SOURCE_DIR}/app/test_intrusive_list.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/intrusive_list.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/parent_from_member.hpp)

#Include Boost directories for intrusive list target
target_include_directories(test_intrusive_list PUBLIC ${Boost_INCLUDE_DIRS})
