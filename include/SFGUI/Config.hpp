#ifndef _SFGUI_CONFIG_HPP__
#define _SFGUI_CONFIG_HPP__

#if defined( _WIN32 ) || defined( __WIN32__ )
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX

	#ifdef SFGUI_EXPORTS
		#define SFGUI_API __declspec( dllexport )
	#else
		#define SFGUI_API __declspec( dllimport )
	#endif
#else
	#define SFGUI_API
#endif

#include <iostream> // XXX Only for debugging purposes.

#endif
