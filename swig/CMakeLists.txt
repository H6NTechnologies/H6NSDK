CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

option(CSHARP_BINDINGS "Build SWIG bindings for C#" ON)


find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})


set_property(SOURCE interfaces.i PROPERTY CPLUSPLUS OFF)
set_source_files_properties(interfaces.i PROPERTIES SWIG_FLAGS "-includeall" INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>;$<INSTALL_INTERFACE:include>")
swig_add_library(libh6n
	TYPE SHARED
	LANGUAGE "csharp"
	SOURCES interfaces.i ../src/interfaces.cpp)
set_target_properties(libh6n PROPERTIES SWIG_USE_LIBRARY_INCLUDE_DIRECTORIES TRUE)

target_include_directories(libh6n PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>  $<INSTALL_INTERFACE:include>)
target_compile_definitions(libh6n PUBLIC _H6N_IMPLEMENTS_STATIC)

if(CSHARP_BINDINGS)
	include(CheckLanguage)
	check_language(CSharp)
	if(CMAKE_CSharp_COMPILER)
		enable_language(CSharp)
		
	endif()
endif()
