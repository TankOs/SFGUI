# This script locates the SFGUI library
# ------------------------------------
#
# Usage
# -----
#
# You can enforce a specific version, one of either MAJOR.MINOR.REVISION,
# MAJOR.MINOR or only MAJOR. If nothing is specified, the version won't
# be checked i.e. any version will be accepted. SFGUI does not consist of
# multiple components, so specifying COMPONENTS is not required.
#
# Example:
#   find_package( SFGUI )       // no specific version
#   find_package( SFGUI 0.2 )   // any 0.2 version
#   find_package( SFGUI 0.2.3 ) // version 0.2.3 or greater
#
# By default, the dynamic version of SFGUI will be found. To find the static
# version instead, you must set the SFGUI_STATIC_LIBRARIES variable to TRUE
# before calling find_package( SFGUI ). In that case, SFGUI_STATIC will also be
# defined by this script.
#
# Example:
#   set( SFGUI_STATIC_LIBRARIES TRUE )
#   find_package( SFGUI )
#
# Since you have to link all of SFGUI's dependencies when you link SFGUI
# statically, the variable SFGUI_DEPENDENCIES is also defined. See below
# for a detailed description.
#
# On Mac OS X if SFGUI_STATIC_LIBRARIES is not set to TRUE then by default CMake
# will search for frameworks unless CMAKE_FIND_FRAMEWORK is set to "NEVER".
# Please refer to CMake documentation for more details.
# Moreover, keep in mind that SFGUI frameworks are only available as release
# libraries unlike dylibs which are available for both release and debug modes.
#
# If SFGUI is not installed in a standard path, you can use the SFGUI_ROOT
# CMake (or environment) variable to tell CMake where to look for SFGUI.
#
# Output
# ------
#
# This script defines the following variables:
#   - SFGUI_LIBRARY_DEBUG:   the path to the debug library
#   - SFGUI_LIBRARY_RELEASE: the path to the release library
#   - SFGUI_LIBRARY:         the path to the library to link to
#   - SFGUI_FOUND:           TRUE if the SFGUI library is found
#   - SFGUI_INCLUDE_DIR:     the path where SFGUI headers are located (the directory containing the SFGUI/Config.hpp file)
#   - SFGUI_DEPENDENCIES:    the list of libraries SFGUI depends on, in case of static linking
#
# Example (dynamic linking):
#   find_package( SFGUI REQUIRED )
#   include_directories( ${SFGUI_INCLUDE_DIR} )
#   add_executable( myapp ... )
#   target_link_libraries( myapp ${SFGUI_LIBRARY} ... )
#
# Example (static linking):
#   set( SFGUI_STATIC_LIBRARIES TRUE )
#   find_package( SFGUI REQUIRED )
#   include_directories( ${SFGUI_INCLUDE_DIR} )
#   add_executable( myapp ... )
#   target_link_libraries( myapp ${SFGUI_LIBRARY} ${SFGUI_DEPENDENCIES} ... )

if( SFGUI_STATIC_LIBRARIES )
	add_definitions( -DSFGUI_STATIC )
endif()

set(
	SFGUI_SEARCH_PATHS
	"${SFGUI_ROOT}"
	"$ENV{SFGUI_ROOT}"
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt
)

find_path(
	SFGUI_INCLUDE_DIR
	SFGUI/Config.hpp
	PATH_SUFFIXES
		include
	PATHS
		${SFGUI_SEARCH_PATHS}
)

set( SFGUI_VERSION_OK true )
if( SFGUI_FIND_VERSION AND SFGUI_INCLUDE_DIR )
	file( READ "${SFGUI_INCLUDE_DIR}/SFGUI/Config.hpp" SFGUI_CONFIG_HPP )
	
	string( REGEX MATCH ".*#define SFGUI_MAJOR_VERSION ([0-9]+).*#define SFGUI_MINOR_VERSION ([0-9]+).*#define SFGUI_REVISION_VERSION ([0-9]+).*" SFGUI_CONFIG_HPP "${SFGUI_CONFIG_HPP}" )
	string( REGEX REPLACE ".*#define SFGUI_MAJOR_VERSION ([0-9]+).*" "\\1" SFGUI_VERSION_MAJOR "${SFGUI_CONFIG_HPP}" )
	string( REGEX REPLACE ".*#define SFGUI_MINOR_VERSION ([0-9]+).*" "\\1" SFGUI_VERSION_MINOR "${SFGUI_CONFIG_HPP}" )
	string( REGEX REPLACE ".*#define SFGUI_REVISION_VERSION ([0-9]+).*" "\\1" SFGUI_VERSION_PATCH "${SFGUI_CONFIG_HPP}" )
	
	math( EXPR SFGUI_REQUESTED_VERSION "${SFGUI_FIND_VERSION_MAJOR} * 10000 + ${SFGUI_FIND_VERSION_MINOR} * 100 + ${SFGUI_FIND_VERSION_PATCH}" )

	if( SFGUI_VERSION_MAJOR OR SFGUI_VERSION_MINOR OR SFGUI_VERSION_PATCH )
		math( EXPR SFGUI_VERSION "${SFGUI_VERSION_MAJOR} * 10000 + ${SFGUI_VERSION_MINOR} * 100 + ${SFGUI_VERSION_PATCH}" )

		if( SFGUI_VERSION LESS SFGUI_REQUESTED_VERSION )
			set( SFGUI_VERSION_OK false )
		endif()
	else()
		# SFGUI version is < 0.3.0
		if( SFGUI_REQUESTED_VERSION GREATER 300 )
			set( SFGUI_VERSION_OK false )
			set( SFGUI_VERSION_MAJOR 0 )
			set( SFGUI_VERSION_MINOR x )
			set( SFGUI_VERSION_PATCH y )
		endif()
	endif()
endif()

find_library(
	SFGUI_LIBRARY_DYNAMIC_RELEASE
	NAMES
		sfgui
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${SFGUI_SEARCH_PATHS}
)

find_library(
	SFGUI_LIBRARY_DYNAMIC_DEBUG
	NAMES
		sfgui-d
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${SFGUI_SEARCH_PATHS}
)

find_library(
	SFGUI_LIBRARY_STATIC_RELEASE
	NAMES
		sfgui-s
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${SFGUI_SEARCH_PATHS}
)

find_library(
	SFGUI_LIBRARY_STATIC_DEBUG
	NAMES
		sfgui-s-d
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${SFGUI_SEARCH_PATHS}
)

if( SFGUI_STATIC_LIBRARIES )
	if( SFGUI_LIBRARY_STATIC_RELEASE )
		set( SFGUI_LIBRARY_RELEASE "${SFGUI_LIBRARY_STATIC_RELEASE}" )
	endif()
	if( SFGUI_LIBRARY_STATIC_DEBUG )
		set( SFGUI_LIBRARY_DEBUG "${SFGUI_LIBRARY_STATIC_DEBUG}" )
	endif()
else()
	if( SFGUI_LIBRARY_DYNAMIC_RELEASE )
		set( SFGUI_LIBRARY_RELEASE "${SFGUI_LIBRARY_DYNAMIC_RELEASE}" )
	endif()
	if( SFGUI_LIBRARY_DYNAMIC_DEBUG )
		set( SFGUI_LIBRARY_DEBUG "${SFGUI_LIBRARY_DYNAMIC_DEBUG}" )
	endif()
endif()

mark_as_advanced(
	SFGUI_LIBRARY_STATIC_RELEASE
	SFGUI_LIBRARY_STATIC_DEBUG
	SFGUI_LIBRARY_DYNAMIC_RELEASE
	SFGUI_LIBRARY_DYNAMIC_DEBUG
)

if( SFGUI_LIBRARY_RELEASE OR SFGUI_LIBRARY_DEBUG )
	if( SFGUI_LIBRARY_RELEASE AND SFGUI_LIBRARY_DEBUG )
		set( SFGUI_LIBRARY debug "${SFGUI_LIBRARY_DEBUG}" optimized "${SFGUI_LIBRARY_RELEASE}" )
	elseif( SFGUI_LIBRARY_DEBUG AND NOT SFGUI_LIBRARY_RELEASE )
		set( SFGUI_LIBRARY_RELEASE "${SFGUI_LIBRARY_DEBUG}" )
		set( SFGUI_LIBRARY "${SFGUI_LIBRARY_DEBUG}" )
	elseif( SFGUI_LIBRARY_RELEASE AND NOT SFGUI_LIBRARY_DEBUG )
		set( SFGUI_LIBRARY_DEBUG "${SFGUI_LIBRARY_RELEASE}" )
		set( SFGUI_LIBRARY "${SFGUI_LIBRARY_RELEASE}" )
	endif()

	set( SFGUI_FOUND true )
else()
	set( SFGUI_LIBRARY "" )
	set( SFGUI_FOUND false )
endif()

mark_as_advanced(
	SFGUI_LIBRARY
	SFGUI_LIBRARY_RELEASE
	SFGUI_LIBRARY_DEBUG
)

if( SFGUI_STATIC_LIBRARIES )
	set( SFGUI_DEPENDENCIES )
	set( SFGUI_MISSING_DEPENDENCIES )

	if( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
		find_library( X11_LIBRARY NAMES X11 PATHS ${SFGUI_SEARCH_PATHS} PATH_SUFFIXES lib )
		
		if( ${X11_LIBRARY} STREQUAL "X11_LIBRARY-NOTFOUND" )
			unset( X11_LIBRARY )
			set( SFGUI_MISSING_DEPENDENCIES "${SFGUI_MISSING_DEPENDENCIES} X11" )
		endif()
	endif()

	if( "${CMAKE_SYSTEM_NAME}" MATCHES "Windows" )
		set( SFGUI_DEPENDENCIES ${SFGUI_DEPENDENCIES} "opengl32" )
	elseif( ( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" ) OR ( "${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD" ) )
		set( SFGUI_DEPENDENCIES ${SFGUI_DEPENDENCIES} "GL" ${X11_LIBRARY} )
	elseif( "${CMAKE_SYSTEM_NAME}" MATCHES "Darwin" )
		set( SFGUI_DEPENDENCIES ${SFGUI_DEPENDENCIES} "-framework OpenGL -framework Foundation" )
	endif()
endif()

if( NOT SFGUI_INCLUDE_DIR OR NOT SFGUI_LIBRARY )
	set( SFGUI_FOUND false )
endif()

if( NOT SFGUI_FOUND )
	set( FIND_SFGUI_ERROR "SFGUI not found." )
elseif( NOT SFGUI_VERSION_OK )
	set( FIND_SFGUI_ERROR "SFGUI found but version too low, requested: ${SFGUI_FIND_VERSION}, found: ${SFGUI_VERSION_MAJOR}.${SFGUI_VERSION_MINOR}.${SFGUI_VERSION_PATCH}" )
	set( SFGUI_FOUND false )
elseif( SFGUI_STATIC_LIBRARIES AND SFGUI_MISSING_DEPENDENCIES )
	set( FIND_SFGUI_ERROR "SFGUI found but some of its dependencies are missing: ${SFGUI_MISSING_DEPENDENCIES}" )
	set( SFGUI_FOUND false )
endif()

if( NOT SFGUI_FOUND )
	if( SFGUI_FIND_REQUIRED )
		message( FATAL_ERROR "${FIND_SFGUI_ERROR}" )
	elseif( NOT SFGUI_FIND_QUIETLY )
		message( "${FIND_SFGUI_ERROR}" )
	endif()
else()
	if( SFGUI_FIND_VERSION )
		message( STATUS "Found SFGUI version ${SFGUI_VERSION_MAJOR}.${SFGUI_VERSION_MINOR}.${SFGUI_VERSION_PATCH} in ${SFGUI_INCLUDE_DIR}" )
	else()
		message( STATUS "Found SFGUI in ${SFGUI_INCLUDE_DIR}" )
	endif()
endif()
