CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

option(CSHARP_NET_BINDINGS "Build SWIG bindings for C#/.NET" ON)
#option(CSHARP_MONO_BINDINGS "Build SWIG bindings for C#/Mono" ON) -- not supported by cmake at this time


find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})


set_property(SOURCE interfaces.i PROPERTY CPLUSPLUS OFF)
set_property(SOURCE interfaces.i PROPERTY SWIG_FLAGS "-includeall")
    set(CMAKE_SWIG_FLAGS -dllimport libh6n)
set_source_files_properties(interfaces.i PROPERTIES INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>;$<INSTALL_INTERFACE:include>")
swig_add_library(libh6n-csharp-bindings
	TYPE SHARED
	LANGUAGE "csharp"
	SOURCES interfaces.i ../src/interfaces.cpp
	OUTPUT_DIR csharp
)
set_target_properties(libh6n-csharp-bindings PROPERTIES OUTPUT_NAME "libh6n")
set_target_properties(libh6n-csharp-bindings PROPERTIES SWIG_USE_LIBRARY_INCLUDE_DIRECTORIES TRUE)

target_include_directories(libh6n-csharp-bindings PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>  $<INSTALL_INTERFACE:include>)
target_compile_definitions(libh6n-csharp-bindings PUBLIC _H6N_IMPLEMENTS_STATIC)

file(GLOB INSTALL_HEADERS "csharp/*.cs")
install(FILES ${INSTALL_HEADERS} DESTINATION csharp/sources)

install(TARGETS libh6n-csharp-bindings
    EXPORT "${TARGETS_EXPORT_NAME}"
    RUNTIME DESTINATION csharp/bin
)


if(CSHARP_NET_BINDINGS)
	include(CheckLanguage)
	check_language(CSharp)
	if(CMAKE_CSharp_COMPILER)
	include_external_msproject(libh6n-net ${CMAKE_CURRENT_LIST_DIR}/csharp/libh6n-net.csproj libh6n-csharp-bindings)
	else()
		message(WARNING "Building for C#/.NET enabled but no C#/.NET compiler available")
	endif()
endif()

