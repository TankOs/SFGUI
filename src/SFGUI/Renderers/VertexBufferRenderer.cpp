// Needs to be included before GLLoader for NOMINMAX
#include <SFGUI/Config.hpp>

// Needs to be included before OpenGL (so anything else)
#include <SFGUI/GLLoader.hpp>

// X headers define None which is used by SFML's window style.
#undef None

#include <SFGUI/Renderers/VertexBufferRenderer.hpp>
#include <SFGUI/RendererBatch.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Signal.hpp>
#include <SFGUI/Primitive.hpp>
#include <SFGUI/PrimitiveVertex.hpp>
#include <SFGUI/GLCheck.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/System/Vector3.hpp>

#define GLEXT_framebuffer_object sfgogl_ext_EXT_framebuffer_object

#define GLEXT_GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
#define GLEXT_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
#define GLEXT_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT

#define GLEXT_glGenFramebuffers glGenFramebuffersEXT
#define GLEXT_glDeleteFramebuffers glDeleteFramebuffersEXT
#define GLEXT_glBindFramebuffer glBindFramebufferEXT
#define GLEXT_glFramebufferTexture2D glFramebufferTexture2DEXT
#define GLEXT_glCheckFramebufferStatus glCheckFramebufferStatusEXT

#define GLEXT_vertex_buffer_object sfgogl_ext_ARB_vertex_buffer_object

#define GLEXT_GL_ARRAY_BUFFER GL_ARRAY_BUFFER_ARB
#define GLEXT_GL_ELEMENT_ARRAY_BUFFER GL_ELEMENT_ARRAY_BUFFER_ARB
#define GLEXT_GL_DYNAMIC_DRAW GL_DYNAMIC_DRAW_ARB

#define GLEXT_glGenBuffers glGenBuffersARB
#define GLEXT_glDeleteBuffers glDeleteBuffersARB
#define GLEXT_glBindBuffer glBindBufferARB
#define GLEXT_glBufferData glBufferDataARB
#define GLEXT_glBufferSubData glBufferSubDataARB

namespace {

bool gl_initialized = false;

}

namespace sfg {

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
	// with GLLoader or else it will report missing extensions sometimes.
	sf::Context context;

	if( !gl_initialized ) {
		auto result = sfgogl_LoadFunctions() - sfgogl_LOAD_SUCCEEDED;

		if( result ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "GL extension initialization failed. Missing " << result << " functions.\n";
#endif
			return;
		}

		gl_initialized = true;
	}

	if( GLEXT_vertex_buffer_object ) {
		m_vbo_supported = true;

		CheckGLError( GLEXT_glGenBuffers( 1, &m_vertex_vbo ) );
		CheckGLError( GLEXT_glGenBuffers( 1, &m_color_vbo ) );
		CheckGLError( GLEXT_glGenBuffers( 1, &m_texture_vbo ) );
		CheckGLError( GLEXT_glGenBuffers( 1, &m_index_vbo ) );
	}
	else {
#if defined( SFGUI_DEBUG )
		std::cerr << "VBO extension unavailable.\n";
#endif
	}

	if( GLEXT_framebuffer_object ) {
		m_fbo_supported = true;
	}
}

VertexBufferRenderer::~VertexBufferRenderer() {
	sf::Context context;

	DestroyFBO();

	if( m_vbo_supported ) {
		CheckGLError( GLEXT_glDeleteBuffers( 1, &m_index_vbo ) );
		CheckGLError( GLEXT_glDeleteBuffers( 1, &m_texture_vbo ) );
		CheckGLError( GLEXT_glDeleteBuffers( 1, &m_color_vbo ) );
		CheckGLError( GLEXT_glDeleteBuffers( 1, &m_vertex_vbo ) );
	}
}

VertexBufferRenderer::Ptr VertexBufferRenderer::Create() {
	return Ptr( new VertexBufferRenderer );
}

const std::string& VertexBufferRenderer::GetName() const {
	static const std::string name( "Vertex Buffer Renderer" );
	return name;
}

bool VertexBufferRenderer::IsAvailable() {
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
		return true;
	}

	return false;
}

void VertexBufferRenderer::Display( sf::Window& target ) const {
	m_window_size = static_cast<sf::Vector2i>( target.getSize() );

	target.setActive( true );

	CheckGLError( glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT ) );
	CheckGLError( glPushAttrib( GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT ) );

	// Since we have no idea what the attribute environment
	// of the user looks like, we need to pretend to be SFML
	// by setting up it's GL attribute environment.
	CheckGLError( glEnable( GL_TEXTURE_2D ) );
	CheckGLError( glEnable( GL_BLEND ) );
	CheckGLError( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );

	CheckGLError( glEnableClientState( GL_VERTEX_ARRAY ) );
	CheckGLError( glEnableClientState( GL_COLOR_ARRAY ) );
	CheckGLError( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );

	DisplayImpl();

	CheckGLError( glPopAttrib() );
	CheckGLError( glPopClientAttrib() );
}

void VertexBufferRenderer::Display( sf::RenderWindow& target ) const {
	m_window_size = static_cast<sf::Vector2i>( target.getSize() );

	target.setActive( true );

	DisplayImpl();

	WipeStateCache( target );
}

void VertexBufferRenderer::Display( sf::RenderTexture& target ) const {
	m_window_size = static_cast<sf::Vector2i>( target.getSize() );

	target.setActive( true );

	DisplayImpl();

	WipeStateCache( target );
}

void VertexBufferRenderer::DisplayImpl() const {
	if( !m_vbo_supported ) {
		return;
	}

	CheckGLError( glMatrixMode( GL_MODELVIEW ) );
	CheckGLError( glPushMatrix() );
	CheckGLError( glLoadIdentity() );

	CheckGLError( glMatrixMode( GL_PROJECTION ) );
	CheckGLError( glPushMatrix() );
	CheckGLError( glLoadIdentity() );

	// When SFML dies (closes) it sets the window size to 0 for some reason.
	// That then causes glOrtho errors.

	// SFML doesn't seem to bother updating the OpenGL viewport when
	// it's window resizes and nothing is drawn directly through SFML...

	if( m_last_window_size != m_window_size ) {
		CheckGLError( glViewport( 0, 0, m_window_size.x, m_window_size.y ) );

		m_last_window_size = m_window_size;

		if( m_window_size.x && m_window_size.y ) {
			const_cast<VertexBufferRenderer*>( this )->Invalidate( INVALIDATE_VERTEX | INVALIDATE_TEXTURE );

			const_cast<VertexBufferRenderer*>( this )->SetupFBO( m_window_size.x, m_window_size.y );
		}
	}

	CheckGLError( glOrtho( 0.0f, static_cast<GLdouble>( m_window_size.x ? m_window_size.x : 1 ), static_cast<GLdouble>( m_window_size.y ? m_window_size.y : 1 ), 0.0f, -1.0f, 64.0f ) );

	CheckGLError( glMatrixMode( GL_TEXTURE ) );
	CheckGLError( glPushMatrix() );
	CheckGLError( glLoadIdentity() );

	if( m_alpha_threshold > 0.f ) {
		CheckGLError( glAlphaFunc( GL_GREATER, m_alpha_threshold ) );
		CheckGLError( glEnable( GL_ALPHA_TEST ) );
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

		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_vertex_vbo ) );
		CheckGLError( glVertexPointer( 2, GL_FLOAT, 0, 0 ) );

		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_color_vbo ) );
		CheckGLError( glColorPointer( 4, GL_UNSIGNED_BYTE, 0, 0 ) );

		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_texture_vbo ) );
		CheckGLError( glTexCoordPointer( 2, GL_FLOAT, 0, 0 ) );

		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, m_index_vbo ) );

		// Not needed, constantly kept enabled by SFML... -_-
		//CheckGLError( glEnableClientState( GL_VERTEX_ARRAY ) );
		//CheckGLError( glEnableClientState( GL_COLOR_ARRAY ) );
		//CheckGLError( glEnableClientState( GL_TEXTURE_COORD_ARRAY ) );

		if( m_use_fbo ) {
			CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, m_frame_buffer ) );

			CheckGLError( glClear( GL_COLOR_BUFFER_BIT ) );
		}

		CheckGLError( glEnable( GL_SCISSOR_TEST ) );

		auto current_atlas_page = 0;

		sf::Texture::bind( m_texture_atlas[0].get() );

		for( const auto& batch : m_batches ) {
			auto viewport = batch.viewport;

			if( batch.custom_draw ) {
				auto destination = static_cast<sf::Vector2i>( viewport->GetDestinationOrigin() );
				auto size = static_cast<sf::Vector2i>( viewport->GetSize() );

				CheckGLError( glViewport( destination.x, m_window_size.y - destination.y - size.y, size.x, size.y ) );

				// Draw canvas.
				( *batch.custom_draw_callback )();

				CheckGLError( glViewport( 0, 0, m_window_size.x, m_window_size.y ) );

				sf::Texture::bind( m_texture_atlas[static_cast<std::size_t>( current_atlas_page )].get() );
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

						sf::Texture::bind( m_texture_atlas[static_cast<std::size_t>( current_atlas_page )].get() );
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


		//CheckGLError( glDisableClientState( GL_TEXTURE_COORD_ARRAY ) );
		//CheckGLError( glDisableClientState( GL_COLOR_ARRAY ) );
		//CheckGLError( glDisableClientState( GL_VERTEX_ARRAY ) );

		// Needed otherwise SFML will blow up...
		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ELEMENT_ARRAY_BUFFER, 0 ) );
		CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, 0 ) );

		if( m_use_fbo ) {
			CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, 0 ) );

			CheckGLError( glCallList( m_display_list ) );
		}

		m_force_redraw = false;
	}
	else {
		CheckGLError( glCallList( m_display_list ) );
	}

	m_vbo_synced = true;

	if( m_alpha_threshold > 0.f ) {
		CheckGLError( glDisable( GL_ALPHA_TEST ) );
		CheckGLError( glAlphaFunc( GL_GREATER, 0.f ) );
	}

	CheckGLError( glPopMatrix() );

	CheckGLError( glMatrixMode( GL_PROJECTION ) );
	CheckGLError( glPopMatrix() );

	CheckGLError( glMatrixMode( GL_MODELVIEW ) );
	CheckGLError( glPopMatrix() );
}

void VertexBufferRenderer::RefreshVBO() {
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

	sf::FloatRect window_viewport( 0.f, 0.f, static_cast<float>( m_window_size.x ), static_cast<float>( m_window_size.y ) );

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
				viewport_rect.left = destination_origin.x;
				viewport_rect.top = destination_origin.y;
				viewport_rect.width = size.x;
				viewport_rect.height = size.y;
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

			sf::FloatRect bounding_rect( 0.f, 0.f, 0.f, 0.f );

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

void VertexBufferRenderer::InvalidateVBO( unsigned char datasets ) {
	m_vbo_sync_type |= datasets;
	m_vbo_synced = false;
}

void VertexBufferRenderer::SetupFBO( int width, int height ) {
	if( !m_use_fbo || !width || !height ) {
		DestroyFBO();

		return;
	}

	// Create FBO.
	if( !m_frame_buffer ) {
		CheckGLError( GLEXT_glGenFramebuffers( 1, &m_frame_buffer ) );
	}

	CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, m_frame_buffer ) );

	// Create FBO texture object.
	if( !m_frame_buffer_texture ) {
		CheckGLError( glGenTextures( 1, &m_frame_buffer_texture ) );
	}

	CheckGLError( glBindTexture( GL_TEXTURE_2D, m_frame_buffer_texture ) );

	CheckGLError( glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	CheckGLError( glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	CheckGLError( glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	CheckGLError( glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );

	CheckGLError( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 ) );

	CheckGLError( glBindTexture( GL_TEXTURE_2D, 0 ) );

	CheckGLError( GLEXT_glFramebufferTexture2D( GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frame_buffer_texture, 0 ) );

	// Sanity check.
	auto status = CheckGLError( GLEXT_glCheckFramebufferStatus( GLEXT_GL_FRAMEBUFFER ) );

	if( status != GLEXT_GL_FRAMEBUFFER_COMPLETE ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "GLEXT_glCheckFramebufferStatus() returned error " << status << ", disabling FBO.\n";
#endif

		DestroyFBO();

		m_fbo_supported = false;
		m_use_fbo = false;
	}

	CheckGLError( GLEXT_glBindFramebuffer( GLEXT_GL_FRAMEBUFFER, 0 ) );

	if( m_use_fbo && !m_display_list ) {
		m_display_list = glGenLists( 1 );

		// Do not fear the immediate-mode GL here, we only compile this once.
		CheckGLError( glNewList( m_display_list, GL_COMPILE ) );

		CheckGLError( glMatrixMode( GL_PROJECTION ) );
		CheckGLError( glPushMatrix() );
		CheckGLError( glLoadIdentity() );

		CheckGLError( glBindTexture( GL_TEXTURE_2D, m_frame_buffer_texture ) );

		// Hard to believe, but GL_TRIANGLE_STRIP performs better
		// in my tests than GL_QUADS, it seems cards might
		// not always optimize by themselves after all.
		CheckGLError( glBegin( GL_TRIANGLE_STRIP ) );
		CheckGLError( glTexCoord2s( 1, 1 ) );
		CheckGLError( glVertex2s( 1, 1 ) );
		CheckGLError( glTexCoord2s( 0, 1 ) );
		CheckGLError( glVertex2s( -1, 1 ) );
		CheckGLError( glTexCoord2s( 1, 0 ) );
		CheckGLError( glVertex2s( 1, -1 ) );
		CheckGLError( glTexCoord2s( 0, 0 ) );
		CheckGLError( glVertex2s( -1, -1 ) );
		CheckGLError( glEnd() );

		CheckGLError( glBindTexture( GL_TEXTURE_2D, 0 ) );

		CheckGLError( glPopMatrix() );
		CheckGLError( glMatrixMode( GL_TEXTURE ) );

		CheckGLError( glEndList() );
	}
}

void VertexBufferRenderer::DestroyFBO() {
	if( m_display_list ) {
		CheckGLError( glDeleteLists( m_display_list, 1 ) );
	}

	if( m_frame_buffer_texture ) {
		CheckGLError( glDeleteTextures( 1, &m_frame_buffer_texture ) );

		m_frame_buffer_texture = 0;
	}

	if( m_frame_buffer ) {
		CheckGLError( GLEXT_glDeleteFramebuffers( 1, &m_frame_buffer ) );

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

}
