#include <GL/glew.h>
#include <SFGUI/ProjectO.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <cmath>
#include <cstring>

namespace sfg {

ProjectO::ProjectO() :
	m_last_vertex_count( 0 ),
	m_alpha_threshold( 0.f ),
	m_vbo_synced( false ),
	m_depth_test( false ),
	m_preblend( false ) {
	glGenBuffers( 1, &m_vertex_vbo );
	glGenBuffers( 1, &m_color_vbo );
	glGenBuffers( 1, &m_texture_vbo );

	// Load our "no texture" pseudo-texture.
	sf::Image image;
	image.Create( 2, 2, sf::Color::White );
	LoadImage( image );
}

ProjectO::~ProjectO() {
	glDeleteBuffers( 1, &m_texture_vbo );
	glDeleteBuffers( 1, &m_color_vbo );
	glDeleteBuffers( 1, &m_vertex_vbo );
}

ProjectO::Primitive::Primitive() :
	position( sf::Vector2f( 0.f, 0.f ) ),
	layer( 0.f ),
	level( 0 ),
	synced( false ),
	visible( true )
{
}

void ProjectO::Primitive::Add( const Primitive& primitive ) {
	std::size_t vertice_count = primitive.vertices.size();

	for( std::size_t index = 0; index < vertice_count; ++index ) {
		vertices.push_back( primitive.vertices[index] );
	}
}

ProjectO::Viewport::Viewport() :
	source_origin( sf::Vector2f( 0.f, 0.f ) ),
	destination_origin( sf::Vector2f( 0.f, 0.f ) ),
	size( sf::Vector2f( 0.f, 0.f ) )
{
}

void ProjectO::Viewport::Apply() {

}

ProjectO::ViewportPtr ProjectO::CreateViewport() {
	ViewportPtr viewport( new Viewport );
	ViewportWeakPtr weak_viewport( viewport );

	m_viewports.push_back( weak_viewport );

	return viewport;
}

ProjectO::PrimitivePtr ProjectO::CreateText( const sf::Text& text ) {
	PrimitivePtr primitive( new Primitive );

	const sf::Font& font = text.GetFont();
	unsigned int character_size = text.GetCharacterSize();
	const sf::Color& color = text.GetColor();

	sf::Vector2f atlas_offset = LoadFont( font, character_size );

	const sf::String& str = text.GetString();
	std::size_t length = str.GetSize();

	float horizontal_spacing = static_cast<float>( font.GetGlyph( L' ', character_size, false ).Advance );
	float vertical_spacing = static_cast<float>( font.GetLineSpacing( character_size ) );
	sf::Vector2f position( std::floor( text.GetPosition().x + .5f ), std::floor( text.GetPosition().y + static_cast<float>( character_size ) + .5f ) );

	const static float tab_spaces = 2.f;

	sf::Uint32 previous_character = 0;

	for( std::size_t index = 0; index < length; ++index ) {
		sf::Uint32 current_character = str[index];

		position.x += static_cast<float>( font.GetKerning( previous_character, current_character, character_size ) );

		switch( current_character ) {
			case L' ':
				position.x += horizontal_spacing;
				continue;
			case L'\t':
				position.x += horizontal_spacing * tab_spaces;
				continue;
			case L'\n':
				position.y += vertical_spacing;
				position.x = 0.f;
				continue;
			case L'\v':
				position.y += vertical_spacing * tab_spaces;
				continue;
			default:
				break;
		}

		const sf::Glyph& glyph = font.GetGlyph( current_character, character_size, false );

		Primitive::Vertex vertex0;
		Primitive::Vertex vertex1;
		Primitive::Vertex vertex2;
		Primitive::Vertex vertex3;

		vertex0.position = position + sf::Vector2f( static_cast<float>( glyph.Bounds.Left ), static_cast<float>( glyph.Bounds.Top ) );
		vertex1.position = position + sf::Vector2f( static_cast<float>( glyph.Bounds.Left ), static_cast<float>( glyph.Bounds.Top + glyph.Bounds.Height ) );
		vertex2.position = position + sf::Vector2f( static_cast<float>( glyph.Bounds.Left + glyph.Bounds.Width ), static_cast<float>( glyph.Bounds.Top ) );
		vertex3.position = position + sf::Vector2f( static_cast<float>( glyph.Bounds.Left + glyph.Bounds.Width ), static_cast<float>( glyph.Bounds.Top + glyph.Bounds.Height ) );

		vertex0.color = color;
		vertex1.color = color;
		vertex2.color = color;
		vertex3.color = color;

		// Let SFML cast the Rect for us.
		sf::FloatRect texture_rect( glyph.TextureRect );

		vertex0.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.Left, texture_rect.Top );
		vertex1.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.Left, texture_rect.Top + texture_rect.Height );
		vertex2.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.Left + texture_rect.Width, texture_rect.Top );
		vertex3.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.Left + texture_rect.Width, texture_rect.Top + texture_rect.Height );

		// Make sure we get a degenerate triangle after every primitive.
		// Widely available alternative to primitive restart.
		primitive->vertices.push_back( vertex0 );
		primitive->vertices.push_back( vertex0 );
		primitive->vertices.push_back( vertex1 );
		primitive->vertices.push_back( vertex2 );
		primitive->vertices.push_back( vertex3 );
		primitive->vertices.push_back( vertex3 );

		position.x += static_cast<float>( glyph.Advance );

		previous_character = current_character;
	}

	m_primitives.push_back( primitive );

	return primitive;
}

ProjectO::PrimitivePtr ProjectO::CreateQuad( const sf::Vector2f& top_left, const sf::Vector2f& bottom_left,
                                             const sf::Vector2f& bottom_right, const sf::Vector2f& top_right,
                                             const sf::Color& color ) {
	PrimitivePtr primitive( new Primitive );

	Primitive::Vertex vertex0;
	Primitive::Vertex vertex1;
	Primitive::Vertex vertex2;
	Primitive::Vertex vertex3;

	vertex0.position = sf::Vector2f( std::floor( top_left.x + .5f ), std::floor( top_left.y + .5f ) );
	vertex1.position = sf::Vector2f( std::floor( bottom_left.x + .5f ), std::floor( bottom_left.y + .5f ) );
	vertex2.position = sf::Vector2f( std::floor( top_right.x + .5f ), std::floor( top_right.y + .5f ) );
	vertex3.position = sf::Vector2f( std::floor( bottom_right.x + .5f ), std::floor( bottom_right.y + .5f ) );

	vertex0.color = color;
	vertex1.color = color;
	vertex2.color = color;
	vertex3.color = color;

	vertex0.texture_coordinate = sf::Vector2f( 0.f, 0.f );
	vertex1.texture_coordinate = sf::Vector2f( 0.f, 1.f );
	vertex2.texture_coordinate = sf::Vector2f( 1.f, 0.f );
	vertex3.texture_coordinate = sf::Vector2f( 1.f, 1.f );

	// Make sure we get a degenerate triangle after every primitive.
	// Widely available alternative to primitive restart.
	primitive->vertices.push_back( vertex0 );
	primitive->vertices.push_back( vertex0 );
	primitive->vertices.push_back( vertex1 );
	primitive->vertices.push_back( vertex2 );
	primitive->vertices.push_back( vertex3 );
	primitive->vertices.push_back( vertex3 );

	m_primitives.push_back( primitive );

	return primitive;
}

ProjectO::PrimitivePtr ProjectO::CreatePane( const sf::Vector2f& position, const sf::Vector2f& size, float border_width,
                                             const sf::Color& color, const sf::Color& border_color, int border_color_shift ) {
  if( border_width <= 0.f ) {
		return CreateRect( position, position + size, color );
  }

  PrimitivePtr primitive( new Primitive );

	sf::Color dark_border( border_color );
	sf::Color light_border( border_color );

	Context::Get().GetEngine().ShiftBorderColors( light_border, dark_border, border_color_shift );

	float left = position.x;
	float top = position.y;
	float right = left + size.x;
	float bottom = top + size.y;

	PrimitivePtr rect(
		CreateQuad(
			sf::Vector2f( left + border_width, top + border_width ),
			sf::Vector2f( left + border_width, bottom - border_width ),
			sf::Vector2f( right - border_width, bottom - border_width ),
			sf::Vector2f( right - border_width, top + border_width ),
			color
		)
	);

	PrimitivePtr line_top(
		CreateLine(
			sf::Vector2f( left, top + border_width / 2.f ),
			sf::Vector2f( right - border_width, top + border_width / 2.f ),
			light_border,
			border_width
		)
	);

	PrimitivePtr line_right(
		CreateLine(
			sf::Vector2f( right - border_width / 2.f, top ),
			sf::Vector2f( right - border_width / 2.f, bottom - border_width ),
			dark_border,
			border_width
		)
	);

	PrimitivePtr line_bottom(
		CreateLine(
			sf::Vector2f( right, bottom - border_width / 2.f ),
			sf::Vector2f( left + border_width, bottom - border_width / 2.f ),
			dark_border,
			border_width
		)
	);

	PrimitivePtr line_left(
		CreateLine(
			sf::Vector2f( left + border_width / 2.f, bottom ),
			sf::Vector2f( left + border_width / 2.f, top + border_width ),
			light_border,
			border_width
		)
	);

	primitive->Add( *rect.get() );
	primitive->Add( *line_top.get() );
	primitive->Add( *line_right.get() );
	primitive->Add( *line_bottom.get() );
	primitive->Add( *line_left.get() );

	std::vector<PrimitivePtr>::iterator iter( std::find( m_primitives.begin(), m_primitives.end(), rect ) );

	assert( iter != m_primitives.end() );

	iter = m_primitives.erase( iter ); // rect
	iter = m_primitives.erase( iter ); // line_top
	iter = m_primitives.erase( iter ); // line_right
	iter = m_primitives.erase( iter ); // line_bottom
	m_primitives.erase( iter ); // line_left

	m_primitives.push_back( primitive );

	return primitive;
}

ProjectO::PrimitivePtr ProjectO::CreateRect( const sf::Vector2f& top_left, const sf::Vector2f& bottom_right, const sf::Color& color ) {
	return CreateQuad(
		sf::Vector2f( top_left.x, top_left.y ),
		sf::Vector2f( top_left.x, bottom_right.y ),
		sf::Vector2f( bottom_right.x, bottom_right.y ),
		sf::Vector2f( bottom_right.x, top_left.y ),
		color
	);
}

ProjectO::PrimitivePtr ProjectO::CreateRect( const sf::FloatRect& rect, const sf::Color& color ) {
	return CreateRect( sf::Vector2f( rect.Left, rect.Top ), sf::Vector2f( rect.Left + rect.Width, rect.Top + rect.Height ), color );
}

ProjectO::PrimitivePtr ProjectO::CreateTriangle( const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color ) {
	PrimitivePtr primitive( new Primitive );

	Primitive::Vertex vertex0;
	Primitive::Vertex vertex1;
	Primitive::Vertex vertex2;

	vertex0.position = point0;
	vertex1.position = point1;
	vertex2.position = point2;

	vertex0.color = color;
	vertex1.color = color;
	vertex2.color = color;

	vertex0.texture_coordinate = sf::Vector2f( 0.f, 0.f );
	vertex1.texture_coordinate = sf::Vector2f( 0.f, 1.f );
	vertex2.texture_coordinate = sf::Vector2f( 1.f, 0.f );

	// Make sure we get a degenerate triangle after every primitive.
	// Widely available alternative to primitive restart.
	primitive->vertices.push_back( vertex0 );
	primitive->vertices.push_back( vertex0 );
	primitive->vertices.push_back( vertex1 );
	primitive->vertices.push_back( vertex2 );
	primitive->vertices.push_back( vertex2 );

	m_primitives.push_back( primitive );

	return primitive;
}

ProjectO::PrimitivePtr ProjectO::CreateImage( const sf::FloatRect& rect, const sf::Image& image ) {
	sf::Vector2f offset( LoadImage( image ) );

	PrimitivePtr primitive( new Primitive );

	Primitive::Vertex vertex0;
	Primitive::Vertex vertex1;
	Primitive::Vertex vertex2;
	Primitive::Vertex vertex3;

	vertex0.position = sf::Vector2f( std::floor( rect.Left + .5f ), std::floor( rect.Top + .5f ) );
	vertex1.position = sf::Vector2f( std::floor( rect.Left + .5f ), std::floor( rect.Top + .5f ) + std::floor( rect.Height + .5f ) );
	vertex2.position = sf::Vector2f( std::floor( rect.Left + .5f ) + std::floor( rect.Width + .5f ), std::floor( rect.Top + .5f ) );
	vertex3.position = sf::Vector2f( std::floor( rect.Left + .5f ) + std::floor( rect.Width + .5f ), std::floor( rect.Top + .5f ) + std::floor( rect.Height + .5f ) );

	vertex0.color = sf::Color( 255, 255, 255, 255 );
	vertex1.color = sf::Color( 255, 255, 255, 255 );
	vertex2.color = sf::Color( 255, 255, 255, 255 );
	vertex3.color = sf::Color( 255, 255, 255, 255 );

	vertex0.texture_coordinate = offset + sf::Vector2f( 0.f, 0.f );
	vertex1.texture_coordinate = offset + sf::Vector2f( 0.f, static_cast<float>( image.GetHeight() ) );
	vertex2.texture_coordinate = offset + sf::Vector2f( static_cast<float>( image.GetWidth() ), 0.f );
	vertex3.texture_coordinate = offset + sf::Vector2f( static_cast<float>( image.GetWidth() ), static_cast<float>( image.GetHeight() ) );

	// Make sure we get a degenerate triangle after every primitive.
	// Widely available alternative to primitive restart.
	primitive->vertices.push_back( vertex0 );
	primitive->vertices.push_back( vertex0 );
	primitive->vertices.push_back( vertex1 );
	primitive->vertices.push_back( vertex2 );
	primitive->vertices.push_back( vertex3 );
	primitive->vertices.push_back( vertex3 );

	m_primitives.push_back( primitive );

	return primitive;
}

ProjectO::PrimitivePtr ProjectO::CreateLine( const sf::Vector2f& begin, const sf::Vector2f& end, const sf::Color& color, float thickness ) {
	// Get vector perpendicular to direction of the line vector.
	// Vector is rotated CCW 90 degrees and normalized.
	sf::Vector2f normal( end - begin );
	std::swap( normal.x, normal.y );
	float length = std::sqrt( normal.x * normal.x + normal.y * normal.y );
	normal.x /= -length;
	normal.y /= length;

	sf::Vector2f corner0( begin + normal * ( thickness * .5f ) );
	sf::Vector2f corner1( begin - normal * ( thickness * .5f ) );
	sf::Vector2f corner2( end - normal * ( thickness * .5f ) );
	sf::Vector2f corner3( end + normal * ( thickness * .5f ) );

	return CreateQuad( corner0, corner1, corner2, corner3, color );
}

void ProjectO::Display( sf::RenderWindow& window ) {
	// Remove expired Viewports
	std::size_t viewports_size = m_viewports.size();
	std::vector<ViewportWeakPtr>::iterator viewports_begin( m_viewports.begin() );

	for( std::size_t index = 0; index < viewports_size; ) {
		const ViewportPtr& viewport_ptr( m_viewports[index].lock() );

		if( !viewport_ptr ) {
			m_viewports.erase( viewports_begin + index );
			--viewports_size;

			continue;
		}

		++index;
	}

	// Remove expired Primitives
	std::size_t primitives_size = m_primitives.size();
	std::vector<PrimitivePtr>::iterator primitives_begin( m_primitives.begin() );

	for( std::size_t index = 0; index < primitives_size; ) {
		const PrimitivePtr& primitive_ptr( m_primitives[index] );

		Primitive* primitive = primitive_ptr.get();

		if( primitive_ptr.use_count() < 2 ) {
			m_primitives.erase( primitives_begin + index );
			--primitives_size;

			m_vbo_synced = false;

			continue;
		}
		else if( !primitive->synced ) {
			m_vbo_synced = false;
			primitive->synced = true;
		}

		++index;
	}

	// Refresh VBO data if out of sync
	if( !m_vbo_synced ) {
		SortPrimitives();

		m_vertex_data.clear();
		m_color_data.clear();
		m_texture_data.clear();

		m_viewport_pairs.clear();

		m_last_vertex_count = 0;

		// Rather notify and disable depth testing until requested again.
		//m_depth_test = true;

		// Check for alpha values in all primitives.
		// Disable depth test if any found.
		for( std::size_t primitive_index = 0; primitive_index < primitives_size; ++primitive_index ) {
			std::size_t vertices_size = m_primitives[primitive_index]->vertices.size();

			for( std::size_t vertex_index = 0; vertex_index < vertices_size; ++vertex_index ) {
				Primitive::Vertex& vertex( m_primitives[primitive_index]->vertices[vertex_index] );

				if( m_depth_test && ( vertex.color.a < 255 ) ) {
#ifdef SFGUI_DEBUG
					std::cerr << "Detected alpha value " << static_cast<int>( vertex.color.a ) << " disabling depth test.\n";
#endif
					m_depth_test = false;
				}
			}
		}

		// Used to normalize texture coordinates.
		sf::Vector2f normalizer( 1.f / static_cast<float>( m_texture_atlas.GetWidth() ), 1.f / static_cast<float>( m_texture_atlas.GetHeight() ) );

		// Depth test vars
		float depth = -4.f;
		float depth_delta = 4.f / static_cast<float>( primitives_size );
		int direction = m_depth_test ? -1 : 1;
		int start = m_depth_test ? primitives_size : 1;
		std::size_t end = m_depth_test ? 0 : primitives_size + 1;

		ViewportWeakPtr current_viewport;
		m_viewport_pairs.push_back( ViewportPair( current_viewport, 0 ) );

		for( std::size_t primitive_index = start; primitive_index != end; primitive_index += direction ) {
			Primitive* primitive = m_primitives[primitive_index - 1].get();

			if( !primitive->visible ) {
				continue;
			}

			sf::Vector2f position_transform( primitive->position );

			// Check if primitive needs to be rendered in a custom viewport.
			ViewportPtr viewport = primitive->viewport.lock();

			if( viewport != current_viewport.lock() ) {
				current_viewport = viewport;

				ViewportPair scissor_pair( viewport, m_last_vertex_count );

				m_viewport_pairs.push_back( scissor_pair );
			}

			if( viewport ) {
				position_transform += ( viewport->destination_origin - viewport->source_origin );
			}

			// Process primitive's vertices.
			std::size_t vertices_size = primitive->vertices.size();

			for( std::size_t vertex_index = 0; vertex_index < vertices_size; ++vertex_index ) {
				Primitive::Vertex& vertex( primitive->vertices[vertex_index] );

				sf::Vector2f position2d( vertex.position + position_transform );
				sf::Vector3f position( position2d.x, position2d.y, depth );

				m_vertex_data.push_back( position );
				m_color_data.push_back( vertex.color );

				// Normalize SFML's pixel texture coordinates.
				m_texture_data.push_back( sf::Vector2f( vertex.texture_coordinate.x * normalizer.x, vertex.texture_coordinate.y * normalizer.y ) );

				++m_last_vertex_count;
			}

			depth -= depth_delta;
		}

		// Sync vertex data
		glBindBuffer( GL_ARRAY_BUFFER, m_vertex_vbo );
		glBufferData( GL_ARRAY_BUFFER, m_vertex_data.size() * sizeof( sf::Vector3f ), &m_vertex_data[0], GL_DYNAMIC_DRAW );

		// Sync color data
		glBindBuffer( GL_ARRAY_BUFFER, m_color_vbo );
		glBufferData( GL_ARRAY_BUFFER, m_color_data.size() * sizeof( sf::Color ), &m_color_data[0], GL_STATIC_DRAW );

		// Sync texture coord data
		glBindBuffer( GL_ARRAY_BUFFER, m_texture_vbo );
		glBufferData( GL_ARRAY_BUFFER, m_texture_data.size() * sizeof( sf::Vector2f ), &m_texture_data[0], GL_STATIC_DRAW );

		m_vbo_synced = true;
	}

	// Thanks to color / texture modulation we can draw the entire
	// frame in a single pass by pseudo-disabling the texturing with
	// the help of a white texture ( 1.f * something = something ).
	// Further, we stick all referenced textures into our giant atlas
	// so we don't have to rebind during the draw.

	SetupGL( window );

	m_texture_atlas.Bind();

	glBindBuffer( GL_ARRAY_BUFFER, m_vertex_vbo );
	glVertexPointer( 3, GL_FLOAT, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_color_vbo );
	glColorPointer( 4, GL_UNSIGNED_BYTE, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_texture_vbo );
	glTexCoordPointer( 2, GL_FLOAT, 0, 0 );

	// Not needed, constantly kept enabled by SFML... -_-
	//glEnableClientState( GL_VERTEX_ARRAY );
	//glEnableClientState( GL_COLOR_ARRAY );
	//glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	std::size_t scissor_pairs_size = m_viewport_pairs.size();

	bool scissor = false;

	for( std::size_t index = 0; index < scissor_pairs_size; ++index ) {
		const ViewportPair& scissor_pair = m_viewport_pairs[index];

		ViewportPtr viewport = scissor_pair.first.lock();

		if( viewport ) {
			if( !scissor ) {
				glEnable( GL_SCISSOR_TEST );
				scissor = true;
			}

			glScissor(
				static_cast<int>( viewport->destination_origin.x ),
				window.GetHeight() - static_cast<int>( viewport->destination_origin.y + viewport->size.y ),
				static_cast<int>( viewport->size.x ),
				static_cast<int>( viewport->size.y )
			);
		}
		else {
			glDisable( GL_SCISSOR_TEST );
			scissor = false;
		}

		if( index < scissor_pairs_size - 1 ) {
			glDrawArrays( GL_TRIANGLE_STRIP, scissor_pair.second, m_viewport_pairs[index + 1].second - scissor_pair.second );
		}
		else {
			glDrawArrays( GL_TRIANGLE_STRIP, scissor_pair.second, m_last_vertex_count - scissor_pair.second );
		}
	}

	if( scissor ) {
		glDisable( GL_SCISSOR_TEST );
	}

	//glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	//glDisableClientState( GL_COLOR_ARRAY );
	//glDisableClientState( GL_VERTEX_ARRAY );

	// Needed otherwise SFML will blow up...
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	RestoreGL( window );
}

void ProjectO::SetupGL( sf::RenderWindow& window ) {
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glOrtho( 0.0f, window.GetWidth(), window.GetHeight(), 0.0f, -1.0f, 64.0f );

	glMatrixMode( GL_TEXTURE );
	glPushMatrix();
	glLoadIdentity();

	if( m_preblend ) {
		glDisable( GL_BLEND );
	}

	if( m_depth_test ) {
		glEnable( GL_DEPTH_TEST );

		glClear( GL_DEPTH_BUFFER_BIT );

		// Textures need to be sharper to be able to do this.
		// But... you get +300 FPS
		//glBlendFunc( GL_SRC_ALPHA, GL_ZERO );
	}

	if( m_alpha_threshold > 0.f ) {
		// This setting needs to be tuned per application?
		glAlphaFunc( GL_GREATER, m_alpha_threshold );
	}
}

void ProjectO::RestoreGL( sf::RenderWindow& window ) {
	if( m_alpha_threshold > 0.f ) {
		glAlphaFunc( GL_GREATER, 0.f );
	}

	if( m_depth_test ) {
		//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glDisable( GL_DEPTH_TEST );
	}

	if( m_preblend ) {
		glEnable( GL_BLEND );
	}

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );

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
		bool ViewChanged;
		sf::BlendMode LastBlendMode;
		sf::Uint64 LastTextureId;
		bool UseVertexCache;
		sf::Vertex VertexCache[4];
	};

	// All your cache are belong to us.
	memset( reinterpret_cast<char*>( &window ) + sizeof( sf::RenderTarget ) - sizeof( StatesCache ), 0, sizeof( StatesCache ) );
}

sf::Vector2f ProjectO::LoadFont( const sf::Font& font, unsigned int size ) {
	// Make sure all the glyphs we need are loaded.
	for( sf::Uint32 codepoint = 0; codepoint < 512; ++codepoint ) {
		font.GetGlyph( codepoint, size, false );
	}

	sf::Image image = font.GetTexture( size ).CopyToImage();

	return LoadImage( image );
}

sf::Vector2f ProjectO::LoadImage( const sf::Image& image ) {
	const sf::Uint8* bytes = image.GetPixelsPtr();
	std::size_t byte_count = image.GetWidth() * image.GetHeight() * 4;

	unsigned long hash = 2166136261UL;

	// Disable this check for now.
	static sf::Uint8 alpha_threshold = 255;

	for ( ; byte_count; --byte_count ) {
		hash ^= static_cast<unsigned long>( bytes[ byte_count - 1 ] );
		hash *= 16777619UL;

		// Check if the image makes intentional use
		// of the alpha channel.
		if( m_depth_test && !( byte_count % 4 ) && ( bytes[ byte_count - 1 ] > alpha_threshold ) && ( bytes[ byte_count - 1 ] < 255 ) ) {
#ifdef SFGUI_DEBUG
			std::cerr << "Detected alpha value " << static_cast<int>( bytes[ byte_count - 1 ]  ) << " in texture, disabling depth test.\n";
#endif
			m_depth_test = false;
		}
	}

	if( m_atlas_offsets.find( hash ) == m_atlas_offsets.end() ) {
		// Image needs to be loaded into atlas.
		sf::Image old_image = m_texture_atlas.CopyToImage();
		sf::Image new_image;

		// We insert padding between atlas elements to prevent
		// texture filtering from screwing up our images.
		// If 1 pixel isn't enough, increase.
		const static unsigned int padding = 1;

		new_image.Create( std::max( old_image.GetWidth(), image.GetWidth() ), old_image.GetHeight() + image.GetHeight() + padding, sf::Color::White );
		new_image.Copy( old_image, 0, 0 );

		new_image.Copy( image, 0, old_image.GetHeight() + padding );

		m_texture_atlas.LoadFromImage( new_image );

		m_atlas_offsets[hash] = sf::Vector2f( 0.f, static_cast<float>( old_image.GetHeight() + padding ) );

		m_vbo_synced = false;
	}

	return m_atlas_offsets[hash];
}

void ProjectO::SortPrimitives() {
	std::size_t current_position = 1;
	std::size_t sort_index;

	std::size_t primitives_size = m_primitives.size();

	// Classic insertion sort.
	while( current_position < primitives_size ) {
		sort_index = current_position++;

		while( ( sort_index > 0 ) && ( m_primitives[sort_index - 1]->level > m_primitives[sort_index]->level ) ) {
			m_primitives[sort_index].swap( m_primitives[sort_index - 1] );
			--sort_index;
		}
	}
}

void ProjectO::TuneDepthTest( bool enable ) {
	m_depth_test = enable;
}

void ProjectO::TuneAlphaThreshold( float alpha_threshold ) {
	m_alpha_threshold = alpha_threshold;
}

void ProjectO::TunePrecomputeBlending( bool enable ) {
	m_preblend = enable;
}

}
