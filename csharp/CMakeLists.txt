find_program(cppsharp CppSharp.CLI)

add_custom_target(libh6n-csgen)
add_custom_command(TARGET libh6n-csgen
                   COMMAND ${cppsharp} -I. -DCppSharp -iln=H6Agent --output="${CMAKE_CURRENT_SOURCE_DIR}/" libh6n/libh6n.h 
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}>/../include)

if(MSVC)
	enable_language(CSharp)
	add_library(libh6n-cs SHARED)
	target_link_libraries(libh6n-cs libh6n-csgen)

	add_dependencies(h6n libh6n-cs)
endif()