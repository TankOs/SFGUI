function( build_example EXAMPLE_NAME SOURCES )
	add_executable( ${EXAMPLE_NAME} ${SOURCES} )
	target_link_libraries( ${EXAMPLE_NAME} sfgui ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} )

	add_custom_command( TARGET ${EXAMPLE_NAME} COMMAND cmake ARGS -E copy_if_different ${PROJECT_BINARY_DIR}/examples/${EXAMPLE_NAME}${BIN_EXTENSION} ${PROJECT_SOURCE_DIR}/examples/${EXAMPLE_NAME}${BIN_EXTENSION} )
endfunction()
