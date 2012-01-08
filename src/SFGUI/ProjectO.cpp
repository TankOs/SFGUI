#include <GL/glew.h>
#include <SFGUI/ProjectO.hpp>

#include <cmath>
#include <algorithm>

namespace sfg {

bool CompareHierarchyLevels( ProjectO::PrimitivePtr left, ProjectO::PrimitivePtr right ) {
	return ( left->level < right->level );
}

ProjectO::ProjectO() :
	m_last_vertex_count( 0 ),
	m_vbo_synced( false ) {
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

	primitive->synced = false;
	primitive->visible = true;

	primitive->position = sf::Vector2f( 0.f, 0.f );
	primitive->layer = 0.f;
	primitive->level = 0;

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

	primitive->synced = false;
	primitive->visible = true;

	primitive->position = sf::Vector2f( 0.f, 0.f );
	primitive->layer = 0.f;
	primitive->level = 0;

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

	primitive->synced = false;
	primitive->visible = true;

	primitive->position = sf::Vector2f( 0.f, 0.f );
	primitive->layer = 0.f;
	primitive->level = 0;

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

	primitive->synced = false;
	primitive->visible = true;

	primitive->position = sf::Vector2f( 0.f, 0.f );
	primitive->layer = 0.f;
	primitive->level = 0;

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
	std::size_t primitives_size = m_primitives.size();
	std::vector<PrimitivePtr>::iterator primitives_begin( m_primitives.begin() );

	for( std::size_t index = 0; index < primitives_size; ) {
		if( m_primitives[index].use_count() < 2 ) {
			m_primitives.erase( primitives_begin + index );
			--primitives_size;

			m_vbo_synced = false;

			continue;
		}
		else if( !m_primitives[index]->synced ) {
			m_vbo_synced = false;
			m_primitives[index]->synced = true;
		}

		++index;
	}

	SetupGL( window );

	if( !m_vbo_synced ) {
		std::stable_sort( primitives_begin, m_primitives.end(), CompareHierarchyLevels );

		m_vertex_data.clear();
		m_color_data.clear();
		m_texture_data.clear();

		m_last_vertex_count = 0;

		sf::Vector2f normalizer( 1.f / static_cast<float>( m_texture_atlas.GetWidth() ), 1.f / static_cast<float>( m_texture_atlas.GetHeight() ) );

		for( std::size_t primitive_index = 0; primitive_index < primitives_size; ++primitive_index ) {
			if( !m_primitives[primitive_index]->visible ) {
				continue;
			}

			std::size_t vertices_size = m_primitives[primitive_index]->vertices.size();

			for( std::size_t vertex_index = 0; vertex_index < vertices_size; ++vertex_index ) {
				Primitive::Vertex& vertex( m_primitives[primitive_index]->vertices[vertex_index] );

				m_vertex_data.push_back( vertex.position + m_primitives[primitive_index]->position );
				m_color_data.push_back( vertex.color );

				// Normalize SFML's pixel texture coordinates.
				m_texture_data.push_back( sf::Vector2f( vertex.texture_coordinate.x * normalizer.x, vertex.texture_coordinate.y * normalizer.y ) );

				++m_last_vertex_count;
			}
		}

		// Sync vertex data
		glBindBuffer( GL_ARRAY_BUFFER, m_vertex_vbo );
		glBufferData( GL_ARRAY_BUFFER, m_vertex_data.size() * sizeof( sf::Vector2f ), &m_vertex_data[0], GL_DYNAMIC_DRAW );

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

	m_texture_atlas.Bind();

	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.f );

	glBindBuffer( GL_ARRAY_BUFFER, m_vertex_vbo );
	glVertexPointer( 2, GL_FLOAT, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_color_vbo );
	glColorPointer( 4, GL_UNSIGNED_BYTE, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_texture_vbo );
	glTexCoordPointer( 2, GL_FLOAT, 0, 0 );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, m_last_vertex_count );

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	glDisable( GL_ALPHA_TEST );

	RestoreGL( window );
}

void ProjectO::SetupGL( sf::RenderWindow& window ) {
	window.PushGLStates();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glOrtho( 0.0f, window.GetWidth(), window.GetHeight(), 0.0f, -1.0f, 1.0f );

	glMatrixMode( GL_TEXTURE );
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
}

void ProjectO::RestoreGL( sf::RenderWindow& window ) {
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_TEXTURE );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );

	window.PopGLStates();

	window.ResetGLStates();
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

	for ( ; byte_count; --byte_count ) {
		hash ^= static_cast<unsigned long>( bytes[ byte_count - 1 ] );
		hash *= 16777619UL;
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

}
