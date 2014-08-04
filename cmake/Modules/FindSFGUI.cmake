# This script locates the SFGUI library.
#
# USAGE
#
# By default, the dynamic version of SFGUI will be found. To find the static
# one instead, you must set the SFGUI_STATIC_LIBRARIES variable to TRUE before
# calling find_package( SFGUI ). In that case SFGUI_STATIC will also be defined
# by this script. Example:
#
# set( SFGUI_STATIC_LIBRARIES TRUE )
# find_package( SFGUI )
#
# If SFGUI is not installed in a standard path, you can use the SFGUIDIR or
# SFGUI_ROOT CMake (or environment) variables to tell CMake where to look for
# SFGUI.
#
#
# OUTPUT
#
# This script defines the following variables:
#   - SFGUI_LIBRARY_DEBUG:   the path to the debug library
#   - SFGUI_LIBRARY_RELEASE: the path to the release library
#   - SFGUI_LIBRARY:         the path to the library to link to
#   - SFGUI_FOUND:           true if the SFGUI library is found
#   - SFGUI_INCLUDE_DIR:     the path where SFGUI headers are located (the directory containing the SFGUI/Config.hpp file)
#
#
# EXAMPLE
#
# find_package( SFGUI REQUIRED )
# include_directories( ${SFGUI_INCLUDE_DIR} )
# add_executable( myapp ... )
# target_link_libraries( myapp ${SFGUI_LIBRARY} ... )

set( SFGUI_FOUND false )

if( SFGUI_STATIC_LIBRARIES )
	set( SFGUI_SUFFIX "-s" )
	add_definitions( -DSFGUI_STATIC )
else()
	set( SFGUI_SUFFIX "" )
endif()

find_path(
	SFGUI_INCLUDE_DIR
	SFGUI/Config.hpp
	PATH_SUFFIXES
		include
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
		${SFGUI_ROOT}
		$ENV{SFGUIDIR}
		$ENV{SFGUI_ROOT}
)

find_library(
	SFGUI_LIBRARY_RELEASE
	sfgui${SFGUI_SUFFIX}
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
		${SFGUI_ROOT}
		$ENV{SFGUIDIR}
		$ENV{SFGUI_ROOT}
)

find_library(
	SFGUI_LIBRARY_DEBUG
	sfgui${SFGUI_SUFFIX}-d
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
		${SFGUI_ROOT}
		$ENV{SFGUIDIR}
		$ENV{SFGUI_ROOT}
)

if( SFGUI_LIBRARY_RELEASE AND SFGUI_LIBRARY_DEBUG )
	set( SFGUI_LIBRARY debug ${SFGUI_LIBRARY_DEBUG} optimized ${SFGUI_LIBRARY_RELEASE} )
endif()

if( SFGUI_LIBRARY_RELEASE AND NOT SFGUI_LIBRARY_DEBUG )
	set( SFGUI_LIBRARY_DEBUG ${SFGUI_LIBRARY_RELEASE} )
	set( SFGUI_LIBRARY ${SFGUI_LIBRARY_RELEASE} )
endif()

if( SFGUI_LIBRARY_DEBUG AND NOT SFGUI_LIBRARY_RELEASE )
	set( SFGUI_LIBRARY_RELEASE ${SFGUI_LIBRARY_DEBUG} )
	set( SFGUI_LIBRARY ${SFGUI_LIBRARY_DEBUG} )
endif()

if( NOT SFGUI_INCLUDE_DIR OR NOT SFGUI_LIBRARY )
	if( SFGUI_FIND_REQUIRED )
		message( FATAL_ERROR "SFGUI not found." )
	elseif( NOT SFGUI_FIND_QUIETLY )
		message( "SFGUI not found." )
	endif()
else()
	set( SFGUI_FOUND true )
	if ( NOT SFGUI_FIND_QUIETLY )
		message( STATUS "SFGUI found: ${SFGUI_INCLUDE_DIR}" )
	endif()
endif()
