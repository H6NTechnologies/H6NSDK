find_program(cppsharp CppSharp.CLI)

file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR} native_output)

add_custom_command(
                   COMMAND ${cppsharp} -I. -DCppSharp -iln=H6Agent --output="${native_output}/" libh6n/libh6n.h 
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}>/../include
					OUTPUT libh6n.cs Std.cs)

if(MSVC)
	enable_language(CSharp)
	add_library(libh6n-cs SHARED h6n.cs libh6n.cs Std.cs)
	target_compile_options(libh6n-cs PRIVATE "/langversion:4")
	target_compile_options(libh6n-cs PUBLIC "/unsafe")

endif()
