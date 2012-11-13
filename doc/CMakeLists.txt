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

add_custom_target(
	doc ALL
	COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
	COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_SOURCE_DIR}/doc/Doxyfile"
	COMMAND ${CMAKE_COMMAND} -E echo "Done."
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
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
