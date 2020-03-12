cmake_minimum_required( VERSION 3.2 )

set( SFGUI_MAJOR_VERSION 0 )
set( SFGUI_MINOR_VERSION 4 )
set( SFGUI_REVISION_VERSION 0 )
set( SFGUI_VERSION ${SFGUI_MAJOR_VERSION}.${SFGUI_MINOR_VERSION}.${SFGUI_REVISION_VERSION} )

project( SFGUI LANGUAGES CXX VERSION ${SFGUI_VERSION} )
set(TARGET SFGUI)

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

### USER INPUT ###

option( SFGUI_BUILD_SHARED_LIBS "Build shared library."                         ON )
set( BUILD_SHARED_LIBS ${SFGUI_BUILD_SHARED_LIBS} )
option( SFGUI_BUILD_EXAMPLES    "Build examples."                               ON)
option( SFGUI_BUILD_DOC         "Generate API documentation."                   OFF)
option( SFGUI_INCLUDE_FONT      "Include default font in library (DejaVuSans)." ON)
option( SFML_STATIC_LIBRARIES   "Do you want to link SFML statically?"          OFF)


# Find packages.
find_package( OpenGL REQUIRED )

if( NOT TARGET sfml-graphics )
    find_package( SFML 2.5 REQUIRED COMPONENTS graphics window system )
endif()

set( INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include/" )
set( SOURCE_PATH "${PROJECT_SOURCE_DIR}/src/" )

file(GLOB_RECURSE HPPS "${INCLUDE_PATH}/*.hpp")
file(GLOB_RECURSE INLS "${INCLUDE_PATH}/*.inl")
file(GLOB_RECURSE CPPS "${SOURCE_PATH}/*.cpp")

if( SFGUI_INCLUDE_FONT )
	list(APPEND CPPS "${SOURCE_PATH}/SFGUI/DejaVuSansFont.cpp")
endif()

# Set the library output directory
set( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" )

add_library( ${TARGET} ${CPPS} ${INLS} ${HPPS} )
target_include_directories( ${TARGET} PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/libELL/include" "${SOURCE_PATH}" )
target_include_directories( ${TARGET} PUBLIC $<BUILD_INTERFACE:${INCLUDE_PATH}> $<INSTALL_INTERFACE:include/> )

if( NOT SFGUI_BUILD_SHARED_LIBS )
	target_compile_definitions( ${TARGET} PUBLIC SFGUI_STATIC )
	set_target_properties( ${TARGET} PROPERTIES DEBUG_POSTFIX -s-d )
	set_target_properties( ${TARGET} PROPERTIES RELEASE_POSTFIX -s )
	set_target_properties( ${TARGET} PROPERTIES MINSIZEREL_POSTFIX -s )
else ()
	set_target_properties( ${TARGET} PROPERTIES DEBUG_POSTFIX -d )
endif ()

if( SFGUI_INCLUDE_FONT )
	target_compile_definitions( ${TARGET} PRIVATE SFGUI_INCLUDE_FONT )
endif()

target_link_libraries( ${TARGET} PUBLIC sfml-graphics sfml-window sfml-system ${OPENGL_gl_LIBRARY} )

# Tell the compiler to export when necessary.
set_target_properties( ${TARGET} PROPERTIES DEFINE_SYMBOL SFGUI_EXPORTS )

# Platform- and compiler-specific options.
if( WIN32 )
	option( SFGUI_STATIC_STD_LIBS "Use statically linked standard/runtime libraries? This option must match the one used for SFML." OFF)

	# Determine whether we're dealing with a TDM compiler or not
	if( CMAKE_COMPILER_IS_GNUCXX )
		execute_process( COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE GCC_COMPILER_VERSION )
		string( REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}" )
	endif()

	# Allow the static linking of the runtime libraries
	if( SFGUI_STATIC_STD_LIBS )
		if( SFGUI_BUILD_SHARED_LIBS )
			message( FATAL_ERROR "\n-> SFGUI_STATIC_STD_LIBS and SFGUI_BUILD_SHARED_LIBS are not compatible.\n-> They lead to multiple runtime environments which results in undefined behavior.\n" )
		else()
			target_compile_definitions( ${TARGET} PUBLIC SFML_STATIC )

			if( MSVC )
				foreach(flag
						CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
						CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
					if( ${flag} MATCHES "/MD" )
					string( REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}" )
					endif()
				endforeach()
			elseif( CMAKE_COMPILER_IS_GNUCXX AND NOT COMPILER_GCC_TDM )
				target_compile_options( ${TARGET} PUBLIC -static-libgcc -static-libstdc++ )
			endif()
		endif()
	elseif( COMPILER_GCC_TDM )
		target_compile_options( ${TARGET} PUBLIC -shared-libgcc -shared-libstdc++ )
	endif()

	target_compile_definitions( ${TARGET} PRIVATE WIN32 )

	if( CMAKE_COMPILER_IS_GNUCXX )
		if( SFGUI_BUILD_SHARED_LIBS )
			set_target_properties( ${TARGET} PROPERTIES PREFIX "" )
		endif()

		set_target_properties( ${TARGET} PROPERTIES IMPORT_SUFFIX ".a" )
	endif()

	set( SHARE_PATH "." )
	set( LIB_PATH "lib" )
elseif( APPLE )
	find_library( COREFOUNDATION_LIBRARY CoreFoundation )

	target_link_libraries( ${TARGET} PUBLIC ${COREFOUNDATION_LIBRARY} )
	set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/SFGUI" )
	set( LIB_PATH "lib" )
elseif( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
	find_package( X11 REQUIRED )
	target_link_libraries( ${TARGET} PUBLIC ${X11_LIBRARIES} )
	set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/SFGUI" )
	
	if( LIB_SUFFIX )
		set( LIB_PATH "lib${LIB_SUFFIX}" )
	else()
		set( LIB_PATH "lib" )
	endif()
else()
	set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/SFGUI" )
	set( LIB_PATH "lib" )
endif()

if( CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options( SFGUI PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic )
endif()

### EXAMPLES ###

if( SFGUI_BUILD_EXAMPLES )
	add_subdirectory( "examples" )
endif()

### DOCUMENTATION ###

if( SFGUI_BUILD_DOC )
	add_subdirectory( "doc" )
endif()

### INSTALL TARGETS ###

add_library(${PROJECT_NAME}::${TARGET} ALIAS ${TARGET})
install(
	TARGETS ${TARGET} EXPORT ${PROJECT_NAME}Targets
	RUNTIME DESTINATION bin COMPONENT bin
	LIBRARY DESTINATION "${LIB_PATH}" COMPONENT bin
	ARCHIVE DESTINATION "${LIB_PATH}" COMPONENT dev
)

install(
	DIRECTORY ${INCLUDE_PATH}
	DESTINATION include
)

install(
	FILES README.md AUTHORS.md LICENSE.md FONT.LICENSE.md CHANGELOG.md
	DESTINATION "${SHARE_PATH}"
)

install(
	EXPORT ${PROJECT_NAME}Targets NAMESPACE ${PROJECT_NAME}:: COMPONENT dev
	DESTINATION "${SHARE_PATH}/cmake/"
)

include( CMakePackageConfigHelpers )
write_basic_package_version_file( ${PROJECT_NAME}ConfigVersion.cmake
	VERSION ${SFGUI_VERSION}
	COMPATIBILITY SameMajorVersion )

configure_file(cmake/templates/config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION "${SHARE_PATH}/cmake/"
)

export(EXPORT ${PROJECT_NAME}Targets NAMESPACE ${PROJECT_NAME}::)
export(PACKAGE ${PROJECT_NAME})
