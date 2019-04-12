find_package(SWIG 3.0.5 REQUIRED)
include(${SWIG_USE_FILE})



SET_SOURCE_FILES_PROPERTIES(interfaces.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(interfaces.i PROPERTIES SWIG_FLAGS "-includeall")
SET_SOURCE_FILES_PROPERTIES(interfaces.i PROPERTIES USE_LIBRARY_INCLUDE_DIRECTORIES TRUE)
swig_add_library(h6n_bindings
	TYPE STATIC
	LANGUAGE "csharp"
	SOURCES "interfaces.i")
set_target_properties (h6n_bindings PROPERTIES
  INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_include_directories(h6n_bindings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)