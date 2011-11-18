cmake_minimum_required( VERSION 2.8 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE )
endif()

project( SFGUI )

find_package( SFML 2.0 REQUIRED GRAPHICS WINDOW SYSTEM )
find_package( OpenGL REQUIRED )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	set( WINDOWS 1 )
endif()

if( CMAKE_COMPILER_IS_GNUCXX )
	set( COMPILER_GCC 1 )
elseif( MSVC_VERSION )
	set( COMPILER_MSVC 1 )
endif()

set( SFGUI_STATIC false CACHE BOOL "Build static library." )
set( SFGUI_BUILD_EXAMPLES true CACHE BOOL "Build SFGUI examples." )

if( WINDOWS )
	set( BIN_EXTENSION ".exe" )
	set( SFML_STATIC false CACHE BOOL "Was SFML built as a static library?" )
	set( SHARE_TARGET_DIR . )
else()
	set( SHARE_TARGET_DIR share/SFGUI )
endif()

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/" )

set( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" )

if( SFGUI_STATIC )
	add_definitions( -D SFGUI_STATIC )
endif()

if( SFML_STATIC )
	add_definitions( -D SFML_STATIC )
endif()

if( COMPILER_GCC )
	if( NOT CMAKE_CXX_FLAGS )
		set( CMAKE_CXX_FLAGS "--std=c++0x -Wall -Wextra -Wshadow -Wconversion -pedantic" CACHE STRING "C++ compiler flags" FORCE )
		set( CMAKE_C_FLAGS "--std=c++0x -Wall -Wextra -Wshadow -Wconversion -pedantic" CACHE STRING "C compiler flags" FORCE )
	endif()

	if( NOT CMAKE_CXX_FLAGS_DEBUG )
		set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "C++ compiler flags (debug)" FORCE )
		set( CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE STRING "C++ compiler flags (debug)" FORCE )
	endif()
endif()

set(
	SOURCES
	${INC_DIR}/SFGUI/Adjustment.hpp
	${INC_DIR}/SFGUI/Bin.hpp
	${INC_DIR}/SFGUI/Box.hpp
	${INC_DIR}/SFGUI/Button.hpp
	${INC_DIR}/SFGUI/CheckButton.hpp
	${INC_DIR}/SFGUI/Config.hpp
	${INC_DIR}/SFGUI/Container.hpp
	${INC_DIR}/SFGUI/Context.hpp
	${INC_DIR}/SFGUI/CullingTarget.hpp
	${INC_DIR}/SFGUI/Desktop.hpp
	${INC_DIR}/SFGUI/Desktop.inl
	${INC_DIR}/SFGUI/Engine.hpp
	${INC_DIR}/SFGUI/Engine.inl
	${INC_DIR}/SFGUI/Engines/BREW.hpp
	${INC_DIR}/SFGUI/Entry.hpp
	${INC_DIR}/SFGUI/FastDelegate.hpp
	${INC_DIR}/SFGUI/FileResourceLoader.hpp
	${INC_DIR}/SFGUI/Label.hpp
	${INC_DIR}/SFGUI/NonCopyable.hpp
	${INC_DIR}/SFGUI/Object.hpp
	${INC_DIR}/SFGUI/Parsers/ThemeParser/Grammar.hpp
	${INC_DIR}/SFGUI/Parsers/ThemeParser/Parse.hpp
	${INC_DIR}/SFGUI/ProgressBar.hpp
	${INC_DIR}/SFGUI/Range.hpp
	${INC_DIR}/SFGUI/RenderQueue.hpp
	${INC_DIR}/SFGUI/ResourceLoader.hpp
	${INC_DIR}/SFGUI/ResourceManager.hpp
	${INC_DIR}/SFGUI/ResourceManager.inl
	${INC_DIR}/SFGUI/Scale.hpp
	${INC_DIR}/SFGUI/Scrollbar.hpp
	${INC_DIR}/SFGUI/ScrolledWindow.hpp
	${INC_DIR}/SFGUI/Selector.hpp
	${INC_DIR}/SFGUI/Signal.hpp
	${INC_DIR}/SFGUI/Signal.inl
	${INC_DIR}/SFGUI/Table.hpp
	${INC_DIR}/SFGUI/TableCell.hpp
	${INC_DIR}/SFGUI/TableOptions.hpp
	${INC_DIR}/SFGUI/ToggleButton.hpp
	${INC_DIR}/SFGUI/Viewport.hpp
	${INC_DIR}/SFGUI/Widget.hpp
	${INC_DIR}/SFGUI/Window.hpp
	${SRC_DIR}/SFGUI/Adjustment.cpp
	${SRC_DIR}/SFGUI/Bin.cpp
	${SRC_DIR}/SFGUI/Box.cpp
	${SRC_DIR}/SFGUI/Button.cpp
	${SRC_DIR}/SFGUI/CheckButton.cpp
	${SRC_DIR}/SFGUI/Container.cpp
	${SRC_DIR}/SFGUI/Context.cpp
	${SRC_DIR}/SFGUI/CullingTarget.cpp
	${SRC_DIR}/SFGUI/Desktop.cpp
	${SRC_DIR}/SFGUI/Engine.cpp
	${SRC_DIR}/SFGUI/Engines/BREW.cpp
	${SRC_DIR}/SFGUI/Entry.cpp
	${SRC_DIR}/SFGUI/FileResourceLoader.cpp
	${SRC_DIR}/SFGUI/Label.cpp
	${SRC_DIR}/SFGUI/NonCopyable.cpp
	${SRC_DIR}/SFGUI/Object.cpp
	${SRC_DIR}/SFGUI/Parsers/ThemeParser/Grammar.cpp
	${SRC_DIR}/SFGUI/Parsers/ThemeParser/GrammarPredicates.cpp
	${SRC_DIR}/SFGUI/Parsers/ThemeParser/GrammarSelector.cpp
	${SRC_DIR}/SFGUI/Parsers/ThemeParser/GrammarSimpleSelector.cpp
	${SRC_DIR}/SFGUI/Parsers/ThemeParser/GrammarStatement.cpp
	${SRC_DIR}/SFGUI/Parsers/ThemeParser/GrammarToken.cpp
	${SRC_DIR}/SFGUI/Parsers/ThemeParser/Parse.cpp
	${SRC_DIR}/SFGUI/ProgressBar.cpp
	${SRC_DIR}/SFGUI/Range.cpp
	${SRC_DIR}/SFGUI/RenderQueue.cpp
	${SRC_DIR}/SFGUI/ResourceLoader.cpp
	${SRC_DIR}/SFGUI/ResourceManager.cpp
	${SRC_DIR}/SFGUI/Scale.cpp
	${SRC_DIR}/SFGUI/Scrollbar.cpp
	${SRC_DIR}/SFGUI/ScrolledWindow.cpp
	${SRC_DIR}/SFGUI/Selector.cpp
	${SRC_DIR}/SFGUI/Signal.cpp
	${SRC_DIR}/SFGUI/Table.cpp
	${SRC_DIR}/SFGUI/TableCell.cpp
	${SRC_DIR}/SFGUI/TableOptions.cpp
	${SRC_DIR}/SFGUI/ToggleButton.cpp
	${SRC_DIR}/SFGUI/Viewport.cpp
	${SRC_DIR}/SFGUI/Widget.cpp
	${SRC_DIR}/SFGUI/Window.cpp
)

include_directories( ${INC_DIR} )
include_directories( ${SFML_INCLUDE_DIR} )
include_directories( ${OPENGL_INCLUDE_DIR} )
include_directories( "${PROJECT_SOURCE_DIR}/extlibs/libELL/include" )

if( NOT SFGUI_STATIC )
	add_library( sfgui SHARED ${SOURCES} )

	set_target_properties( sfgui PROPERTIES DEBUG_POSTFIX -d )

	if ( WINDOWS AND COMPILER_GCC )
		set_target_properties( sfgui PROPERTIES PREFIX "" )
		set_target_properties( sfgui PROPERTIES IMPORT_SUFFIX ".a" )
	endif()
else()
	add_library( sfgui ${SOURCES} )

	set_target_properties( sfgui PROPERTIES DEBUG_POSTFIX -s-d )
	set_target_properties( sfgui PROPERTIES RELEASE_POSTFIX -s )
endif()

set_target_properties( sfgui PROPERTIES COMPILE_DEFINITIONS SFGUI_EXPORTS )

target_link_libraries( sfgui ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${OPENGL_gl_LIBRARY} )

### EXAMPLES ###

if( SFGUI_BUILD_EXAMPLES )
	add_subdirectory( "examples" )
endif()

### INSTALL TARGETS ###

install(
	TARGETS sfgui
	RUNTIME DESTINATION bin COMPONENT bin
	LIBRARY DESTINATION lib COMPONENT bin
	ARCHIVE DESTINATION lib COMPONENT dev
)

install(
	DIRECTORY include
	DESTINATION .
)

install(
	FILES README AUTHORS LICENSE
	DESTINATION ${SHARE_TARGET_DIR}
)
