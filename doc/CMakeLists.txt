# find doxygen
if( MACOSX )
	# Add some path to search doxygen in more directories.
	set(
		ADDITIONAL_PATHS 
		/Developer/Applications/Doxygen.app/Contents/Resources 
		/Developer/Applications/Doxygen.app/Contents/MacOS 
		$ENV{HOME}/Applications/Doxygen.app/Contents/Resources 
		$ENV{HOME}/Applications/Doxygen.app/Contents/MacOS 
		$ENV{HOME}/Applications/Developer/Doxygen.app/Contents/Resources 
		$ENV{HOME}/Applications/Developer/Doxygen.app/Contents/MacOS
	)
 
	set( CMAKE_PROGRAM_PATH ${CMAKE_PROGRAM_PATH} ${ADDITIONAL_PATHS} )
endif()

find_package( Doxygen REQUIRED )

# set the input and output documentation paths
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR})
set(DOXYGEN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/doc)


# configure the source Doxyfile by copying it and replacing all @variables@
set(DOXYGEN_CONFIGURED_INPUT ${DOXYGEN_OUTPUT_DIR}/doxyfile)
configure_file(${DOXYGEN_INPUT_DIR}/doc/Doxyfile ${DOXYGEN_CONFIGURED_INPUT} @ONLY)

add_custom_target(
	doc ALL
	COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
	COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIGURED_INPUT}
	COMMAND ${CMAKE_COMMAND} -E echo "Done."
	WORKING_DIRECTORY ${DOXYGEN_INPUT_DIR}
)

if( WINDOWS )
	set( INSTALL_MISC_DIR "." )
elseif( UNIX )
	set( INSTALL_MISC_DIR "share/SFGUI" )
endif()

install(
	DIRECTORY "${PROJECT_BINARY_DIR}/doc/html"
	DESTINATION "${INSTALL_MISC_DIR}/doc"
	COMPONENT doc
)
