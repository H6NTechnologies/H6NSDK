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
		set(DEFINITION_ARG "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/${DEF_FILE}\"")

		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
			add_library(${TARGET_NAME} SHARED ${STUB_FILE})
			# Pass linker flag via -Wl on Clang/GCC
			set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "-Wl,${DEFINITION_ARG}")
		elseif (MSVC)
			add_library(${TARGET_NAME} SHARED ${STUB_FILE} ${DEF_FILE})
			# MSVC invokes link.exe with parameters directly
			#set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "${DEFINITION_ARG}")
		endif()

		set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_NAME "${TARGET_NAME}")
		set_target_properties(${TARGET_NAME} PROPERTIES IMPORT_PREFIX "")
		set_target_properties(${TARGET_NAME} PROPERTIES IMPORT_SUFFIX ".lib")
		set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "")

	else()
		add_library(${TARGET_NAME} INTERFACE)
	endif()
	target_include_directories(${TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
endmacro(DefineImplib)

# Only generate implibs on Windows since other platforms are smart enough to link against
# the shared library directly
if(WIN32)
	if(BUILD_AGENT_LIB)
		DefineImplib(libh6n-agent src/agent.c src/agent.def)
	endif()

	if(BUILD_CAPSULE_LIB)
		DefineImplib(libh6n-capsule src/capsule.c src/capsule.def)
	endif()
endif()

add_library(libh6n-headers INTERFACE)
target_include_directories(libh6n-headers INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include")


if(WIN32)
	# Create "imported" target which imports the agent/capsule implibs
	add_library(libh6n INTERFACE)
	add_dependencies(libh6n libh6n-agent libh6n-capsule)

	get_filename_component(capsule_name $<TARGET_PROPERTY:libh6n-capsule,ARCHIVE_OUTPUT_NAME> "NAME_WE")
	get_filename_component(agent_name $<TARGET_PROPERTY:libh6n-agent,ARCHIVE_OUTPUT_NAME> "NAME_WE")

	set(bindir "${CMAKE_CURRENT_BINARY_DIR}/")

	target_link_libraries(libh6n INTERFACE libh6n-headers "${BINDIR}H6NSDK/${capsule_name}.lib" "${BINDIR}H6NSDK/${agent_name}.lib")

	# This is kind of a hack -- bug in CMake causes non-MSVC-built targets to link
	# against the DLL itself instead of the implib. 
	#set_target_properties(libh6n PROPERTIES IMPORTED_LOCATION  "$<TARGET_FILE:libh6n-capsule>")
	#set_target_properties(libh6n PROPERTIES INCLUDE_DIRECTORIES  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include")

else()
	# Create "interface" target which is header only
	add_library(libh6n ALIAS libh6n-headers)
endif()



# Deprecated
if(SWIG_BINDINGS)
	add_subdirectory(swig)
endif()

# Generate C# bindings
if(CSHARP_BINDINGS)
	add_subdirectory(csharp)
endif()
