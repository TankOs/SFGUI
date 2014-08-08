#include <SFGUI/GLCheck.hpp>

#include <SFML/OpenGL.hpp>
#include <iostream>
#include <iomanip>

void DoCheckGLError( const char* file, unsigned int line, const char* call ) {
	// Flag to control whether an external debug tool is being used
	static const auto external_gl_debug = false;

	if( external_gl_debug ) {
		return;
	}

	auto error = glGetError();

	if( error != GL_NO_ERROR ) {
		std::cerr << "GL Error detected at " << file << ":L" << std::dec << line << std::endl;
		std::cerr << call << std::endl;
		std::cerr << "Error: ";

		switch( error ) {
		case GL_INVALID_ENUM:
			std::cerr << "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			std::cerr << "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			std::cerr << "GL_INVALID_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << "GL_OUT_OF_MEMORY";
			break;
		default:
			std::cerr << "0x" << std::setw(4) << std::setfill('0') << std::hex << error;
			break;
		}

		std::cerr << std::endl << std::endl;
	}
}
