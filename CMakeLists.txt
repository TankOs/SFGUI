cmake_minimum_required( VERSION 2.8 )
project( SFGUI )

find_package( SFML 2.0 REQUIRED GRAPHICS WINDOW SYSTEM )
find_package( Boost 1.46 REQUIRED )

set( SFGUI_STATIC false CACHE BOOL "Build static library." )
set( BUILD_SAMPLES true CACHE BOOL "Build SFGUI sample applications." )

if( WINDOWS )
	set( BIN_EXTENSION ".exe" )
endif()

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/" )

add_definitions( -D SFGUI_EXPORTS )

if( SFGUI_STATIC )
	add_definitions( -D SFGUI_STATIC )
endif()

set(
	SOURCES
	${INC_DIR}/SFGUI/Bin.hpp
	${INC_DIR}/SFGUI/Box.hpp
	${INC_DIR}/SFGUI/Button.hpp
	${INC_DIR}/SFGUI/Config.hpp
	${INC_DIR}/SFGUI/Container.hpp
	${INC_DIR}/SFGUI/Context.hpp
	${INC_DIR}/SFGUI/DragInfo.hpp
	${INC_DIR}/SFGUI/Engine.hpp
	${INC_DIR}/SFGUI/Engine.inl
	${INC_DIR}/SFGUI/Engines/BREW.hpp
	${INC_DIR}/SFGUI/Entry.hpp
	${INC_DIR}/SFGUI/KeyValueLoader.hpp
	${INC_DIR}/SFGUI/Label.hpp
	${INC_DIR}/SFGUI/ListBox.hpp
	${INC_DIR}/SFGUI/RenderQueue.hpp
	${INC_DIR}/SFGUI/Signal.hpp
	${INC_DIR}/SFGUI/Signal.inl
	${INC_DIR}/SFGUI/SlotResult.hpp
	${INC_DIR}/SFGUI/SlotResult.inl
	${INC_DIR}/SFGUI/ThemeLoader.hpp
	${INC_DIR}/SFGUI/ThemeLoader.inl
	${INC_DIR}/SFGUI/Widget.hpp
	${INC_DIR}/SFGUI/Widget.inl
	${INC_DIR}/SFGUI/Window.hpp
	${SRC_DIR}/Entry.cpp
	${SRC_DIR}/SFGUI/Bin.cpp
	${SRC_DIR}/SFGUI/Box.cpp
	${SRC_DIR}/SFGUI/Button.cpp
	${SRC_DIR}/SFGUI/Container.cpp
	${SRC_DIR}/SFGUI/Context.cpp
	${SRC_DIR}/SFGUI/DragInfo.cpp
	${SRC_DIR}/SFGUI/Engine.cpp
	${SRC_DIR}/SFGUI/Engines/BREW.cpp
	${SRC_DIR}/SFGUI/KeyValueLoader.cpp
	${SRC_DIR}/SFGUI/Label.cpp
	${SRC_DIR}/SFGUI/RenderQueue.cpp
	${SRC_DIR}/SFGUI/ThemeLoader.cpp
	${SRC_DIR}/SFGUI/Widget.cpp
	${SRC_DIR}/SFGUI/Window.cpp
)

include_directories( ${INC_DIR} )
include_directories( ${SFML_INCLUDE_DIR} )
include_directories( ${Boost_INCLUDE_DIR} )

if( NOT SFGUI_STATIC )
	add_library( sfgui SHARED ${SOURCES} )
else()
	add_library( sfgui ${SOURCES} )
endif()

target_link_libraries( sfgui ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY} )

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
	FILES ${PROJECT_SOURCE_DIR}/cmake/FindSFGUI.cmake
	DESTINATION ${CMAKE_ROOT}/Modules/
)

if( BUILD_SAMPLES )
	add_subdirectory( "samples" )
endif()
