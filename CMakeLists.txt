CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

project(h6nsdk C)

option(SWIG_BINDINGS "Build SWIG bindings for C#, Java, Go, etc" OFF)
option(CSHARP_BINDINGS "Build C# bindings" ON)

option(BUILD_AGENT_LIB "Generate Windows import lib for H6Agent" ON)
option(BUILD_CAPSULE_LIB "Generate Windows import lib for libcapsule" ON)

# Fix dumb bug in cmake...
if(CMAKE_C_STANDARD_DEFAULT EQUAL 98)
    set(CMAKE_C_STANDARD_DEFAULT 99 CACHE STRING "" FORCE)
endif()

# C90 and below is too old for us to feasibly support
set(CMAKE_C_STANDARD 99)

macro(DefineImplib TARGET_NAME STUB_FILE DEF_FILE)
	# If targetting Windows, we need to generate an import library (implib) which is a
	# static library of stubs.
	if (WIN32)
		set(DEFINITION_ARG "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/src/agent.def\"")
		add_library(${TARGET_NAME} SHARED src/agent.c)

		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
			# Pass linker flag via -Wl on Clang/GCC
			set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "-Wl,${DEFINITION_ARG}")
		elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
			# MSVC invokes link.exe with parameters directly
			set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "${DEFINITION_ARG}")
		endif()

		# set base name of generated DLL import library
		set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_NAME "${TARGET_NAME}")
		# set prefix of generated DLL import library
		set_target_properties(${TARGET_NAME} PROPERTIES IMPORT_PREFIX "")
		# set suffix of generated DLL import library
		set_target_properties(${TARGET_NAME} PROPERTIES IMPORT_SUFFIX ".lib")
	else()
		add_library(${TARGET_NAME} INTERFACE)
	endif()
	target_include_directories(${TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
endmacro(DefineImplib)

if(BUILD_AGENT_LIB)
	DefineImplib(libh6n-agent src/agent.c src/agent.def)
endif()

if(BUILD_CAPSULE_LIB)
	DefineImplib(libh6n-capsule src/capsule.c src/capsule.def)
endif()

add_library(libh6n INTERFACE)
target_include_directories(libh6n INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

if(SWIG_BINDINGS)
	add_subdirectory(swig)
endif()

if(CSHARP_BINDINGS)
	add_subdirectory(csharp)
endif()
