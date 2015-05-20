#pragma once

#define SFGUI_MAJOR_VERSION 0
#define SFGUI_MINOR_VERSION 3
#define SFGUI_REVISION_VERSION 0

#if defined( _WIN32 ) || defined( __WIN32__ )
	#define SFGUI_SYSTEM_WINDOWS
	#define WIN32_LEAN_AND_MEAN

	#if !defined( NOMINMAX )
		#define NOMINMAX
	#endif
#endif

#if defined( SFGUI_SYSTEM_WINDOWS ) && !defined( SFGUI_STATIC )
	#if defined( SFGUI_EXPORTS )
		#define SFGUI_API __declspec( dllexport )
	#else
		#define SFGUI_API __declspec( dllimport )
	#endif
#else
	#define SFGUI_API
#endif

#if defined( _MSC_VER )
	#pragma warning(disable : 4251) // Suppress a warning which is meaningless for us
	#pragma warning(disable : 4503) // Suppress warnings about truncated names. Enable again if linker errors occur.
#endif

#if !defined( NDEBUG )
	#define SFGUI_DEBUG
	#include <iostream> // XXX Only for debugging purposes.
#endif
