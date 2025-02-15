// Needs to be included before GLLoader for NOMINMAX
#include <SFGUI/Config.hpp>

// Needs to be included before OpenGL (so anything else)
#include <SFGUI/GLLoader.hpp>

// X headers define None which is used by SFML's window style.
#undef None

#include <SFGUI/Renderers/NonLegacyRenderer.hpp>
#include <SFGUI/RendererBatch.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Signal.hpp>
#include <SFGUI/Primitive.hpp>
#include <SFGUI/PrimitiveVertex.hpp>
#include <SFGUI/GLCheck.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/System/Vector3.hpp>
#include <sstream>
#include <cstddef>
#include <cassert>

// ARB_vertex_buffer_object
#define GLEXT_vertex_buffer_object sfgogl_ext_ARB_vertex_buffer_object

#define GLEXT_GL_ARRAY_BUFFER GL_ARRAY_BUFFER_ARB
#define GLEXT_GL_ELEMENT_ARRAY_BUFFER GL_ELEMENT_ARRAY_BUFFER_ARB
#define GLEXT_GL_DYNAMIC_DRAW GL_DYNAMIC_DRAW_ARB
#define GLEXT_GL_STATIC_DRAW GL_STATIC_DRAW_ARB

#define GLEXT_glBindBuffer glBindBufferARB
#define GLEXT_glDeleteBuffers glDeleteBuffersARB
#define GLEXT_glGenBuffers glGenBuffersARB
#define GLEXT_glBufferData glBufferDataARB
#define GLEXT_glBufferSubData glBufferSubDataARB

// ARB_multitexture
#define GLEXT_GL_TEXTURE0 GL_TEXTURE0_ARB

#define GLEXT_glActiveTexture glActiveTextureARB

// ARB_vertex_program
#define GLEXT_vertex_program sfgogl_ext_ARB_vertex_program

#define GLEXT_GL_PROGRAM_OBJECT GL_PROGRAM_OBJECT_ARB

#define GLEXT_glGetHandle glGetHandleARB
#define GLEXT_glUseProgramObject glUseProgramObjectARB
#define GLEXT_glVertexAttribPointer glVertexAttribPointerARB
#define GLEXT_glEnableVertexAttribArray glEnableVertexAttribArrayARB
#define GLEXT_glDisableVertexAttribArray glDisableVertexAttribArrayARB

// ARB_shading_language_100 (ensured by sf::Shader::isAvailable())
#define GLEXT_GL_SHADING_LANGUAGE_VERSION GL_SHADING_LANGUAGE_VERSION_ARB

// ARB_vertex_shader (ensured by sf::Shader::isAvailable())
#define GLEXT_GL_VERTEX_SHADER GL_VERTEX_SHADER_ARB

#define GLEXT_glGetAttribLocation glGetAttribLocationARB

// ARB_fragment_shader (ensured by sf::Shader::isAvailable())
#define GLEXT_GL_FRAGMENT_SHADER GL_FRAGMENT_SHADER_ARB

// ARB_shader_objects
#define GLEXT_GLhandle GLhandleARB

#define GLEXT_GL_OBJECT_COMPILE_STATUS GL_OBJECT_COMPILE_STATUS_ARB
#define GLEXT_GL_OBJECT_LINK_STATUS GL_OBJECT_LINK_STATUS_ARB

#define GLEXT_glCreateProgramObject glCreateProgramObjectARB
#define GLEXT_glDeletePrograms glDeleteProgramsARB
#define GLEXT_glCreateShaderObject glCreateShaderObjectARB
#define GLEXT_glShaderSource glShaderSourceARB
#define GLEXT_glCompileShader glCompileShaderARB
#define GLEXT_glGetObjectParameteriv glGetObjectParameterivARB
#define GLEXT_glDeleteObject glDeleteObjectARB
#define GLEXT_glAttachObject glAttachObjectARB
#define GLEXT_glLinkProgram glLinkProgramARB
#define GLEXT_glGetUniformLocation glGetUniformLocationARB
#define GLEXT_glUniform1i glUniform1iARB
#define GLEXT_glUniform2f glUniform2fARB

// ARB_vertex_array_object
#define GLEXT_vertex_array_object sfgogl_ext_ARB_vertex_array_object

#define GLEXT_glIsVertexArray glIsVertexArray
#define GLEXT_glBindVertexArray glBindVertexArray
#define GLEXT_glDeleteVertexArrays glDeleteVertexArrays
#define GLEXT_glGenVertexArrays glGenVertexArrays

// EXT_framebuffer_object
#define GLEXT_framebuffer_object sfgogl_ext_EXT_framebuffer_object

#define GLEXT_GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
#define GLEXT_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
#define GLEXT_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT

#define GLEXT_glBindFramebuffer glBindFramebufferEXT
#define GLEXT_glDeleteFramebuffers glDeleteFramebuffersEXT
#define GLEXT_glGenFramebuffers glGenFramebuffersEXT
#define GLEXT_glFramebufferTexture2D glFramebufferTexture2DEXT
#define GLEXT_glCheckFramebufferStatus glCheckFramebufferStatusEXT

#if defined( __APPLE__ )

    #define CastToGlHandle( x ) reinterpret_cast<GLEXT_GLhandle>( static_cast<std::ptrdiff_t>( x ) )
    #define CastFromGlHandle( x ) static_cast<unsigned int>( reinterpret_cast<std::ptrdiff_t>( x ) )

#else

    #define CastToGlHandle( x ) ( x )
    #define CastFromGlHandle( x ) ( x )

#endif

namespace {

bool gl_initialized = false;

bool vbo_supported = false;
bool vao_supported = false;
bool vap_supported = false;
bool shader_supported = false;
bool fbo_supported = false;

unsigned int GetAttributeLocation( unsigned int shader, std::string name ) {
	auto location = CheckGLError( GLEXT_glGetAttribLocation( CastToGlHandle( shader ), name.c_str() ) );

#if defined( SFGUI_DEBUG )
	if( location < 0 ) {
		std::cerr << "SFGUI warning: Failed to get location of attribute \"" << name << "\".\n";
	}
#endif

	assert( location >= 0 );

	return static_cast<unsigned int>( location );
}

unsigned int CreateShader( const char* vertex_source, const char* fragment_source ) {
	auto shader = CheckGLError( GLEXT_glCreateProgramObject() );

	auto vertex_shader = CheckGLError( GLEXT_glCreateShaderObject( GLEXT_GL_VERTEX_SHADER ) );

	CheckGLError( GLEXT_glShaderSource(
		vertex_shader,
		1,
		&vertex_source,
		nullptr
	) );
	CheckGLError( GLEXT_glCompileShader( vertex_shader ) );

	GLint success;

	CheckGLError( GLEXT_glGetObjectParameteriv( vertex_shader, GLEXT_GL_OBJECT_COMPILE_STATUS, &success ) );

	if( success == GL_FALSE ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "Failed to compile vertex shader, non-legacy renderer unavailable.\n";
#endif
		CheckGLError( GLEXT_glDeleteObject( vertex_shader ) );
		CheckGLError( GLEXT_glDeleteObject( shader ) );

		shader_supported = false;

		return 0;
	}

	CheckGLError( GLEXT_glAttachObject( shader, vertex_shader ) );
	CheckGLError( GLEXT_glDeleteObject( vertex_shader ) );

	auto fragment_shader = CheckGLError( GLEXT_glCreateShaderObject( GLEXT_GL_FRAGMENT_SHADER ) );

	CheckGLError( GLEXT_glShaderSource(
		fragment_shader,
		1,
		&fragment_source,
		nullptr
	) );
	CheckGLError( GLEXT_glCompileShader( fragment_shader ) );

	CheckGLError( GLEXT_glGetObjectParameteriv( fragment_shader, GLEXT_GL_OBJECT_COMPILE_STATUS, &success ) );

	if( success == GL_FALSE ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "Failed to compile fragment shader, non-legacy renderer unavailable.\n";
#endif
		CheckGLError( GLEXT_glDeleteObject( fragment_shader ) );
		CheckGLError( GLEXT_glDeleteObject( shader ) );

		shader_supported = false;

		return 0;
	}

	CheckGLError( GLEXT_glAttachObject( shader, fragment_shader ) );
	CheckGLError( GLEXT_glDeleteObject( fragment_shader ) );

	CheckGLError( GLEXT_glLinkProgram( shader ) );

	CheckGLError( GLEXT_glGetObjectParameteriv( shader, GLEXT_GL_OBJECT_LINK_STATUS, &success ) );

	if( success == GL_FALSE ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "Failed to link shader program, non-legacy renderer unavailable.\n";
#endif
			CheckGLError( GLEXT_glDeleteObject( shader ) );

			return 0;
	}

	return CastFromGlHandle( shader );
}

}

namespace sfg {

NonLegacyRenderer::NonLegacyRenderer() :
	m_previous_window_size( -1, -1 ),
	m_last_vertex_count( 0 ),
	m_last_index_count( 0 ),
	m_vbo_sync_type( INVALIDATE_ALL ),
	m_vbo_synced( false ),
	m_cull( false ),
	m_use_fbo( false ) {
	if( IsAvailable() ) {
		sf::Context context;

		m_shader = CreateShader(
			"#version 130\n"
			"uniform vec2 viewport_parameters;\n"
			"in vec2 vertex;\n"
			"in vec4 color;\n"
			"in vec2 texture_coordinate;\n"
			"out vec4 vertex_color;\n"
			"out vec2 vertex_texture_coordinate;\n"
			"void main() {\n"
			"\tmat4 mvp_matrix = mat4(1.f);\n"
			"\tmvp_matrix[3][0] = -1.f;\n"
			"\tmvp_matrix[3][1] = 1.f;\n"
			"\tmvp_matrix[0][0] = viewport_parameters.x;\n"
			"\tmvp_matrix[1][1] = viewport_parameters.y;\n"
			"\tmvp_matrix[2][2] = -1.f;\n"
			"\tgl_Position = mvp_matrix * vec4(vertex.xy, 1.f, 1.f);\n"
			"\tvertex_color = color;\n"
			"\tvertex_texture_coordinate = texture_coordinate;\n"
			"}\n",
			"#version 130\n"
			"uniform sampler2D texture0;\n"
			"in vec4 vertex_color;\n"
			"in vec2 vertex_texture_coordinate;\n"
			"out vec4 fragment_color;\n"
			"void main() {\n"
			"\tfragment_color = vertex_color * texture(texture0, vertex_texture_coordinate);\n"
			"}\n"
		);

		m_fbo_shader = CreateShader(
			"#version 130\n"
			"in vec2 vertex;\n"
			"in vec2 texture_coordinate;\n"
			"out vec2 vertex_texture_coordinate;\n"
			"void main() {\n"
			"\tgl_Position = vec4(vertex.xy, 1.f, 1.f);\n"
			"\tvertex_texture_coordinate = texture_coordinate;\n"
			"}\n",
			"#version 130\n"
			"uniform sampler2D texture0;\n"
			"in vec2 vertex_texture_coordinate;\n"
			"out vec4 fragment_color;\n"
			"void main() {\n"
			"\tfragment_color = texture(texture0, vertex_texture_coordinate);\n"
			"}\n"
		);

		if( !m_shader || !m_fbo_shader )
			return;

		CheckGLError( m_viewport_parameters_location = GLEXT_glGetUniformLocation( CastToGlHandle( m_shader ), "viewport_parameters" ) );
		CheckGLError( m_texture_location = GLEXT_glGetUniformLocation( CastToGlHandle( m_shader ), "texture0" ) );

		CheckGLError( m_vertex_location = GetAttributeLocation( m_shader, "vertex" ) );
		CheckGLError( m_color_location = GetAttributeLocation( m_shader, "color" ) );
		CheckGLError( m_texture_coordinate_location = GetAttributeLocation( m_shader, "texture_coordinate" ) );

		CheckGLError( m_fbo_texture_location = GLEXT_glGetUniformLocation( CastToGlHandle( m_fbo_shader ), "texture0" ) );

		CheckGLError( m_fbo_vertex_location = GetAttributeLocation( m_fbo_shader, "vertex" ) );
		CheckGLError( m_fbo_texture_coordinate_location = GetAttributeLocation( m_fbo_shader, "texture_coordinate" ) );

		CheckGLError( GLEXT_glGenBuffers( 1, &m_vertex_vbo ) );
		CheckGLError( GLEXT_glGenBuffers( 1, &m_color_vbo ) );
		CheckGLError( GLEXT_glGenBuffers( 1, &m_texture_vbo ) );
		CheckGLError( GLEXT_glGenBuffers( 1, &m_index_vbo ) );
	}
	else {
#if defined( SFGUI_DEBUG )
		std::cerr << "Non-legacy renderer unavailable.\n";
#endif
	}
}

NonLegacyRenderer::~NonLegacyRenderer() {
	sf::Context context;

	DestroyFBO();

	CheckGLError( GLEXT_glDeleteBuffers( 1, &m_index_vbo ) );
	CheckGLError( GLEXT_glDeleteBuffers( 1, &m_texture_vbo ) );
	CheckGLError( GLEXT_glDeleteBuffers( 1, &m_color_vbo ) );
	CheckGLError( GLEXT_glDeleteBuffers( 1, &m_vertex_vbo ) );

	CheckGLError( GLEXT_glDeleteVertexArrays( 1, &m_vao ) );

	CheckGLError( GLEXT_glDeletePrograms( 1, &m_fbo_shader ) );
	CheckGLError( GLEXT_glDeletePrograms( 1, &m_shader ) );
}

NonLegacyRenderer::Ptr NonLegacyRenderer::Create() {
	return Ptr( new NonLegacyRenderer );
}

const std::string& NonLegacyRenderer::GetName() const {
	static const std::string name( "Non-Legacy Renderer" );
	return name;
}

bool NonLegacyRenderer::IsAvailable() {
	static bool checked = false;

	if( !checked ) {
		// Make sure we have a valid GL context before messing around
		// with GLLoader or else it will report missing extensions sometimes.
		sf::Context context;

		if( !gl_initialized ) {
			auto result = sfgogl_LoadFunctions() - sfgogl_LOAD_SUCCEEDED;

			if( result ) {
#if defined( SFGUI_DEBUG )
				std::cerr << "GL extension initialization failed. Missing " << result << " functions.\n";
#endif
				return false;
			}

			gl_initialized = true;
		}

		if( GLEXT_vertex_buffer_object ) {
			vbo_supported = true;
		}

		if( GLEXT_vertex_program ) {
			vap_supported = true;
		}

		if( sf::Shader::isAvailable() ) {
			auto glsl_version_string = CheckGLError( glGetString( GLEXT_GL_SHADING_LANGUAGE_VERSION ) );

			if( glsl_version_string ) {
				std::stringstream sstr( reinterpret_cast<const char*>( glsl_version_string ) );
				auto version = 0.0;
				sstr >> version;

				if( version > 1.29 ) {
					shader_supported = true;
				}
			}
		}

		if( GLEXT_vertex_array_object ) {
			vao_supported = true;
		}

		if( GLEXT_framebuffer_object ) {
			fbo_supported = true;
		}

		checked = true;
	}

	if( vbo_supported && vap_supported && shader_supported && vao_supported ) {
		return true;
	}

	return false;
}

void NonLegacyRenderer::Display( sf::Window& target ) const {
	m_window_size = static_cast<sf::Vector2i>( target.getSize() );

	(void)target.setActive( true );

	auto blend_enabled = CheckGLError( glIsEnabled( GL_BLEND ) );

	if( !blend_enabled ) {
		glEnable( GL_BLEND );
	}

	GLenum source_factor;
	GLenum destination_factor;
	CheckGLError( glGetIntegerv( GL_BLEND_SRC, reinterpret_cast<GLint*>( &source_factor ) ) );
	CheckGLError( glGetIntegerv( GL_BLEND_DST, reinterpret_cast<GLint*>( &destination_factor ) ) );

	if( ( source_factor != GL_SRC_ALPHA ) || ( destination_factor != GL_ONE_MINUS_SRC_ALPHA ) ) {
		CheckGLError( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	}

	DisplayImpl();

	if( !blend_enabled ) {
		CheckGLError( glDisable( GL_BLEND ) );
	}

	if( ( source_factor != GL_SRC_ALPHA ) || ( destination_factor != GL_ONE_MINUS_SRC_ALPHA ) ) {
		CheckGLError( glBlendFunc( source_factor, destination_factor ) );
	}
}

void NonLegacyRenderer::Display( sf::RenderWindow& target ) const {
	m_window_size = static_cast<sf::Vector2i>( target.getSize() );

	(void)target.setActive( true );

	DisplayImpl();
}

void NonLegacyRenderer::Display( sf::RenderTexture& target ) const {
	m_window_size = static_cast<sf::Vector2i>( target.getSize() );

	(void)target.setActive( true );

	DisplayImpl();
}

void NonLegacyRenderer::DisplayImpl() const {
	if( !IsAvailable() ) {
		return;
	}

	auto previous_program = CheckGLError( GLEXT_glGetHandle( GLEXT_GL_PROGRAM_OBJECT ) );

	if( m_last_window_size != m_window_size ) {
		CheckGLError( glViewport( 0, 0, m_window_size.x, m_window_size.y ) );

		m_last_window_size = m_window_size;

		if( m_window_size.x && m_window_size.y ) {
			const_cast<NonLegacyRenderer*>( this )->Invalidate( INVALIDATE_VERTEX | INVALIDATE_TEXTURE );

			CheckGLError( GLEXT_glUseProgramObject( CastToGlHandle( m_shader ) ) );
			CheckGLError( GLEXT_glUniform2f(m_viewport_parameters_location, 2.f / static_cast<float>( m_window_size.x ), -2.f / static_cast<float>( m_window_size.y ) ) );

			const_cast<NonLegacyRenderer*>( this )->SetupFBO( m_window_size.x, m_window_size.y );
		}
	}

	if( !m_vbo_synced ) {
		// Disclaimer:
		// const_cast IS safe to use in ANY non-static method of
		// Renderer. Because we make sure that the only instance
		// that can be constructed is the singleton instance and
		// that the singleton instance is non-const, we don't have
		// to fear that some member variable is non-mutable. This
		// is a nice hack to please certain people who require const
		// methods in the most exotic places without having to declare
		// half the member variables and methods as mutable. Then
		// again this might be all wrong...

		// Refresh VBO data if out of sync
		const_cast<NonLegacyRenderer*>( this )->RefreshVBO();
	}

	if( !GLEXT_glIsVertexArray( m_vao ) ) {
		const_cast<NonLegacyRenderer*>( this )->SetupVAO();
	}

	CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
	auto texture_binding = 0;
	CheckGLError( glGetIntegerv( GL_TEXTURE_BINDING_2D, &texture_binding) );
	CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );

	if( !m_use_fbo || !m_vbo_synced || m_force_redraw ) {
		// Thanks to color / texture modulation we can draw the entire
		// frame in a single pass by pseudo-disabling the texturing with
		// the help of a white texture ( 1.f * something = something ).
		// Further, we stick all referenced textures into our giant atlas
		// so we don't have to rebind during the draw.

		if( m_use_fbo ) {
			CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, m_frame_buffer ) );

			CheckGLError( glClear( GL_COLOR_BUFFER_BIT ) );
		}

		CheckGLError( GLEXT_glUseProgramObject( CastToGlHandle( m_shader ) ) );
		CheckGLError( GLEXT_glUniform1i( m_texture_location, 1 ) );

		CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
		sf::Texture::bind( m_texture_atlas[0].get() );
		CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );

		CheckGLError( GLEXT_glBindVertexArray( m_vao ) );

		CheckGLError( glEnable( GL_SCISSOR_TEST ) );

		auto current_atlas_page = 0;

		for( const auto& batch : m_batches ) {
			auto viewport = batch.viewport;

			if( batch.custom_draw ) {
				auto destination = static_cast<sf::Vector2i>( viewport->GetDestinationOrigin() );
				auto size = static_cast<sf::Vector2i>( viewport->GetSize() );

				CheckGLError( GLEXT_glBindVertexArray( 0 ) );
				CheckGLError( GLEXT_glUseProgramObject( 0 ) );

				CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
				auto custom_draw_texture_binding = 0;
				CheckGLError( glGetIntegerv( GL_TEXTURE_BINDING_2D, &custom_draw_texture_binding) );
				CheckGLError( glBindTexture( GL_TEXTURE_2D, static_cast<unsigned int>( 0 ) ) );
				CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );

				CheckGLError( glViewport( destination.x, m_window_size.y - destination.y - size.y, size.x, size.y ) );

				// Draw canvas.
				( *batch.custom_draw_callback )();

				CheckGLError( glViewport( 0, 0, m_window_size.x, m_window_size.y ) );

				CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
				CheckGLError( glBindTexture( GL_TEXTURE_2D, static_cast<unsigned int>( custom_draw_texture_binding ) ) );
				CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );

				CheckGLError( GLEXT_glUseProgramObject( CastToGlHandle( m_shader ) ) );
				CheckGLError( GLEXT_glBindVertexArray( m_vao ) );
			}
			else {
				if( viewport && ( ( *viewport ) != ( *m_default_viewport ) ) ) {
					auto destination_origin = viewport->GetDestinationOrigin();
					auto size = viewport->GetSize();

					CheckGLError( glScissor(
						static_cast<int>( destination_origin.x ),
						m_window_size.y - static_cast<int>( destination_origin.y + size.y ),
						static_cast<int>( size.x ),
						static_cast<int>( size.y )
					) );
				}
				else {
					CheckGLError( glScissor( 0, 0, m_window_size.x, m_window_size.y ) );
				}

				if( batch.index_count ) {
					if( batch.atlas_page != current_atlas_page ) {
						current_atlas_page = batch.atlas_page;

						CheckGLError( GLEXT_glUseProgramObject( CastToGlHandle( m_shader ) ) );
						CheckGLError( GLEXT_glUniform1i( m_texture_location, 1 ) );
						CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
						sf::Texture::bind( ( m_texture_atlas[static_cast<std::size_t>( current_atlas_page )] ).get() );
						CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );
					}

					CheckGLError( glDrawRangeElements(
						GL_TRIANGLES,
						static_cast<unsigned int>( batch.min_index ),
						static_cast<unsigned int>( batch.max_index ),
						batch.index_count,
						GL_UNSIGNED_INT,
						reinterpret_cast<const GLvoid*>( static_cast<std::size_t>( batch.start_index ) * sizeof( GLuint ) )
					) );
				}
			}
		}

		CheckGLError( glDisable( GL_SCISSOR_TEST ) );

		m_force_redraw = false;

		if( m_use_fbo ) {
			CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, 0 ) );

			CheckGLError( GLEXT_glUseProgramObject( CastToGlHandle( m_fbo_shader ) ) );
			CheckGLError( GLEXT_glUniform1i( m_fbo_texture_location, 1 ) );
			CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
			CheckGLError( glBindTexture( GL_TEXTURE_2D, m_frame_buffer_texture ) );
			CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );

			auto is_vertex_array = CheckGLError( GLEXT_glIsVertexArray( m_fbo_vao ) );

			if( !is_vertex_array ) {
				const_cast<NonLegacyRenderer*>( this )->SetupFBOVAO();
			}

			CheckGLError( GLEXT_glBindVertexArray( m_fbo_vao ) );

			assert( m_fbo_vbo != 0 );
			assert( m_fbo_vao != 0 );

			CheckGLError( glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
		}
	}
	else {
		CheckGLError( GLEXT_glUseProgramObject( CastToGlHandle( m_fbo_shader ) ) );
		CheckGLError( GLEXT_glUniform1i( m_fbo_texture_location, 1 ) );
		CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
		CheckGLError( glBindTexture( GL_TEXTURE_2D, m_frame_buffer_texture ) );
		CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );

		auto is_vertex_array = CheckGLError( GLEXT_glIsVertexArray( m_fbo_vao ) );

		if( !is_vertex_array ) {
			const_cast<NonLegacyRenderer*>( this )->SetupFBOVAO();
		}

		CheckGLError( GLEXT_glBindVertexArray( m_fbo_vao ) );

		assert( m_fbo_vbo != 0 );
		assert( m_fbo_vao != 0 );

		CheckGLError( glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );
	}

	// Needed otherwise SFML will blow up...
	CheckGLError( GLEXT_glBindVertexArray( 0 ) );

	CheckGLError( GLEXT_glUseProgramObject( previous_program ) );

	CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 + 1 ) );
	CheckGLError( glBindTexture( GL_TEXTURE_2D, static_cast<unsigned int>( texture_binding ) ) );
	CheckGLError( GLEXT_glActiveTexture( GLEXT_GL_TEXTURE0 ) );

	m_vbo_synced = true;
}

void NonLegacyRenderer::RefreshVBO() {
	SortPrimitives();

	m_vertex_data.clear();
	m_color_data.clear();
	m_texture_data.clear();
	m_index_data.clear();

	m_vertex_data.reserve( static_cast<std::size_t>( m_vertex_count ) );
	m_color_data.reserve( static_cast<std::size_t>( m_vertex_count ) );
	m_texture_data.reserve( static_cast<std::size_t>( m_vertex_count ) );
	m_index_data.reserve( static_cast<std::size_t>( m_index_count ) );

	m_batches.clear();

	m_last_vertex_count = 0;
	m_last_index_count = 0;

	// Default viewport
	priv::RendererBatch current_batch;
	current_batch.viewport = m_default_viewport;
	current_batch.atlas_page = 0;
	current_batch.start_index = 0;
	current_batch.index_count = 0;
	current_batch.min_index = 0;
	current_batch.max_index = m_vertex_count - 1;
	current_batch.custom_draw = false;

	sf::FloatRect window_viewport( { 0.f, 0.f }, sf::Vector2f( m_window_size ) );

	const auto max_texture_size = GetMaxTextureSize();
	const auto default_texture_size = m_texture_atlas[0]->getSize();

	for( const auto& primitive_ptr : m_primitives ) {
		auto primitive = primitive_ptr.get();

		primitive->SetSynced();

		if( !primitive->IsVisible() ) {
			continue;
		}

		auto position_transform = primitive->GetPosition();

		auto viewport = primitive->GetViewport();

		auto viewport_rect = window_viewport;

		// Check if primitive needs to be rendered in a custom viewport.
		if( viewport && ( ( *viewport ) != ( *m_default_viewport ) ) ) {
			auto destination_origin = viewport->GetDestinationOrigin();
			auto size = viewport->GetSize();

			position_transform += ( destination_origin - viewport->GetSourceOrigin() );

			if( m_cull ) {
				viewport_rect = { destination_origin, size };
			}
		}

		const auto& custom_draw_callback = primitive->GetCustomDrawCallback();

		if( custom_draw_callback ) {
			// Start a new batch.
			current_batch.max_index = m_last_vertex_count ? ( m_last_vertex_count - 1 ) : 0;
			m_batches.push_back( current_batch );

			// Mark current_batch custom draw batch.
			current_batch.viewport = viewport;
			current_batch.start_index = 0;
			current_batch.index_count = 0;
			current_batch.min_index = 0;
			current_batch.max_index = 0;
			current_batch.custom_draw = true;
			current_batch.custom_draw_callback = custom_draw_callback;

			// Start a new batch.
			m_batches.push_back( current_batch );

			// Reset current_batch to defaults.
			current_batch.viewport = m_default_viewport;
			current_batch.start_index = m_last_index_count;
			current_batch.index_count = 0;
			current_batch.min_index = m_last_vertex_count ? ( m_last_vertex_count - 1 ) : 0;
			current_batch.custom_draw = false;
		}
		else {
			// Process primitive's vertices and indices
			const std::vector<PrimitiveVertex>& vertices( primitive->GetVertices() );
			const std::vector<GLuint>& indices( primitive->GetIndices() );

			sf::Vector2f position( 0.f, 0.f );

			sf::FloatRect bounding_rect( { 0.f, 0.f }, { 0.f, 0.f } );

			auto atlas_page = 0;

			const auto vertices_size = vertices.size();
			sf::Vector2f normalizer;

			for( std::size_t index = 0; index < vertices_size; ++index ) {
				const auto vertex = vertices[index];
				position.x = vertex.position.x + position_transform.x;
				position.y = vertex.position.y + position_transform.y;

				m_vertex_data.push_back( position );
				m_color_data.push_back( vertex.color );

				// The bound texture can only change between triangles.
				if( index % 3 == 0 ) {
					atlas_page = static_cast<int>( vertex.texture_coordinate.y ) / max_texture_size;
					auto texture_size = ( vertex.texture_coordinate.y <= 1.f ) ? default_texture_size : m_texture_atlas[static_cast<std::size_t>( atlas_page )]->getSize();

					// Used to normalize texture coordinates.
					normalizer.x = 1.f / static_cast<float>( texture_size.x );
					normalizer.y = 1.f / static_cast<float>( texture_size.y );
				}

				// Normalize SFML's pixel texture coordinates.
				m_texture_data.emplace_back( vertex.texture_coordinate.x * normalizer.x, static_cast<float>( static_cast<int>( vertex.texture_coordinate.y ) % max_texture_size ) * normalizer.y );

				// Update the bounding rect.
				if( m_cull ) {
					if( position.x < bounding_rect.position.x ) {
						bounding_rect.size.x += bounding_rect.position.x - position.x;
						bounding_rect.position.x = position.x;
					}
					else if( position.x > bounding_rect.position.x + bounding_rect.size.x ) {
						bounding_rect.size.x = position.x - bounding_rect.position.x;
					}

					if( position.y < bounding_rect.position.y ) {
						bounding_rect.size.y += bounding_rect.position.y - position.y;
						bounding_rect.position.y = position.y;
					}
					else if( position.y > bounding_rect.position.y + bounding_rect.size.y ) {
						bounding_rect.size.y = position.y - bounding_rect.position.y;
					}
				}
			}

			if( m_cull && !viewport_rect.findIntersection( bounding_rect ) ) {
				m_vertex_data.resize( static_cast<std::size_t>( m_last_vertex_count ) );
				m_color_data.resize( static_cast<std::size_t>( m_last_vertex_count ) );
				m_texture_data.resize( static_cast<std::size_t>( m_last_vertex_count ) );
			}
			else {
				for( const auto& index : indices ) {
					m_index_data.push_back( static_cast<unsigned int>( m_last_vertex_count ) + index );
				}

				// Check if we need to start a new batch.
				if( ( ( *viewport ) != ( *current_batch.viewport ) ) || ( atlas_page != current_batch.atlas_page ) ) {
					current_batch.max_index = m_last_vertex_count ? ( m_last_vertex_count - 1 ) : 0;
					m_batches.push_back( current_batch );

					// Reset current_batch to defaults.
					current_batch.viewport = viewport;
					current_batch.atlas_page = atlas_page;
					current_batch.start_index = m_last_index_count;
					current_batch.index_count = 0;
					current_batch.min_index = m_last_vertex_count ? ( m_last_vertex_count - 1 ) : 0;
					current_batch.custom_draw = false;
				}

				current_batch.index_count += static_cast<int>( indices.size() );

				m_last_vertex_count += static_cast<GLsizei>( vertices.size() );
				m_last_index_count += static_cast<GLsizei>( indices.size() );
			}
		}
	}

	current_batch.max_index = m_last_vertex_count ? ( m_last_vertex_count - 1 ) : 0;
	m_batches.push_back( current_batch );

	if( !m_vertex_data.empty() && !m_color_data.empty() && !m_texture_data.empty() ) {
		if( m_vbo_sync_type & INVALIDATE_VERTEX ) {
			// Sync vertex data
			CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_vertex_vbo ) );
			CheckGLError( GLEXT_glBufferData( GLEXT_GL_ARRAY_BUFFER, static_cast<int>( m_vertex_data.size() * sizeof( sf::Vector3f ) ), 0, GLEXT_GL_DYNAMIC_DRAW ) );

			if( m_vertex_data.size() > 0 ) {
				CheckGLError( GLEXT_glBufferSubData( GLEXT_GL_ARRAY_BUFFER, 0, static_cast<int>( m_vertex_data.size() * sizeof( sf::Vector2f ) ), m_vertex_data.data() ) );
			}
		}

		if( m_vbo_sync_type & INVALIDATE_COLOR ) {
			// Sync color data
			CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_color_vbo ) );
			CheckGLError( GLEXT_glBufferData( GLEXT_GL_ARRAY_BUFFER, static_cast<int>( m_color_data.size() * sizeof( sf::Color ) ), 0, GLEXT_GL_DYNAMIC_DRAW ) );

			if( m_color_data.size() > 0 ) {
				CheckGLError( GLEXT_glBufferSubData( GLEXT_GL_ARRAY_BUFFER, 0, static_cast<int>( m_color_data.size() * sizeof( sf::Color ) ), m_color_data.data() ) );
			}
		}

		if( m_vbo_sync_type & INVALIDATE_TEXTURE ) {
			// Sync texture coord data
			CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_texture_vbo ) );
			CheckGLError( GLEXT_glBufferData( GLEXT_GL_ARRAY_BUFFER, static_cast<int>( m_texture_data.size() * sizeof( sf::Vector2f ) ), 0, GLEXT_GL_DYNAMIC_DRAW ) );

			if( m_texture_data.size() > 0 ) {
				CheckGLError( GLEXT_glBufferSubData( GLEXT_GL_ARRAY_BUFFER, 0, static_cast<int>( m_texture_data.size() * sizeof( sf::Vector2f ) ), m_texture_data.data() ) );
			}
		}

		if( m_vbo_sync_type & INVALIDATE_INDEX ) {
			// Sync index data
			CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, m_index_vbo ) );
			CheckGLError( GLEXT_glBufferData( GLEXT_GL_ELEMENT_ARRAY_BUFFER, static_cast<int>( m_index_data.size() * sizeof( GLuint ) ), 0, GLEXT_GL_DYNAMIC_DRAW ) );

			if( m_index_data.size() > 0 ) {
				CheckGLError( GLEXT_glBufferSubData( GLEXT_GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<int>( m_index_data.size() * sizeof( GLuint ) ), m_index_data.data() ) );
			}

			CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, 0 ) );
		}
	}

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, 0 ) );

	m_vbo_sync_type = 0;
}

void NonLegacyRenderer::InvalidateVBO( unsigned char datasets ) {
	m_vbo_sync_type |= datasets;
	m_vbo_synced = false;
}

void NonLegacyRenderer::SetupFBO( int width, int height ) {
	if( !m_use_fbo || !width || !height ) {
		DestroyFBO();

		return;
	}

	// Create FBO.
	if( !m_frame_buffer ) {
		CheckGLError( GLEXT_glGenFramebuffers( 1, &m_frame_buffer ) );
	}

	CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, m_frame_buffer ) );

	auto old_texture_id = 0u;
	CheckGLError( glGetIntegerv( GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>( &old_texture_id ) ) );

	// Create FBO attachment
	if( !m_frame_buffer_texture ) {
		CheckGLError( glGenTextures( 1, &m_frame_buffer_texture ) );
	}

	CheckGLError( glBindTexture( GL_TEXTURE_2D, m_frame_buffer_texture ) );
	CheckGLError( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr ) );
	CheckGLError( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	CheckGLError( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	CheckGLError( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	CheckGLError( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );

	CheckGLError( glBindTexture( GL_TEXTURE_2D, old_texture_id ) );

	CheckGLError( GLEXT_glFramebufferTexture2D( GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frame_buffer_texture, 0 ) );

	// Sanity check.
	auto status = CheckGLError( GLEXT_glCheckFramebufferStatus( GLEXT_GL_FRAMEBUFFER ) );

	if( status != GLEXT_GL_FRAMEBUFFER_COMPLETE ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "GLEXT_glCheckFramebufferStatus() returned error " << status << ", disabling FBO.\n";
#endif

		DestroyFBO();

		fbo_supported = false;
		m_use_fbo = false;
	}

	CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, 0 ) );

	if( m_use_fbo && !m_fbo_vbo && !m_fbo_vao ) {
		CheckGLError( GLEXT_glGenBuffers( 1, &m_fbo_vbo ) );
		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_fbo_vbo ) );

		// GL_TRIANGLE_STRIP
		GLbyte data[] = {
			1, 1, 1, 1,
			-1, 1, 0, 1,
			1, -1, 1, 0,
			-1, -1, 0, 0
		};

		CheckGLError( GLEXT_glBufferData( GLEXT_GL_ARRAY_BUFFER, sizeof( data ), data, GLEXT_GL_STATIC_DRAW ) );

		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, 0 ) );
	}
}

void NonLegacyRenderer::DestroyFBO() {
	if( m_fbo_vbo ) {
		CheckGLError( GLEXT_glDeleteBuffers( 1, &m_fbo_vbo ) );
		m_fbo_vbo = 0;
	}

	if( m_frame_buffer ) {
		CheckGLError( GLEXT_glDeleteFramebuffers( 1, &m_frame_buffer ) );
		m_frame_buffer = 0;
	}

	if( m_frame_buffer_texture ) {
		CheckGLError( glDeleteTextures( 1, &m_frame_buffer_texture ) );
		m_frame_buffer_texture = 0;
	}
}

void NonLegacyRenderer::TuneCull( bool enable ) {
	m_cull = enable;
}

void NonLegacyRenderer::TuneUseFBO( bool enable ) {
	if( !fbo_supported && enable ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "FBO extension unavailable.\n";
#endif
	}

	m_use_fbo = enable && fbo_supported;

	if( m_use_fbo ) {
		SetupFBO( m_window_size.x, m_window_size.y );
	}
	else {
		DestroyFBO();
	}
}

void NonLegacyRenderer::InvalidateImpl( unsigned char datasets ) {
	InvalidateVBO( datasets );
}

void NonLegacyRenderer::SetupVAO() {
	CheckGLError( GLEXT_glGenVertexArrays( 1, &m_vao ) );
	CheckGLError( GLEXT_glBindVertexArray( m_vao ) );

	assert( m_vertex_vbo != 0 );
	assert( m_color_vbo != 0 );
	assert( m_texture_vbo != 0 );
	assert( m_index_vbo != 0 );
	assert( m_vao != 0 );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_vertex_vbo ) );
	CheckGLError( GLEXT_glEnableVertexAttribArray( m_vertex_location ) );
	CheckGLError( GLEXT_glVertexAttribPointer( m_vertex_location, 2, GL_FLOAT, GL_FALSE, 0, 0 ) );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_color_vbo ) );
	CheckGLError( GLEXT_glEnableVertexAttribArray( m_color_location ) );
	CheckGLError( GLEXT_glVertexAttribPointer( m_color_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0 ) );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_texture_vbo ) );
	CheckGLError( GLEXT_glEnableVertexAttribArray( m_texture_coordinate_location ) );
	CheckGLError( GLEXT_glVertexAttribPointer( m_texture_coordinate_location, 2, GL_FLOAT, GL_FALSE, 0, 0 ) );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, m_index_vbo ) );

	CheckGLError( GLEXT_glBindVertexArray( 0 ) );

	CheckGLError( GLEXT_glDisableVertexAttribArray( m_texture_coordinate_location ) );
	CheckGLError( GLEXT_glDisableVertexAttribArray( m_color_location ) );
	CheckGLError( GLEXT_glDisableVertexAttribArray( m_vertex_location ) );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, 0 ) );
	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, 0 ) );
}

void NonLegacyRenderer::SetupFBOVAO() {
	if( !m_fbo_vbo ) {
		return;
	}

	CheckGLError( GLEXT_glGenVertexArrays( 1, &m_fbo_vao ) );
	CheckGLError( GLEXT_glBindVertexArray( m_fbo_vao ) );

	assert( m_fbo_vbo != 0 );
	assert( m_fbo_vao != 0 );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_fbo_vbo ) );

	CheckGLError( GLEXT_glEnableVertexAttribArray( m_fbo_vertex_location ) );
	CheckGLError( GLEXT_glEnableVertexAttribArray( m_fbo_texture_coordinate_location ) );
	CheckGLError( GLEXT_glVertexAttribPointer( m_fbo_vertex_location, 2, GL_BYTE, GL_FALSE, 4 * sizeof( GLbyte ), reinterpret_cast<GLvoid*>( 0 ) ) );
	CheckGLError( GLEXT_glVertexAttribPointer( m_fbo_texture_coordinate_location, 2, GL_BYTE, GL_FALSE, 4 * sizeof( GLbyte ), reinterpret_cast<GLvoid*>( 2 * sizeof( GLbyte ) ) ) );

	CheckGLError( GLEXT_glBindVertexArray( 0 ) );

	CheckGLError( GLEXT_glDisableVertexAttribArray( m_fbo_texture_coordinate_location ) );
	CheckGLError( GLEXT_glDisableVertexAttribArray( m_fbo_vertex_location ) );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, 0 ) );
}

}
