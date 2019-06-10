add_executable(libh6nTest agent.cpp)
target_link_libraries(libh6nTest libh6n-static H6Vendor::gtest_main H6Vendor::gmock)
gtest_discover_tests(libh6nTest)
add_test(libh6nTest libh6nTest)

add_custom_command(TARGET libh6nTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:H6Agent> $<TARGET_FILE_DIR:libh6nTest>
)

IF (WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# Add SUBSYSTEM flag to linker so that the binary will run on Windows XP
	set_target_properties(libh6nTest PROPERTIES LINK_FLAGS "-Xlinker \"/SUBSYSTEM:CONSOLE,5.01\"")
ENDIF()