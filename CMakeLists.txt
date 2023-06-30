#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(containers LANGUAGES CXX)

#Find packages
find_package(Catch2 REQUIRED)

#Include Catch2 header
include(Catch)

#Allow options to enable asan and ubsan
option(ENABLE_ASAN "Address Sanitizer" FALSE)
option(ENABLE_UBSAN "Undefined Behaviour Sanitizer" FALSE)
option(ENABLE_LSAN "Leak Sanitizer" FALSE)

#Conditional logic for activating address sanitizer
if(ENABLE_ASAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

#Conditional logic for activating undefined behaviour sanitizer
if(ENABLE_UBSAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()

#Conditional logic for activating leak sanitizer
if(ENABLE_LSAN)
	set(CMAKE_CXX_FLAGS"${CMAKE_CXX_CLAGS} -fsanitize=leak")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
endif()

#Globally include 'include' directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Add executable targets
add_executable(test_sv_set ${CMAKE_CURRENT_SOURCE_DIR}/app/test_sv_set.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/sv_set.hpp)

#Link libraries and include target-specific directories
target_include_directories(test_sv_set PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
