#include <SFGUI/Canvas.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Container.hpp>
#include <cmath>

namespace sfg {

Canvas::Canvas( bool depth ) :
	m_custom_draw_callback( std::make_shared<Signal>() ),
	m_display_list( 0 ),
	m_depth( depth ),
	m_resize( false )
{
	m_custom_viewport = Renderer::Get().CreateViewport();
	SetViewport( m_custom_viewport );

	m_custom_draw_callback->Connect( std::bind( &Canvas::DrawRenderTexture, this ) );
}

Canvas::~Canvas() {
	if( m_display_list ) {
		glDeleteLists( m_display_list, 1 );
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

	glClearColor( color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f );
	glClear( GL_COLOR_BUFFER_BIT | ( depth ? GL_DEPTH_BUFFER_BIT : 0 ) );
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

	// Matrix mode is currently GL_TEXTURE as set by Renderer.
	// We get ready for a surprise from SFML...
	glPushMatrix();
	sf::Texture::bind( &( m_render_texture->getTexture() ) );

	if( !m_display_list ) {
		m_display_list = glGenLists( 1 );

		if( !m_display_list ) {
#if defined( SFGUI_DEBUG )
			std::cerr << "SFGUI warning: Canvas failed to create OpenGL display list.\n";
#endif
		}

		glNewList( m_display_list, GL_COMPILE );

		// Pop the texture matrix that SFML secretly loaded behind our backs...
		glMatrixMode( GL_TEXTURE );
		glPopMatrix();

		glColor3ub( 255, 255, 255 );

		// Oh the horror... not.
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		glBegin( GL_QUADS );
		glTexCoord2s( 0, 0 );
		glVertex2s( -1, -1 );
		glTexCoord2s( 1, 0 );
		glVertex2s( 1, -1 );
		glTexCoord2s( 1, 1 );
		glVertex2s( 1, 1 );
		glTexCoord2s( 0, 1 );
		glVertex2s( -1, 1 );
		glEnd();
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
		glMatrixMode( GL_TEXTURE );

		glEndList();
	}

	glCallList( m_display_list );
}

}
