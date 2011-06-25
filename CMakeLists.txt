cmake_minimum_required( VERSION 2.8 )
project( SFGUI )

find_package( SFML 2.0 REQUIRED )

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/" )

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
	${INC_DIR}/SFGUI/KeyValueLoader.hpp
	${INC_DIR}/SFGUI/Label.hpp
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
	${SRC_DIR}/Bin.cpp
	${SRC_DIR}/Box.cpp
	${SRC_DIR}/Button.cpp
	${SRC_DIR}/Container.cpp
	${SRC_DIR}/Context.cpp
	${SRC_DIR}/DragInfo.cpp
	${SRC_DIR}/Engine.cpp
	${SRC_DIR}/Engines/BREW.cpp
	${SRC_DIR}/KeyValueLoader.cpp
	${SRC_DIR}/Label.cpp
	${SRC_DIR}/RenderQueue.cpp
	${SRC_DIR}/ThemeLoader.cpp
	${SRC_DIR}/Widget.cpp
	${SRC_DIR}/Window.cpp
)

include_directories( ${INC_DIR} )
include_directories( ${SFML_INCLUDE_DIR} )

add_library( sfgui SHARED ${SOURCES} )

target_link_libraries( sfgui sfml-graphics sfml-window sfml-system )

install(
	TARGETS sfgui
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(
	DIRECTORY include
	DESTINATION .
)

install(
	FILES ${PROJECT_SOURCE_DIR}/cmake/FindSFGUI.cmake
	DESTINATION ${CMAKE_ROOT}/Modules/
)
