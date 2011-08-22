function( build_sample SAMPLE_NAME SOURCES )
	add_executable( ${SAMPLE_NAME} ${SOURCES} )
	target_link_libraries( ${SAMPLE_NAME} sfgui sfml-graphics sfml-window sfml-system )

	add_custom_command( TARGET ${SAMPLE_NAME} COMMAND cmake ARGS -E copy_if_different ${PROJECT_BINARY_DIR}/samples/${SAMPLE_NAME}${BIN_EXTENSION} ${PROJECT_SOURCE_DIR}/samples/${SAMPLE_NAME}${BIN_EXTENSION} )
endfunction()
