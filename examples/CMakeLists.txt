cmake_minimum_required( VERSION 2.8 )

function( build_example SAMPLE_NAME SOURCES )
	add_executable( ${SAMPLE_NAME} ${SOURCES} )
	target_link_libraries( ${SAMPLE_NAME} sfgui ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY} )

	install(
		TARGETS ${SAMPLE_NAME}
		RUNTIME DESTINATION "${SHARE_PATH}/examples" COMPONENT examples
	)

	install(
		FILES ${SOURCES}
		DESTINATION "${SHARE_PATH}/examples" COMPONENT examples
	)
endfunction()

build_example( "HelloWorld" "HelloWorld.cpp" )
build_example( "Window" "Window.cpp" )
build_example( "Label" "Label.cpp" )
build_example( "Box" "Box.cpp" )
build_example( "Entry" "Entry.cpp" )
build_example( "Range" "Range.cpp" )
build_example( "Desktop" "Desktop.cpp" )
build_example( "Signals" "Signals.cpp" )
build_example( "OpenGL" "OpenGL.cpp" )
build_example( "ComboBox" "ComboBox.cpp" )
build_example( "Notebook" "Notebook.cpp" )
build_example( "GuessMyNumber" "GuessMyNumber.cpp" )
build_example( "Multiview" "Multiview.cpp" )
build_example( "Image" "Image.cpp" )
build_example( "Layout" "Layout.cpp" )
build_example( "ScrolledWindowViewport" "ScrolledWindowViewport.cpp" )
build_example( "Spinner" "Spinner.cpp" )
build_example( "Table" "Table.cpp" )
build_example( "Buttons" "Buttons.cpp" )
build_example( "ProgressBar" "ProgressBar.cpp" )
build_example( "SpinButton" "SpinButton.cpp" )
build_example( "Canvas" "Canvas.cpp" )
build_example( "CustomWidget" "CustomWidget.cpp" )
build_example( "SFGUI-Test" "Test.cpp" )

# Copy data directory to build cache directory to be able to run examples from
# there. Useful for testing stuff.
# Don't try to copy if the directories are the same.
if( NOT ( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" ) )
	add_custom_command(
		TARGET "SFGUI-Test"
		COMMAND "${CMAKE_COMMAND}"
		ARGS -E copy_directory "${PROJECT_SOURCE_DIR}/examples/data" "${PROJECT_BINARY_DIR}/examples/data"
	)
	
	add_custom_command(
		TARGET "Image"
		COMMAND "${CMAKE_COMMAND}"
		ARGS -E copy_directory "${PROJECT_SOURCE_DIR}/examples/data" "${PROJECT_BINARY_DIR}/examples/data"
	)
	
	add_custom_command(
		TARGET "Canvas"
		COMMAND "${CMAKE_COMMAND}"
		ARGS -E copy_directory "${PROJECT_SOURCE_DIR}/examples/data" "${PROJECT_BINARY_DIR}/examples/data"
	)
endif()

install(
	DIRECTORY data
	DESTINATION "${SHARE_PATH}/examples"
)
