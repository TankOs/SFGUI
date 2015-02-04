#include <SFGUI/Canvas.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/GLLoader.hpp>
#include <SFGUI/Renderers/NonLegacyRenderer.hpp>
#include <SFGUI/GLCheck.hpp>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Context.hpp>
#include <cmath>
#include <cassert>

// ARB_vertex_buffer_object
#define GLEXT_vertex_buffer_object sfgogl_ext_ARB_vertex_buffer_object

#define GLEXT_GL_ARRAY_BUFFER GL_ARRAY_BUFFER_ARB
#define GLEXT_GL_STATIC_DRAW GL_STATIC_DRAW_ARB

#define GLEXT_glBindBuffer glBindBufferARB
#define GLEXT_glDeleteBuffers glDeleteBuffersARB
#define GLEXT_glGenBuffers glGenBuffersARB
#define GLEXT_glBufferData glBufferDataARB

// ARB_vertex_program
#define GLEXT_vertex_program sfgogl_ext_ARB_vertex_program

#define GLEXT_GL_PROGRAM_OBJECT GL_PROGRAM_OBJECT_ARB

#define GLEXT_glGetHandle glGetHandleARB
#define GLEXT_glUseProgramObject glUseProgramObjectARB
#define GLEXT_glVertexAttribPointer glVertexAttribPointerARB
#define GLEXT_glEnableVertexAttribArray glEnableVertexAttribArrayARB
#define GLEXT_glDisableVertexAttribArray glDisableVertexAttribArrayARB

// ARB_vertex_shader (ensured by sf::Shader::isAvailable())
#define GLEXT_glGetAttribLocation glGetAttribLocationARB

// ARB_vertex_array_object
#define GLEXT_vertex_array_object sfgogl_ext_ARB_vertex_array_object

#define GLEXT_glIsVertexArray glIsVertexArray
#define GLEXT_glBindVertexArray glBindVertexArray
#define GLEXT_glDeleteVertexArrays glDeleteVertexArrays
#define GLEXT_glGenVertexArrays glGenVertexArrays

namespace {

auto non_legacy_supported = false;

unsigned int GetAttributeLocation( const sf::Shader& shader, std::string name ) {
	auto previous_program = CheckGLError( GLEXT_glGetHandle( GLEXT_GL_PROGRAM_OBJECT ) );

	sf::Shader::bind( &shader );

	auto program = CheckGLError( GLEXT_glGetHandle( GLEXT_GL_PROGRAM_OBJECT ) );

	auto location = CheckGLError( GLEXT_glGetAttribLocation( program, name.c_str() ) );

	CheckGLError( GLEXT_glUseProgramObject( previous_program ) );

#if defined( SFGUI_DEBUG )
	if( location < 0 ) {
		std::cerr << "SFGUI warning: Failed to get location of attribute \"" << name << "\".\n";
	}
#endif

	assert( location >= 0 );

	return static_cast<unsigned int>( location );
}

void WipeStateCache( sf::RenderTarget& target ) {
	// Make SFML disable it's **************** vertex cache without us
	// having to call ResetGLStates() and disturbing OpenGL needlessly.
	// This would be sooo much easier if we could somehow set
	// myCache.UseVertexCache = false;
	// in window by ourself every frame.
	// SFML doesn't like to play nice with other VBO / Vertex Array
	// users, that's for sure... It assumes that the array pointers
	// don't get changed between calls to Draw() unless ResetGLStates()
	// is explicitly called in between. Why do we need to call OpenGL
	// 10+ times just to reset something this simple? No logic.

	//static sf::VertexArray resetter_array( sf::TrianglesStrip, 5 );
	//window.Draw( resetter_array );

	// Or... even more evil... use memset to blank the StatesCache of
	// the RenderWindow with zeros. Thankfully, because we are using
	// the data structures directly from the SFML headers, if Laurent
	// decides to change their size one day we won't even notice.
	struct StatesCache {
		bool glStatesSet;
		bool ViewChanged;
		sf::BlendMode LastBlendMode;
		sf::Uint64 LastTextureId;
		bool UseVertexCache;
		sf::Vertex VertexCache[4];
	};

	// All your cache are belong to us.
	memset( reinterpret_cast<char*>( &target ) + sizeof( sf::RenderTarget ) - sizeof( StatesCache ) + 1, 0, sizeof( StatesCache ) - 1 );

	// This is to make it forget it's cached viewport.
	// Seriously... caching the viewport? Come on...
	memset( reinterpret_cast<char*>( &target ) + sizeof( sf::RenderTarget ) - sizeof( StatesCache ) + 1, 1, 1 );

	// Since we wiped SFML's cache of its bound texture, we
	// should make sure that it stays coherent with reality :).
	sf::Texture::bind( 0 );
}

}

namespace sfg {

Canvas::Canvas( bool depth ) :
	m_custom_draw_callback( std::make_shared<Signal>() ),
	m_depth( depth ),
	m_resize( false )
{
	m_custom_viewport = Renderer::Get().CreateViewport();
	SetViewport( m_custom_viewport );

	m_custom_draw_callback->Connect( std::bind( &Canvas::DrawRenderTexture, this ) );

	static auto checked = false;

	if( !checked ) {
		non_legacy_supported = NonLegacyRenderer::IsAvailable();

		checked = true;
	}
}

Canvas::~Canvas() {
	if( NonLegacyRenderer::IsAvailable() ) {
		CheckGLError( GLEXT_glDeleteVertexArrays( 1, &m_vao ) );
		CheckGLError( GLEXT_glDeleteBuffers( 1, &m_vbo ) );
	}

	if( m_display_list ) {
		CheckGLError( glDeleteLists( m_display_list, 1 ) );
	}
}

Canvas::Ptr Canvas::Create( bool depth ) {
	return Ptr( new Canvas( depth ) );
}

std::unique_ptr<RenderQueue> Canvas::InvalidateImpl() const {
	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	queue->Add(
		Renderer::Get().CreateGLCanvas(
			m_custom_draw_callback
		)
	);

	// Override whatever viewport our parents want us to use.
	// Yes... we are very naughty aren't we...
	queue->SetViewport( m_custom_viewport );

	return queue;
}

sf::Vector2f Canvas::CalculateRequisition() {
	return sf::Vector2f( 0.f, 0.f );
}

void Canvas::HandleSizeChange() {
	auto allocation = GetAllocation();

	m_custom_viewport->SetSize(
		sf::Vector2f(
			std::floor( allocation.width + .5f ),
			std::floor( allocation.height + .5f )
		)
	);

	if( m_render_texture ) {
		m_resize = true;
	}

	Invalidate();
}

void Canvas::HandleAbsolutePositionChange() {
	auto position = Widget::GetAbsolutePosition();

	auto parent = GetParent();

	sf::Vector2f parent_position( 0.f, 0.f );

	while( parent ) {
		if( parent->GetName() == "Viewport" ) {
			// Try to get the first ancestor of the viewport that is not a viewport itself.
			// Add up all allocations while searching.
			Container::PtrConst viewport_parent = parent->GetParent();

			while( viewport_parent && viewport_parent->GetName() == "Viewport" ) {
				parent_position += sf::Vector2f( viewport_parent->GetAllocation().left, viewport_parent->GetAllocation().top );

				viewport_parent = viewport_parent->GetParent();
			}

			if( !viewport_parent || ( viewport_parent->GetName() == "Viewport" ) ) {
				parent_position = sf::Vector2f( 0.f, 0.f );
				break;
			}

			parent_position += viewport_parent->GetAbsolutePosition();
			parent_position += sf::Vector2f( parent->GetAllocation().left, parent->GetAllocation().top );

			break;
		}

		parent = parent->GetParent();
	}

	m_custom_viewport->SetDestinationOrigin(
		sf::Vector2f(
			std::floor( parent_position.x + position.x + .5f ),
			std::floor( parent_position.y + position.y + .5f )
		)
	);

	Invalidate();
}

const std::string& Canvas::GetName() const {
	static const std::string name( "Canvas" );
	return name;
}

void Canvas::Redraw() const {
	sfg::Renderer::Get().Redraw();
}

void Canvas::Clear( const sf::Color& color, bool depth ) {
	auto allocation = GetAllocation();

	if( !m_render_texture ) {
		// Make sure there is a non-internal/shared context active.
		// After 8 hours of debugging, I found out that if you deactivate the
		// currently active context and construct a sf::RenderTexture, it leads
		// to the sf::RenderTexture's internal OpenGL texture being created
		// in a "corrupted" context leading to OpenGL possibly generating the
		// same texture IDs that it already previously generated and are still in
		// use. Since 2 textures share the same ID, when you draw to the
		// sf::RenderTexture it actually overwrites whatever was in the other
		// normally created texture. This leads to very exotic looking sf::Sprites,
		// not for the end-user. SFML context management strikes again.
		sf::Context context;

		m_render_texture = std::make_shared<sf::RenderTexture>();

		if( !m_render_texture->create( static_cast<unsigned int>( std::floor( allocation.width + .5f ) ), static_cast<unsigned int>( std::floor( allocation.height + .5f ) ), m_depth ) ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "SFGUI warning: Canvas failed to create internal SFML RenderTexture.\n";
#endif
		}
	}
	else if( m_resize ) {
		if( !m_render_texture->create( static_cast<unsigned int>( std::floor( allocation.width + .5f ) ), static_cast<unsigned int>( std::floor( allocation.height + .5f ) ), m_depth ) ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "SFGUI warning: Canvas failed to create internal SFML RenderTexture.\n";
#endif
		}
	}

	m_resize = false;

	m_render_texture->setActive( true );

	WipeStateCache( *m_render_texture );

	CheckGLError( glClearColor( color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f ) );
	CheckGLError( glClear( GL_COLOR_BUFFER_BIT | ( depth ? GL_DEPTH_BUFFER_BIT : 0 ) ) );
}

void Canvas::Display() const {
	if( !m_render_texture ) {
		return;
	}

	m_render_texture->display();

	Redraw();
}

void Canvas::Draw( const sf::Drawable& drawable, const sf::RenderStates& states ) {
	m_render_texture->draw( drawable, states );
}

void Canvas::Draw( const sf::Vertex* vertices, unsigned int vertex_count, sf::PrimitiveType type, const sf::RenderStates& states ) {
	m_render_texture->draw( vertices, vertex_count, type, states );
}

void Canvas::SetView( const sf::View& view ) {
	if( !m_render_texture ) {
		return;
	}

	m_render_texture->setView( view );
}

sf::View Canvas::GetView() const {
	if( !m_render_texture ) {
		return sf::View();
	}

	return m_render_texture->getView();
}

void Canvas::Bind() {
	auto allocation = GetAllocation();

	if( !m_render_texture ) {
		// Make sure there is a non-internal/shared context active.
		// After 8 hours of debugging, I found out that if you deactivate the
		// currently active context and construct a sf::RenderTexture, it leads
		// to the sf::RenderTexture's internal OpenGL texture being created
		// in a "corrupted" context leading to OpenGL possibly generating the
		// same texture IDs that it already previously generated and are still in
		// use. Since 2 textures share the same ID, when you draw to the
		// sf::RenderTexture it actually overwrites whatever was in the other
		// normally created texture. This leads to very exotic looking sf::Sprites,
		// not for the end-user. SFML context management strikes again.
		sf::Context context;

		m_render_texture = std::make_shared<sf::RenderTexture>();

		if( !m_render_texture->create( static_cast<unsigned int>( std::floor( allocation.width + .5f ) ), static_cast<unsigned int>( std::floor( allocation.height + .5f ) ), m_depth ) ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "SFGUI warning: Canvas failed to create internal SFML RenderTexture.\n";
#endif
		}
	}
	else if( m_resize ) {
		if( !m_render_texture->create( static_cast<unsigned int>( std::floor( allocation.width + .5f ) ), static_cast<unsigned int>( std::floor( allocation.height + .5f ) ), m_depth ) ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "SFGUI warning: Canvas failed to create internal SFML RenderTexture.\n";
#endif
		}
	}

	m_resize = false;

	m_render_texture->setActive( true );
}

void Canvas::Unbind() {
	if( !m_render_texture ) {
		return;
	}

	m_render_texture->setActive( false );
}

void Canvas::DrawRenderTexture() {
	if( !m_render_texture ) {
		return;
	}

	if( !non_legacy_supported ) {
		// Matrix mode is currently GL_TEXTURE as set by Renderer.
		// We get ready for a surprise from SFML...
		CheckGLError( glPushMatrix() );
		sf::Texture::bind( &( m_render_texture->getTexture() ) );

		if( !m_display_list ) {
			m_display_list = glGenLists( 1 );

			if( !m_display_list ) {
#if defined( SFGUI_DEBUG )
				std::cerr << "SFGUI warning: Canvas failed to create OpenGL display list.\n";
#endif
			}

			CheckGLError( glNewList( m_display_list, GL_COMPILE ) );

			// Pop the texture matrix that SFML secretly loaded behind our backs...
			CheckGLError( glMatrixMode( GL_TEXTURE ) );
			CheckGLError( glPopMatrix() );

			CheckGLError( glColor3ub( 255, 255, 255 ) );

			// Oh the horror... not.
			CheckGLError( glMatrixMode( GL_MODELVIEW ) );
			CheckGLError( glPushMatrix() );
			CheckGLError( glLoadIdentity() );
			CheckGLError( glMatrixMode( GL_PROJECTION ) );
			CheckGLError( glPushMatrix() );
			CheckGLError( glLoadIdentity() );
			CheckGLError( glBegin( GL_QUADS ) );
			CheckGLError( glTexCoord2s( 0, 0 ) );
			CheckGLError( glVertex2s( -1, -1 ) );
			CheckGLError( glTexCoord2s( 1, 0 ) );
			CheckGLError( glVertex2s( 1, -1 ) );
			CheckGLError( glTexCoord2s( 1, 1 ) );
			CheckGLError( glVertex2s( 1, 1 ) );
			CheckGLError( glTexCoord2s( 0, 1 ) );
			CheckGLError( glVertex2s( -1, 1 ) );
			CheckGLError( glEnd() );
			CheckGLError( glPopMatrix() );
			CheckGLError( glMatrixMode( GL_MODELVIEW ) );
			CheckGLError( glPopMatrix() );
			CheckGLError( glMatrixMode( GL_TEXTURE ) );

			CheckGLError( glEndList() );
		}

		CheckGLError( glCallList( m_display_list ) );
	}
	else {
		// Non-Legacy
		if( !m_shader ) {
			SetupShader();
		}

		if( !non_legacy_supported ) {
			// Fall back to legacy starting in this frame.
			DrawRenderTexture();
			return;
		}

		if( !m_vbo ) {
			SetupVBO();
		}

		auto is_vertex_array = CheckGLError( GLEXT_glIsVertexArray( m_vao ) );

		if( !is_vertex_array ) {
			SetupVAO();
		}

		struct Texture : sf::GlResource {
			sf::Vector2u unused1;
			sf::Vector2u unused2;
			unsigned int unused3;
			bool unused4;
			bool unused5;
			mutable bool pixels_flipped;
			sf::Uint64 unused6;
		};

		// Just so that SFML doesn't mess with the texture matrix.
		const_cast<Texture*>( reinterpret_cast<const Texture*>( &( m_render_texture->getTexture() ) ) )->pixels_flipped = false;

		sf::Shader::bind( m_shader.get() );

		CheckGLError( GLEXT_glBindVertexArray( m_vao ) );

		CheckGLError( glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );

		CheckGLError( GLEXT_glBindVertexArray( 0 ) );

		sf::Shader::bind( nullptr );
	}
}

void Canvas::SetupVBO() {
	if( m_vbo ) {
		return;
	}

	CheckGLError( GLEXT_glGenBuffers( 1, &m_vbo ) );
	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_vbo ) );

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

void Canvas::SetupVAO() {
	CheckGLError( GLEXT_glGenVertexArrays( 1, &m_vao ) );
	CheckGLError( GLEXT_glBindVertexArray( m_vao ) );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, m_vbo ) );

	CheckGLError( GLEXT_glEnableVertexAttribArray( m_vertex_location ) );
	CheckGLError( GLEXT_glEnableVertexAttribArray( m_texture_coordinate_location ) );

	CheckGLError( GLEXT_glVertexAttribPointer( m_vertex_location, 2, GL_BYTE, GL_FALSE, 4 * sizeof( GLbyte ), reinterpret_cast<GLvoid*>( 0 ) ) );
	CheckGLError( GLEXT_glVertexAttribPointer( m_texture_coordinate_location, 2, GL_BYTE, GL_FALSE, 4 * sizeof( GLbyte ), reinterpret_cast<GLvoid*>( 2 * sizeof( GLbyte ) ) ) );

	CheckGLError( GLEXT_glBindVertexArray( 0 ) );

	CheckGLError( GLEXT_glDisableVertexAttribArray( m_texture_coordinate_location ) );
	CheckGLError( GLEXT_glDisableVertexAttribArray( m_vertex_location ) );

	CheckGLError( GLEXT_glBindBuffer( GLEXT_GL_ARRAY_BUFFER, 0 ) );
}

void Canvas::SetupShader() {
	m_shader = std::unique_ptr<sf::Shader>( new sf::Shader );

	auto load_result = m_shader->loadFromMemory(
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

	if( !load_result ) {
		non_legacy_supported = false;

		m_shader.reset();

		return;
	}

	m_vertex_location = GetAttributeLocation( *m_shader, "vertex" );
	m_texture_coordinate_location = GetAttributeLocation( *m_shader, "texture_coordinate" );

	m_shader->setParameter( "texture0", m_render_texture->getTexture() );
}

}
