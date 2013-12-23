// Needs to be included before GLEW for NOMINMAX
#include <SFGUI/Config.hpp>

// Needs to be included before OpenGL (so anything else)
#include <GL/glew.h>

// X headers define None which is used by SFML's window style.
#undef None

#include <SFGUI/Renderers/VertexBufferRenderer.hpp>
#include <SFGUI/RendererViewport.hpp>

namespace sfg {

// Although it has the ARB prefix, this is a core extension promoted from EXT.
#if defined( GLEW_ARB_framebuffer_object )

	#define GLEXT_GLEW_framebuffer_object GLEW_ARB_framebuffer_object

	#define GLEXT_GL_FRAMEBUFFER GL_FRAMEBUFFER
	#define GLEXT_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE
	#define GLEXT_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0

	#define GLEXT_glGenFramebuffers glGenFramebuffers
	#define GLEXT_glDeleteFramebuffers glDeleteFramebuffers
	#define GLEXT_glBindFramebuffer glBindFramebuffer
	#define GLEXT_glFramebufferTexture2D glFramebufferTexture2D
	#define GLEXT_glCheckFramebufferStatus glCheckFramebufferStatus

#elif defined( GLEW_EXT_framebuffer_object )

	#define GLEXT_GLEW_framebuffer_object GLEW_EXT_framebuffer_object

	#define GLEXT_GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
	#define GLEXT_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
	#define GLEXT_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT

	#define GLEXT_glGenFramebuffers glGenFramebuffersEXT
	#define GLEXT_glDeleteFramebuffers glDeleteFramebuffersEXT
	#define GLEXT_glBindFramebuffer glBindFramebufferEXT
	#define GLEXT_glFramebufferTexture2D glFramebufferTexture2DEXT
	#define GLEXT_glCheckFramebufferStatus glCheckFramebufferStatusEXT

#else

	#error You have a version of GLEW that is too old for SFGUI. Please update it.

#endif

// VBO support promoted to core from ARB in 1.5.
#if defined( GLEW_VERSION_1_5 )

	#define GLEXT_GLEW_vertex_buffer_object GLEW_VERSION_1_5

	#define GLEXT_GL_ARRAY_BUFFER GL_ARRAY_BUFFER
	#define GLEXT_GL_ELEMENT_ARRAY_BUFFER GL_ELEMENT_ARRAY_BUFFER
	#define GLEXT_GL_DYNAMIC_DRAW GL_DYNAMIC_DRAW

	#define GLEXT_glGenBuffers glGenBuffers
	#define GLEXT_glDeleteBuffers glDeleteBuffers
	#define GLEXT_glBindBuffer glBindBuffer
	#define GLEXT_glBufferData glBufferData
	#define GLEXT_glBufferSubData glBufferSubData

#elif defined( GLEW_ARB_vertex_buffer_object )

	#define GLEXT_GLEW_vertex_buffer_object GLEW_ARB_vertex_buffer_object

	#define GLEXT_GL_ARRAY_BUFFER GL_ARRAY_BUFFER_ARB
	#define GLEXT_GL_ELEMENT_ARRAY_BUFFER GL_ELEMENT_ARRAY_BUFFER_ARB
	#define GLEXT_GL_DYNAMIC_DRAW GL_DYNAMIC_DRAW_ARB

	#define GLEXT_glGenBuffers glGenBuffersARB
	#define GLEXT_glDeleteBuffers glDeleteBuffersARB
	#define GLEXT_glBindBuffer glBindBufferARB
	#define GLEXT_glBufferData glBufferDataARB
	#define GLEXT_glBufferSubData glBufferSubDataARB

#else

	#error You have a version of GLEW that is too old for SFGUI. Please update it.

#endif

bool VertexBufferRenderer::m_glew_initialized = false;

VertexBufferRenderer::VertexBufferRenderer() :
	m_frame_buffer( 0 ),
	m_frame_buffer_texture( 0 ),
	m_display_list( 0 ),
	m_last_vertex_count( 0 ),
	m_last_index_count( 0 ),
	m_alpha_threshold( 0.f ),
	m_vbo_sync_type( INVALIDATE_ALL ),
	m_vbo_synced( false ),
	m_cull( false ),
	m_use_fbo( false ),
	m_vbo_supported( false ),
	m_fbo_supported( false ) {

	// Make sure we have a valid GL context before messing around
	// with GLEW or else it will report missing extensions sometimes.
	sf::Context context;

	if( !m_glew_initialized ) {
		auto result = glewInit();

		if( result != GLEW_OK ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "GLEW initialization failed: " << glewGetErrorString( result ) << "\n";
#endif
			return;
		}

		m_glew_initialized = true;
	}

	if( GLEXT_GLEW_vertex_buffer_object ) {
		m_vbo_supported = true;

		GLEXT_glGenBuffers( 1, &m_vertex_vbo );
		GLEXT_glGenBuffers( 1, &m_color_vbo );
		GLEXT_glGenBuffers( 1, &m_texture_vbo );
		GLEXT_glGenBuffers( 1, &m_index_vbo );
	}
	else {
#if defined( SFGUI_DEBUG )
		std::cerr << "VBO extension unavailable.\n";
#endif
	}

	if( GLEXT_GLEW_framebuffer_object ) {
		m_fbo_supported = true;
	}
}

VertexBufferRenderer::~VertexBufferRenderer() {
	DestroyFBO();

	if( m_vbo_supported ) {
		GLEXT_glDeleteBuffers( 1, &m_index_vbo );
		GLEXT_glDeleteBuffers( 1, &m_texture_vbo );
		GLEXT_glDeleteBuffers( 1, &m_color_vbo );
		GLEXT_glDeleteBuffers( 1, &m_vertex_vbo );
	}
}

const std::string& VertexBufferRenderer::GetName() const {
	static const std::string name( "Vertex Buffer Renderer" );
	return name;
}

bool VertexBufferRenderer::IsAvailable() {
	// Make sure we have a valid GL context before messing around
	// with GLEW or else it will report missing extensions sometimes.
	sf::Context context;

	if( !m_glew_initialized ) {
		auto result = glewInit();

		if( result != GLEW_OK ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "GLEW initialization failed: " << glewGetErrorString( result ) << "\n";
#endif
			return false;
		}

		m_glew_initialized = true;
	}

	if( GLEXT_GLEW_vertex_buffer_object ) {
		return true;
	}

	return false;
}

void VertexBufferRenderer::DisplayImpl() const {
	if( !m_vbo_supported ) {
		return;
	}

	if( m_alpha_threshold > 0.f ) {
		glAlphaFunc( GL_GREATER, m_alpha_threshold );
		glEnable( GL_ALPHA_TEST );
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
		const_cast<VertexBufferRenderer*>( this )->RefreshVBO();
	}

	if( !m_use_fbo || !m_vbo_synced || m_force_redraw ) {
		// Thanks to color / texture modulation we can draw the entire
		// frame in a single pass by pseudo-disabling the texturing with
		// the help of a white texture ( 1.f * something = something ).
		// Further, we stick all referenced textures into our giant atlas
		// so we don't have to rebind during the draw.

		GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_vertex_vbo );
		glVertexPointer( 2, GL_FLOAT, 0, 0 );

		GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_color_vbo );
		glColorPointer( 4, GL_UNSIGNED_BYTE, 0, 0 );

		GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_texture_vbo );
		glTexCoordPointer( 2, GL_FLOAT, 0, 0 );

		GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, m_index_vbo );

		// Not needed, constantly kept enabled by SFML... -_-
		//glEnableClientState( GL_VERTEX_ARRAY );
		//glEnableClientState( GL_COLOR_ARRAY );
		//glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		if( m_use_fbo ) {
			GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, m_frame_buffer );

			glClear( GL_COLOR_BUFFER_BIT );
		}

		glEnable( GL_SCISSOR_TEST );

		std::size_t current_atlas_page = 0;

		sf::Texture::bind( m_texture_atlas[0].get() );

		for( const auto& batch : m_batches ) {
			auto viewport = batch.viewport;

			if( batch.custom_draw ) {
				sf::Vector2i destination( viewport->GetDestinationOrigin() );
				sf::Vector2u size( viewport->GetSize() );

				glViewport( destination.x, m_window_size.y - destination.y - size.y, size.x, size.y );

				// Draw canvas.
				( *batch.custom_draw_callback )();

				glViewport( 0, 0, m_window_size.x, m_window_size.y );

				sf::Texture::bind( m_texture_atlas[current_atlas_page].get() );
			}
			else {
				if( viewport && ( ( *viewport ) != ( *m_default_viewport ) ) ) {
					auto destination_origin = viewport->GetDestinationOrigin();
					auto size = viewport->GetSize();

					glScissor(
						static_cast<int>( destination_origin.x ),
						m_window_size.y - static_cast<int>( destination_origin.y + size.y ),
						static_cast<int>( size.x ),
						static_cast<int>( size.y )
					);
				}
				else {
					glScissor( 0, 0, m_window_size.x, m_window_size.y );
				}

				if( batch.index_count ) {
					if( batch.atlas_page != current_atlas_page ) {
						current_atlas_page = batch.atlas_page;

						sf::Texture::bind( m_texture_atlas[current_atlas_page].get() );
					}

					glDrawRangeElements(
						GL_TRIANGLES,
						batch.min_index,
						batch.max_index,
						batch.index_count,
						GL_UNSIGNED_INT,
						reinterpret_cast<const GLvoid*>( batch.start_index * sizeof( GLuint ) )
					);
				}
			}
		}

		glDisable( GL_SCISSOR_TEST );


		//glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		//glDisableClientState( GL_COLOR_ARRAY );
		//glDisableClientState( GL_VERTEX_ARRAY );

		// Needed otherwise SFML will blow up...
		GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, 0 );
		GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, 0 );

		if( m_use_fbo ) {
			GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, 0 );

			glCallList( m_display_list );
		}

		m_force_redraw = false;
	}
	else {
		glCallList( m_display_list );
	}

	m_vbo_synced = true;

	if( m_alpha_threshold > 0.f ) {
		glDisable( GL_ALPHA_TEST );
		glAlphaFunc( GL_GREATER, 0.f );
	}
}

void VertexBufferRenderer::RefreshVBO() {
	SortPrimitives();

	std::vector<sf::Vector2f> vertex_data;
	std::vector<sf::Color> color_data;
	std::vector<sf::Vector2f> texture_data;
	std::vector<GLuint> index_data;

	vertex_data.reserve( m_vertex_count );
	color_data.reserve( m_vertex_count );
	texture_data.reserve( m_vertex_count );
	index_data.reserve( m_index_count );

	m_batches.clear();

	m_last_vertex_count = 0;
	m_last_index_count = 0;

	// Default viewport
	Batch current_batch;
	current_batch.viewport = m_default_viewport;
	current_batch.atlas_page = 0;
	current_batch.start_index = 0;
	current_batch.index_count = 0;
	current_batch.min_index = 0;
	current_batch.max_index = static_cast<GLuint>( m_vertex_count - 1 );
	current_batch.custom_draw = false;

	sf::FloatRect window_viewport( 0.f, 0.f, static_cast<float>( m_window_size.x ), static_cast<float>( m_window_size.y ) );

	for( const auto& primitive_ptr : m_primitives ) {
		Primitive* primitive = primitive_ptr.get();

		primitive->SetSynced();

		if( !primitive->IsVisible() ) {
			continue;
		}

		sf::Vector2f position_transform( primitive->GetPosition() );

		auto viewport = primitive->GetViewport();

		std::size_t atlas_page = 0;

		auto viewport_rect = window_viewport;

		// Check if primitive needs to be rendered in a custom viewport.
		if( viewport && ( ( *viewport ) != ( *m_default_viewport ) ) ) {
			sf::Vector2f destination_origin( viewport->GetDestinationOrigin() );
			sf::Vector2f size( viewport->GetSize() );

			position_transform += ( destination_origin - viewport->GetSourceOrigin() );

			if( m_cull ) {
				viewport_rect.left = destination_origin.x;
				viewport_rect.top = destination_origin.y;
				viewport_rect.width = size.x;
				viewport_rect.height = size.y;
			}
		}

		const std::shared_ptr<Signal>& custom_draw_callback( primitive->GetCustomDrawCallback() );

		if( custom_draw_callback ) {
			// Start a new batch.
			current_batch.max_index = m_last_vertex_count ? ( static_cast<GLuint>( m_last_vertex_count ) - 1 ) : 0;
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
			current_batch.min_index = m_last_vertex_count ? ( static_cast<GLuint>( m_last_vertex_count ) - 1 ) : 0;
			current_batch.custom_draw = false;
		}
		else {
			// Process primitive's vertices and indices
			const std::vector<Primitive::Vertex>& vertices( primitive->GetVertices() );
			const std::vector<GLuint>& indices( primitive->GetIndices() );

			sf::Vector2f position( 0.f, 0.f );

			sf::FloatRect bounding_rect( 0.f, 0.f, 0.f, 0.f );

			for( const auto& vertex : vertices ) {
				position.x = vertex.position.x + position_transform.x;
				position.y = vertex.position.y + position_transform.y;

				vertex_data.push_back( position );
				color_data.push_back( vertex.color );

				atlas_page = static_cast<unsigned int>( vertex.texture_coordinate.y ) / m_max_texture_size;

				// Used to normalize texture coordinates.
				sf::Vector2f normalizer( 1.f / static_cast<float>( m_texture_atlas[atlas_page]->getSize().x ), 1.f / static_cast<float>( m_texture_atlas[atlas_page]->getSize().y ) );

				// Normalize SFML's pixel texture coordinates.
				texture_data.push_back( sf::Vector2f( vertex.texture_coordinate.x * normalizer.x, static_cast<float>( static_cast<unsigned int>( vertex.texture_coordinate.y ) % m_max_texture_size ) * normalizer.y ) );

				// Update the bounding rect.
				if( m_cull ) {
					if( position.x < bounding_rect.left ) {
						bounding_rect.width += bounding_rect.left - position.x;
						bounding_rect.left = position.x;
					}
					else if( position.x > bounding_rect.left + bounding_rect.width ) {
						bounding_rect.width = position.x - bounding_rect.left;
					}

					if( position.y < bounding_rect.top ) {
						bounding_rect.height += bounding_rect.top - position.y;
						bounding_rect.top = position.y;
					}
					else if( position.y > bounding_rect.top + bounding_rect.height ) {
						bounding_rect.height = position.y - bounding_rect.top;
					}
				}
			}

			if( m_cull && !viewport_rect.intersects( bounding_rect ) ) {
				vertex_data.resize( m_last_vertex_count );
				color_data.resize( m_last_vertex_count );
				texture_data.resize( m_last_vertex_count );
			}
			else {
				for( const auto& index : indices ) {
					index_data.push_back( m_last_vertex_count + index );
				}

				// Check if we need to start a new batch.
				if( ( ( *viewport ) != ( *current_batch.viewport ) ) || ( atlas_page != current_batch.atlas_page ) ) {
					current_batch.max_index = m_last_vertex_count ? ( static_cast<GLuint>( m_last_vertex_count ) - 1 ) : 0;
					m_batches.push_back( current_batch );

					// Reset current_batch to defaults.
					current_batch.viewport = viewport;
					current_batch.atlas_page = atlas_page;
					current_batch.start_index = m_last_index_count;
					current_batch.index_count = 0;
					current_batch.min_index = m_last_vertex_count ? ( static_cast<GLuint>( m_last_vertex_count ) - 1 ) : 0;
					current_batch.custom_draw = false;
				}

				current_batch.index_count += static_cast<unsigned int>( indices.size() );

				m_last_vertex_count += static_cast<GLsizei>( vertices.size() );
				m_last_index_count += static_cast<GLsizei>( indices.size() );
			}
		}
	}

	current_batch.max_index = m_last_vertex_count ? ( static_cast<GLuint>( m_last_vertex_count ) - 1 ) : 0;
	m_batches.push_back( current_batch );

	if( !vertex_data.empty() && !color_data.empty() && !texture_data.empty() ) {
		if( m_vbo_sync_type & INVALIDATE_VERTEX ) {
			// Sync vertex data
			GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_vertex_vbo );
			GLEXT_glBufferData( GLEXT_GL_ARRAY_BUFFER, vertex_data.size() * sizeof( sf::Vector3f ), 0, GLEXT_GL_DYNAMIC_DRAW );

			if( vertex_data.size() > 0 ) {
				GLEXT_glBufferSubData( GLEXT_GL_ARRAY_BUFFER, 0, vertex_data.size() * sizeof( sf::Vector2f ), &vertex_data[0] );
			}
		}

		if( m_vbo_sync_type & INVALIDATE_COLOR ) {
			// Sync color data
			GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_color_vbo );
			GLEXT_glBufferData( GLEXT_GL_ARRAY_BUFFER, color_data.size() * sizeof( sf::Color ), 0, GLEXT_GL_DYNAMIC_DRAW );

			if( color_data.size() > 0 ) {
				GLEXT_glBufferSubData( GLEXT_GL_ARRAY_BUFFER, 0, color_data.size() * sizeof( sf::Color ), &color_data[0] );
			}
		}

		if( m_vbo_sync_type & INVALIDATE_TEXTURE ) {
			// Sync texture coord data
			GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_texture_vbo );
			GLEXT_glBufferData( GLEXT_GL_ARRAY_BUFFER, texture_data.size() * sizeof( sf::Vector2f ), 0, GLEXT_GL_DYNAMIC_DRAW );

			if( texture_data.size() > 0 ) {
				GLEXT_glBufferSubData( GLEXT_GL_ARRAY_BUFFER, 0, texture_data.size() * sizeof( sf::Vector2f ), &texture_data[0] );
			}
		}

		if( m_vbo_sync_type & INVALIDATE_INDEX ) {
			// Sync index data
			GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, m_index_vbo );
			GLEXT_glBufferData( GLEXT_GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof( GLuint ), 0, GLEXT_GL_DYNAMIC_DRAW );

			if( index_data.size() > 0 ) {
				GLEXT_glBufferSubData( GLEXT_GL_ELEMENT_ARRAY_BUFFER, 0, index_data.size() * sizeof( GLuint ), &index_data[0] );
			}
		}
	}

	m_vbo_sync_type = 0;
}

void VertexBufferRenderer::InvalidateVBO( unsigned char datasets ) {
	m_vbo_sync_type |= datasets;
	m_vbo_synced = false;
}

void VertexBufferRenderer::SetupFBO( unsigned int width, unsigned int height ) {
	if( !m_use_fbo || !width || !height ) {
		DestroyFBO();

		return;
	}

	// Create FBO.
	if( !m_frame_buffer ) {
		GLEXT_glGenFramebuffers( 1, &m_frame_buffer );
	}

	GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, m_frame_buffer );

	// Create FBO texture object.
	if( !m_frame_buffer_texture ) {
		glGenTextures( 1, &m_frame_buffer_texture );
	}

	glBindTexture( GL_TEXTURE_2D, m_frame_buffer_texture );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

	glBindTexture( GL_TEXTURE_2D, 0 );

	GLEXT_glFramebufferTexture2D( GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frame_buffer_texture, 0 );

	// Sanity check.
	auto status = GLEXT_glCheckFramebufferStatus( GLEXT_GL_FRAMEBUFFER );

	if( status != GLEXT_GL_FRAMEBUFFER_COMPLETE ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "GLEXT_glCheckFramebufferStatus() returned error " << status << ", disabling FBO.\n";
#endif

		DestroyFBO();

		m_fbo_supported = false;
		m_use_fbo = false;
	}

	GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, 0 );

	if( m_use_fbo && !m_display_list ) {
		m_display_list = glGenLists( 1 );

		// Do not fear the immediate-mode GL here, we only compile this once.
		glNewList( m_display_list, GL_COMPILE );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();

		glBindTexture( GL_TEXTURE_2D, m_frame_buffer_texture );

		// Hard to believe, but GL_TRIANGLE_STRIP performs better
		// in my tests than GL_QUADS, it seems cards might
		// not always optimize by themselves after all.
		glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2s( 1, 1 );
		glVertex2s( 1, 1 );
		glTexCoord2s( 0, 1 );
		glVertex2s( -1, 1 );
		glTexCoord2s( 1, 0 );
		glVertex2s( 1, -1 );
		glTexCoord2s( 0, 0 );
		glVertex2s( -1, -1 );
		glEnd();

		glBindTexture( GL_TEXTURE_2D, 0 );

		glPopMatrix();
		glMatrixMode( GL_TEXTURE );

		glEndList();
	}
}

void VertexBufferRenderer::DestroyFBO() {
	if( m_display_list ) {
		glDeleteLists( m_display_list, 1 );
	}

	if( m_frame_buffer_texture ) {
		glDeleteTextures( 1, &m_frame_buffer_texture );

		m_frame_buffer_texture = 0;
	}

	if( m_frame_buffer ) {
		GLEXT_glDeleteFramebuffers( 1, &m_frame_buffer );

		m_frame_buffer = 0;
	}
}

void VertexBufferRenderer::TuneAlphaThreshold( float alpha_threshold ) {
	m_alpha_threshold = alpha_threshold;
}

void VertexBufferRenderer::TuneCull( bool enable ) {
	m_cull = enable;
}

void VertexBufferRenderer::TuneUseFBO( bool enable ) {
	if( !m_fbo_supported && enable ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "FBO extension unavailable.\n";
#endif
	}

	m_use_fbo = enable && m_fbo_supported;

	if( m_use_fbo ) {
		SetupFBO( m_window_size.x, m_window_size.y );
	}
	else {
		DestroyFBO();
	}
}

void VertexBufferRenderer::InvalidateImpl( unsigned char datasets ) {
	InvalidateVBO( datasets );
}

void VertexBufferRenderer::InvalidateWindow() {
	SetupFBO( m_window_size.x, m_window_size.y );
}

}
