cmake_minimum_required( VERSION 2.8 )
project( SFGUI )

set( SFGUI_MAJOR_VERSION 0 )
set( SFGUI_MINOR_VERSION 3 )
set( SFGUI_REVISION_VERSION 0 )

### USER INPUT ###

set( SFGUI_BUILD_SHARED_LIBS true CACHE BOOL "Build shared library." )
set( SFGUI_BUILD_EXAMPLES true CACHE BOOL "Build examples." )
set( SFGUI_BUILD_DOC false CACHE BOOL "Generate API documentation." )
set( SFGUI_INCLUDE_FONT true CACHE BOOL "Include default font in library (DejaVuSans)." )
set( SFML_STATIC_LIBRARIES false CACHE BOOL "Do you want to link SFML statically?" )

# Tell CMake where to find additional find modules
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/" )

# Find packages.
find_package( OpenGL REQUIRED )
find_package( SFML 2.3 REQUIRED COMPONENTS graphics window system )

# Find X11 for glX on Linux, checking for UNIX would match other UNIX systems as well
if( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
	find_package( X11 REQUIRED )
endif()

if( SFGUI_INCLUDE_FONT )
	add_definitions( -DSFGUI_INCLUDE_FONT )
endif()

set( INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include" )
set( SOURCE_PATH "${PROJECT_SOURCE_DIR}/src" )

set(
	SOURCES
	"${INCLUDE_PATH}/SFGUI/Adjustment.hpp"
	"${INCLUDE_PATH}/SFGUI/Alignment.hpp"
	"${INCLUDE_PATH}/SFGUI/Bin.hpp"
	"${INCLUDE_PATH}/SFGUI/Box.hpp"
	"${INCLUDE_PATH}/SFGUI/Button.hpp"
	"${INCLUDE_PATH}/SFGUI/Canvas.hpp"
	"${INCLUDE_PATH}/SFGUI/CheckButton.hpp"
	"${INCLUDE_PATH}/SFGUI/ComboBox.hpp"
	"${INCLUDE_PATH}/SFGUI/Config.hpp"
	"${INCLUDE_PATH}/SFGUI/Container.hpp"
	"${INCLUDE_PATH}/SFGUI/Context.hpp"
	"${INCLUDE_PATH}/SFGUI/Desktop.hpp"
	"${INCLUDE_PATH}/SFGUI/Desktop.inl"
	"${INCLUDE_PATH}/SFGUI/Engine.hpp"
	"${INCLUDE_PATH}/SFGUI/Engine.inl"
	"${INCLUDE_PATH}/SFGUI/Engines/BREW.hpp"
	"${INCLUDE_PATH}/SFGUI/Entry.hpp"
	"${INCLUDE_PATH}/SFGUI/FileResourceLoader.hpp"
	"${INCLUDE_PATH}/SFGUI/Fixed.hpp"
	"${INCLUDE_PATH}/SFGUI/Frame.hpp"
	"${INCLUDE_PATH}/SFGUI/Image.hpp"
	"${INCLUDE_PATH}/SFGUI/Label.hpp"
	"${INCLUDE_PATH}/SFGUI/Misc.hpp"
	"${INCLUDE_PATH}/SFGUI/Notebook.hpp"
	"${INCLUDE_PATH}/SFGUI/Object.hpp"
	"${INCLUDE_PATH}/SFGUI/Primitive.hpp"
	"${INCLUDE_PATH}/SFGUI/PrimitiveTexture.hpp"
	"${INCLUDE_PATH}/SFGUI/PrimitiveVertex.hpp"
	"${INCLUDE_PATH}/SFGUI/ProgressBar.hpp"
	"${INCLUDE_PATH}/SFGUI/RadioButton.hpp"
	"${INCLUDE_PATH}/SFGUI/RadioButtonGroup.hpp"
	"${INCLUDE_PATH}/SFGUI/Range.hpp"
	"${INCLUDE_PATH}/SFGUI/RenderQueue.hpp"
	"${INCLUDE_PATH}/SFGUI/Renderer.hpp"
	"${INCLUDE_PATH}/SFGUI/Renderers.hpp"
	"${INCLUDE_PATH}/SFGUI/RendererTextureNode.hpp"
	"${INCLUDE_PATH}/SFGUI/RendererViewport.hpp"
	"${INCLUDE_PATH}/SFGUI/Renderers/NonLegacyRenderer.hpp"
	"${INCLUDE_PATH}/SFGUI/Renderers/VertexArrayRenderer.hpp"
	"${INCLUDE_PATH}/SFGUI/Renderers/VertexBufferRenderer.hpp"
	"${INCLUDE_PATH}/SFGUI/ResourceLoader.hpp"
	"${INCLUDE_PATH}/SFGUI/ResourceManager.hpp"
	"${INCLUDE_PATH}/SFGUI/ResourceManager.inl"
	"${INCLUDE_PATH}/SFGUI/SFGUI.hpp"
	"${INCLUDE_PATH}/SFGUI/Scale.hpp"
	"${INCLUDE_PATH}/SFGUI/Scrollbar.hpp"
	"${INCLUDE_PATH}/SFGUI/ScrolledWindow.hpp"
	"${INCLUDE_PATH}/SFGUI/Selector.hpp"
	"${INCLUDE_PATH}/SFGUI/Separator.hpp"
	"${INCLUDE_PATH}/SFGUI/Signal.hpp"
	"${INCLUDE_PATH}/SFGUI/SpinButton.hpp"
	"${INCLUDE_PATH}/SFGUI/Spinner.hpp"
	"${INCLUDE_PATH}/SFGUI/Table.hpp"
	"${INCLUDE_PATH}/SFGUI/TableCell.hpp"
	"${INCLUDE_PATH}/SFGUI/TableOptions.hpp"
	"${INCLUDE_PATH}/SFGUI/ToggleButton.hpp"
	"${INCLUDE_PATH}/SFGUI/Viewport.hpp"
	"${INCLUDE_PATH}/SFGUI/Widget.hpp"
	"${INCLUDE_PATH}/SFGUI/Widgets.hpp"
	"${INCLUDE_PATH}/SFGUI/Window.hpp"
	"${SOURCE_PATH}/SFGUI/Adjustment.cpp"
	"${SOURCE_PATH}/SFGUI/Alignment.cpp"
	"${SOURCE_PATH}/SFGUI/Bin.cpp"
	"${SOURCE_PATH}/SFGUI/Box.cpp"
	"${SOURCE_PATH}/SFGUI/Button.cpp"
	"${SOURCE_PATH}/SFGUI/Canvas.cpp"
	"${SOURCE_PATH}/SFGUI/CheckButton.cpp"
	"${SOURCE_PATH}/SFGUI/ComboBox.cpp"
	"${SOURCE_PATH}/SFGUI/Container.cpp"
	"${SOURCE_PATH}/SFGUI/Context.cpp"
	"${SOURCE_PATH}/SFGUI/DejaVuSansFont.hpp"
	"${SOURCE_PATH}/SFGUI/Desktop.cpp"
	"${SOURCE_PATH}/SFGUI/Engine.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Button.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/CheckButton.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/ComboBox.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Entry.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Frame.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Image.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Label.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Notebook.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/ProgressBar.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Scale.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Scrollbar.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/ScrolledWindow.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Separator.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/SpinButton.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Spinner.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/ToggleButton.cpp"
	"${SOURCE_PATH}/SFGUI/Engines/BREW/Window.cpp"
	"${SOURCE_PATH}/SFGUI/Entry.cpp"
	"${SOURCE_PATH}/SFGUI/FileResourceLoader.cpp"
	"${SOURCE_PATH}/SFGUI/Fixed.cpp"
	"${SOURCE_PATH}/SFGUI/Frame.cpp"
	"${SOURCE_PATH}/SFGUI/GLCheck.cpp"
	"${SOURCE_PATH}/SFGUI/GLCheck.hpp"
	"${SOURCE_PATH}/SFGUI/GLLoader.cpp"
	"${SOURCE_PATH}/SFGUI/GLLoader.hpp"
	"${SOURCE_PATH}/SFGUI/Image.cpp"
	"${SOURCE_PATH}/SFGUI/Label.cpp"
	"${SOURCE_PATH}/SFGUI/Misc.cpp"
	"${SOURCE_PATH}/SFGUI/Notebook.cpp"
	"${SOURCE_PATH}/SFGUI/Object.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/Grammar.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/Grammar.hpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/GrammarPredicates.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/GrammarSelector.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/GrammarSimpleSelector.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/GrammarStatement.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/GrammarToken.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/Parse.cpp"
	"${SOURCE_PATH}/SFGUI/Parsers/ThemeParser/Parse.hpp"
	"${SOURCE_PATH}/SFGUI/Primitive.cpp"
	"${SOURCE_PATH}/SFGUI/PrimitiveTexture.cpp"
	"${SOURCE_PATH}/SFGUI/PrimitiveVertex.cpp"
	"${SOURCE_PATH}/SFGUI/ProgressBar.cpp"
	"${SOURCE_PATH}/SFGUI/RadioButton.cpp"
	"${SOURCE_PATH}/SFGUI/RadioButtonGroup.cpp"
	"${SOURCE_PATH}/SFGUI/Range.cpp"
	"${SOURCE_PATH}/SFGUI/RenderQueue.cpp"
	"${SOURCE_PATH}/SFGUI/Renderer.cpp"
	"${SOURCE_PATH}/SFGUI/RendererBatch.hpp"
	"${SOURCE_PATH}/SFGUI/RendererViewport.cpp"
	"${SOURCE_PATH}/SFGUI/Renderers/NonLegacyRenderer.cpp"
	"${SOURCE_PATH}/SFGUI/Renderers/VertexArrayRenderer.cpp"
	"${SOURCE_PATH}/SFGUI/Renderers/VertexBufferRenderer.cpp"
	"${SOURCE_PATH}/SFGUI/ResourceManager.cpp"
	"${SOURCE_PATH}/SFGUI/SFGUI.cpp"
	"${SOURCE_PATH}/SFGUI/Scale.cpp"
	"${SOURCE_PATH}/SFGUI/Scrollbar.cpp"
	"${SOURCE_PATH}/SFGUI/ScrolledWindow.cpp"
	"${SOURCE_PATH}/SFGUI/Selector.cpp"
	"${SOURCE_PATH}/SFGUI/Separator.cpp"
	"${SOURCE_PATH}/SFGUI/Signal.cpp"
	"${SOURCE_PATH}/SFGUI/SpinButton.cpp"
	"${SOURCE_PATH}/SFGUI/Spinner.cpp"
	"${SOURCE_PATH}/SFGUI/Table.cpp"
	"${SOURCE_PATH}/SFGUI/TableCell.cpp"
	"${SOURCE_PATH}/SFGUI/TableOptions.cpp"
	"${SOURCE_PATH}/SFGUI/ToggleButton.cpp"
	"${SOURCE_PATH}/SFGUI/Viewport.cpp"
	"${SOURCE_PATH}/SFGUI/Widget.cpp"
	"${SOURCE_PATH}/SFGUI/Window.cpp"
)

if( SFGUI_INCLUDE_FONT )
	set(
		SOURCES
		${SOURCES}
		"${SOURCE_PATH}/SFGUI/DejaVuSansFont.cpp"
	)
endif()

include_directories( "${INCLUDE_PATH}" )
include_directories( "${SOURCE_PATH}" )
include_directories( SYSTEM "${SFML_INCLUDE_DIR}" )
include_directories( SYSTEM "${PROJECT_SOURCE_DIR}/extlibs/libELL/include" )

# Set the library output directory
set( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" )

# Add the library.
if( SFGUI_BUILD_SHARED_LIBS )
	add_library( sfgui SHARED ${SOURCES} )

	set_target_properties( sfgui PROPERTIES DEBUG_POSTFIX -d )
else()
	add_definitions( -DSFGUI_STATIC )
	add_library( sfgui ${SOURCES} )

	set_target_properties( sfgui PROPERTIES DEBUG_POSTFIX -s-d )
	set_target_properties( sfgui PROPERTIES RELEASE_POSTFIX -s )
	set_target_properties( sfgui PROPERTIES MINSIZEREL_POSTFIX -s )
endif()

# Tell the compiler to export when necessary.
set_target_properties( sfgui PROPERTIES DEFINE_SYMBOL SFGUI_EXPORTS )

# Platform- and compiler-specific options.
if( WIN32 )
	set( SFGUI_STATIC_STD_LIBS FALSE CACHE BOOL "Use statically linked standard/runtime libraries? This option must match the one used for SFML." )

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
			add_definitions( -DSFML_STATIC )
			if( MSVC )
				foreach( flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE )
					if( ${flag} MATCHES "/MD" )
					string( REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}" )
					endif()
				endforeach()
			elseif( CMAKE_COMPILER_IS_GNUCXX AND NOT COMPILER_GCC_TDM )
				set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++" )
			endif()
		endif()
	elseif( COMPILER_GCC_TDM )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared-libgcc -shared-libstdc++" )
	endif()

	add_definitions( -DWIN32 )
	target_link_libraries( sfgui ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY} )

	if( CMAKE_COMPILER_IS_GNUCXX )
		if( SFGUI_BUILD_SHARED_LIBS )
			set_target_properties( sfgui PROPERTIES PREFIX "" )
		endif()

		set_target_properties( sfgui PROPERTIES IMPORT_SUFFIX ".a" )
	endif()

	set( SHARE_PATH "." )
	set( LIB_PATH "lib" )
elseif( APPLE )
	find_library( COREFOUNDATION_LIBRARY CoreFoundation )
	mark_as_advanced( COREFOUNDATION_LIBRARY )

	include_directories( SYSTEM /System/Library/Frameworks/CoreFoundation.framework/Headers )
	target_link_libraries( sfgui ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY} ${COREFOUNDATION_LIBRARY} )
	set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/SFGUI" )
	set( LIB_PATH "lib" )
elseif( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
	target_link_libraries( sfgui ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} )
	set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/SFGUI" )
	
	if( LIB_SUFFIX )
		set( LIB_PATH "lib${LIB_SUFFIX}" )
	else()
		set( LIB_PATH "lib" )
	endif()
else()
	target_link_libraries( sfgui ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY} )
	set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/SFGUI" )
	set( LIB_PATH "lib" )
endif()

if( CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic ${CMAKE_CXX_FLAGS}" )
	set( CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic ${CMAKE_C_FLAGS}" )
elseif( CMAKE_COMPILER_IS_GNUCXX )
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic ${CMAKE_CXX_FLAGS}" )
	set( CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic ${CMAKE_C_FLAGS}" )
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

install(
	TARGETS sfgui
	RUNTIME DESTINATION bin COMPONENT bin
	LIBRARY DESTINATION "${LIB_PATH}" COMPONENT bin
	ARCHIVE DESTINATION "${LIB_PATH}" COMPONENT dev
)

install(
	DIRECTORY include
	DESTINATION .
)

install(
	FILES README AUTHORS LICENSE FONT.LICENSE CHANGELOG
	DESTINATION "${SHARE_PATH}"
)

install(
	FILES cmake/Modules/FindSFGUI.cmake
	DESTINATION "${SHARE_PATH}/cmake/Modules"
)
