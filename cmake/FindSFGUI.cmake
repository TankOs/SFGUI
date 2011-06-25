cmake_minimum_required( VERSION 2.8 )

set( SFGUI_FOUND FALSE )

find_path(
	SFGUI_INCLUDE_DIR
	SFGUI/Config.hpp
	PATH_SUFFIXES
		include
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
)

find_library(
	SFGUI_LIBRARY
	sfgui
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
)

if( NOT SFGUI_INCLUDE_DIR OR NOT SFGUI_LIBRARY )
	message( FATAL_ERROR "SFGUI not found." )
else()
	message( "SFGUI found: ${SFGUI_INCLUDE_DIR}" )
endif()
