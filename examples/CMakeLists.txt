cmake_minimum_required( VERSION 2.8 )

function( build_example SAMPLE_NAME SOURCES )
	add_executable( ${SAMPLE_NAME} ${SOURCES} )
	target_link_libraries( ${SAMPLE_NAME} sfgui ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY} )

	add_custom_command( TARGET ${SAMPLE_NAME} COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${PROJECT_BINARY_DIR}/examples/${CMAKE_CFG_INTDIR}/${SAMPLE_NAME}${BIN_EXTENSION} ${PROJECT_SOURCE_DIR}/examples/${SAMPLE_NAME}${BIN_EXTENSION} )

	install(
		TARGETS ${SAMPLE_NAME}
		RUNTIME DESTINATION ${SHARE_TARGET_DIR}/examples COMPONENT examples
	)
endfunction()

if( NOT SKIP_ADDT_EXAMPLES )
	build_example( "Window" "Window.cpp" )
	build_example( "Label" "Label.cpp" )
	build_example( "Button" "Button.cpp" )
	build_example( "Box" "Box.cpp" )
	build_example( "Entry" "Entry.cpp" )
	build_example( "Range" "Range.cpp" )
	build_example( "Desktop" "Desktop.cpp" )
	build_example( "Signals" "Signals.cpp" )
	build_example( "OpenGL" "OpenGL.cpp" )

	build_example( "GuessMyNumber" "GuessMyNumber.cpp" )
endif()

build_example( "test" "Test.cpp" )

install(
	DIRECTORY data
	DESTINATION ${SHARE_TARGET_DIR}/examples
)
