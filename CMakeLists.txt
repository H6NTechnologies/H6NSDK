CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

project(h6nsdk C CXX)

option(SWIG_BINDINGS "Build SWIG bindings for C#, Java, Go, etc" ON)
option(CSHARP_BINDINGS "Build C# bindings" OFF)

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

		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU)
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
		target_compile_definitions(${TARGET_NAME} PRIVATE "_H6N_IMPLEMENTS_EXPORT")
	else()
		add_library(${TARGET_NAME} INTERFACE)
	endif()
	target_include_directories(${TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
endmacro(DefineImplib)

# Only generate implibs on Windows since other platforms are smart enough to link against
# the shared library directly
if(WIN32)
	if(BUILD_AGENT_LIB)
		DefineImplib(libh6n-agent impgen/agent.c impgen/agent.def)
	endif()

	if(BUILD_CAPSULE_LIB)
		DefineImplib(libh6n-capsule impgen/capsule.c impgen/capsule.def)
	endif()
endif()

add_library(libh6n-headers INTERFACE)
target_include_directories(libh6n-headers INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include")

if(WIN32)
	# Create "imported" target which imports the agent/capsule implibs
	add_library(libh6n-direct INTERFACE)
	add_dependencies(libh6n-direct libh6n-agent libh6n-capsule)

	get_filename_component(capsule_name $<TARGET_PROPERTY:libh6n-capsule,ARCHIVE_OUTPUT_NAME> "NAME_WE")
	get_filename_component(agent_name $<TARGET_PROPERTY:libh6n-agent,ARCHIVE_OUTPUT_NAME> "NAME_WE")

	set(bindir "${CMAKE_CURRENT_BINARY_DIR}/")

	target_link_libraries(libh6n-direct INTERFACE libh6n-headers "${BINDIR}H6NSDK/${capsule_name}.lib" "${BINDIR}H6NSDK/${agent_name}.lib")

	# This is kind of a hack -- bug in CMake causes non-MSVC-built targets to link
	# against the DLL itself instead of the implib. 
	#set_target_properties(libh6n PROPERTIES IMPORTED_LOCATION  "$<TARGET_FILE:libh6n-capsule>")
	#set_target_properties(libh6n PROPERTIES INCLUDE_DIRECTORIES  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include")

else()
	# Create "interface" target which is header only
	add_library(libh6n-direct ALIAS libh6n-headers)
endif()

add_library(libh6n-static STATIC src/interfaces.cpp)
target_link_libraries(libh6n-static libh6n-headers)
target_compile_definitions(libh6n-static PUBLIC _H6N_IMPLEMENTS_STATIC)


if(SWIG_BINDINGS)
	add_subdirectory(swig)
else()
	add_library(libh6n SHARED src/interfaces.cpp)
	target_link_libraries(libh6n libh6n-headers)
	target_compile_definitions(libh6n PUBLIC _H6N_IMPLEMENTS_STATIC)
endif()



# Create install target
# This target creates the SDK bundle

include(CMakePackageConfigHelpers)

set(INSTALL_TARGETS
	libh6n-headers
	libh6n-capsule
	libh6n-agent
	libh6n-static
	libh6n)

	set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_config "${generated_dir}/${PROJECT_NAME}Targets.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")


file(GLOB INSTALL_HEADERS "include/libh6n/*.h")
install(FILES ${INSTALL_HEADERS} DESTINATION include/libh6n)

install(TARGETS ${INSTALL_TARGETS}
        EXPORT ${TARGETS_EXPORT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# Layout. This works for all platforms:
#   * <prefix>/lib*/cmake/<PROJECT-NAME>
#   * <prefix>/lib*/
#   * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")




EXPORT(TARGETS ${targets}
  FILE "${targets_config}")



configure_package_config_file(
    "Config.cmake.in"
    "${project_config}"
	INSTALL_DESTINATION "${config_install_dir}"
)



# Config
#   * <prefix>/lib/cmake/Foo/FooConfig.cmake
#   * <prefix>/lib/cmake/Foo/FooConfigVersion.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
#   * <prefix>/lib/cmake/Foo/FooTargets.cmake
install(
    EXPORT "${PROJECT_NAME}Targets"
	NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

