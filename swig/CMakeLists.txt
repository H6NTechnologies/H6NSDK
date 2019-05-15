find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_property(SOURCE interfaces.i PROPERTY CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(interfaces.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_library(libh6n
	TYPE SHARED
	LANGUAGE "csharp"
	SOURCES interfaces.i ../src/interfaces.cpp)
set_target_properties (libh6n PROPERTIES SWIG_USE_LIBRARY_INCLUDE_DIRECTORIES TRUE)

target_include_directories(libh6n PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_compile_definitions(libh6n PUBLIC _H6N_IMPLEMENTS_STATIC)
