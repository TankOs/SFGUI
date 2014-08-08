#pragma once

#include <cstring>

#if defined( _WIN32 )
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#if !defined( NDEBUG )
	#define CheckGLError(call) call; DoCheckGLError( __FILENAME__, __LINE__, #call );
#else
	#define CheckGLError(call) (call)
#endif

void DoCheckGLError( const char* file, unsigned int line, const char* call );
