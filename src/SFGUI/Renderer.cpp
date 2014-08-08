#include <SFGUI/Renderer.hpp>
#include <SFGUI/Renderers.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/RendererBatch.hpp>
#include <SFGUI/RendererTextureNode.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Primitive.hpp>
#include <SFGUI/PrimitiveTexture.hpp>
#include <SFGUI/PrimitiveVertex.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Context.hpp>
#include <cmath>
#include <cstring>
#include <cassert>

namespace {

std::shared_ptr<sfg::Renderer> instance;
int max_texture_size = 0;

}

namespace sfg {

Renderer::Renderer() :
	m_vertex_count( 0 ),
	m_index_count( 0 ),
	m_force_redraw( false ),
	m_primitives_sorted( false ) {
	static auto checked_max_texture_size = false;

	if( !checked_max_texture_size ) {
		// Needed to determine maximum texture size.
		sf::Context context;

		max_texture_size = static_cast<int>( sf::Texture::getMaximumSize() );

		checked_max_texture_size = true;
	}

	m_default_viewport = CreateViewport();

	// Load our "no texture" pseudo-texture.
	sf::Image pseudo_image;
	pseudo_image.create( 2, 2, sf::Color::White );
	m_pseudo_texture = LoadTexture( pseudo_image );
}

Renderer::~Renderer() {
}

Renderer& Renderer::Create() {
	if( !instance ) {
		if( NonLegacyRenderer::IsAvailable() ) {
			instance = NonLegacyRenderer::Create();
		}
		else if( VertexBufferRenderer::IsAvailable() ) {
			instance = VertexBufferRenderer::Create();
		}
		else {
			instance = VertexArrayRenderer::Create();
		}
	}

	return *instance;
}

Renderer& Renderer::Get() {
	if( !instance ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "Renderer not created yet. Did you create an sfg::SFGUI object?\n";
#endif
		Create();
	}

	return *instance;
}

void Renderer::Set( std::shared_ptr<Renderer> renderer ) {
	if( renderer ) {
		instance = renderer;
	}
}

void Renderer::Destroy() {
	instance.reset();
}

bool Renderer::Exists() {
	return static_cast<bool>( instance );
}

RendererViewport::Ptr Renderer::GetDefaultViewport() {
	return m_default_viewport;
}

RendererViewport::Ptr Renderer::CreateViewport() {
	return std::make_shared<RendererViewport>();
}

Primitive::Ptr Renderer::CreateText( const sf::Text& text ) {
	const auto& font = *text.getFont();
	auto character_size = text.getCharacterSize();
	auto color = text.getColor();

	auto atlas_offset = LoadFont( font, character_size );

	const auto& str = text.getString();

	auto horizontal_spacing = static_cast<float>( font.getGlyph( L' ', character_size, false ).advance );
	auto vertical_spacing = static_cast<float>( Context::Get().GetEngine().GetFontLineHeight( font, character_size ) );
	sf::Vector2f start_position( std::floor( text.getPosition().x + .5f ), std::floor( text.getPosition().y + static_cast<float>( character_size ) + .5f ) );

	sf::Vector2f position( start_position );

	const static auto tab_spaces = 2.f;

	sf::Uint32 previous_character = 0;

	auto primitive = std::make_shared<Primitive>( str.getSize() * 4 );

	Primitive character_primitive( 4 );

	for( const auto& current_character : str ) {
		position.x += static_cast<float>( font.getKerning( previous_character, current_character, character_size ) );

		switch( current_character ) {
			case L' ':
				position.x += horizontal_spacing;
				continue;
			case L'\t':
				position.x += horizontal_spacing * tab_spaces;
				continue;
			case L'\n':
				position.y += vertical_spacing;
				position.x = start_position.x;
				continue;
			case L'\v':
				position.y += vertical_spacing * tab_spaces;
				continue;
			default:
				break;
		}

		const auto& glyph = font.getGlyph( current_character, character_size, false );

		PrimitiveVertex vertex0;
		PrimitiveVertex vertex1;
		PrimitiveVertex vertex2;
		PrimitiveVertex vertex3;

		vertex0.position = position + sf::Vector2f( static_cast<float>( glyph.bounds.left ), static_cast<float>( glyph.bounds.top ) );
		vertex1.position = position + sf::Vector2f( static_cast<float>( glyph.bounds.left ), static_cast<float>( glyph.bounds.top + glyph.bounds.height ) );
		vertex2.position = position + sf::Vector2f( static_cast<float>( glyph.bounds.left + glyph.bounds.width ), static_cast<float>( glyph.bounds.top ) );
		vertex3.position = position + sf::Vector2f( static_cast<float>( glyph.bounds.left + glyph.bounds.width ), static_cast<float>( glyph.bounds.top + glyph.bounds.height ) );

		vertex0.color = color;
		vertex1.color = color;
		vertex2.color = color;
		vertex3.color = color;

		// Let SFML cast the Rect for us.
		sf::FloatRect texture_rect( glyph.textureRect );

		vertex0.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.left, texture_rect.top );
		vertex1.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.left, texture_rect.top + texture_rect.height );
		vertex2.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.left + texture_rect.width, texture_rect.top );
		vertex3.texture_coordinate = atlas_offset + sf::Vector2f( texture_rect.left + texture_rect.width, texture_rect.top + texture_rect.height );

		character_primitive.Clear();

		character_primitive.AddVertex( vertex0 );
		character_primitive.AddVertex( vertex1 );
		character_primitive.AddVertex( vertex2 );
		character_primitive.AddVertex( vertex2 );
		character_primitive.AddVertex( vertex1 );
		character_primitive.AddVertex( vertex3 );

		primitive->Add( character_primitive );

		position.x += static_cast<float>( glyph.advance );

		previous_character = current_character;
	}

	AddPrimitive( primitive );

	return primitive;
}

Primitive::Ptr Renderer::CreateQuad( const sf::Vector2f& top_left, const sf::Vector2f& bottom_left,
                                     const sf::Vector2f& bottom_right, const sf::Vector2f& top_right,
                                     const sf::Color& color ) {
	auto primitive = std::make_shared<Primitive>( 4 );

	PrimitiveVertex vertex0;
	PrimitiveVertex vertex1;
	PrimitiveVertex vertex2;
	PrimitiveVertex vertex3;

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

	primitive->AddVertex( vertex0 );
	primitive->AddVertex( vertex1 );
	primitive->AddVertex( vertex2 );
	primitive->AddVertex( vertex2 );
	primitive->AddVertex( vertex1 );
	primitive->AddVertex( vertex3 );

	AddPrimitive( primitive );

	return primitive;
}

Primitive::Ptr Renderer::CreatePane( const sf::Vector2f& position, const sf::Vector2f& size, float border_width,
                                     const sf::Color& color, const sf::Color& border_color, int border_color_shift ) {
	if( border_width <= 0.f ) {
		return CreateRect( position, position + size, color );
	}

	auto primitive = std::make_shared<Primitive>( 20 );

	sf::Color dark_border( border_color );
	sf::Color light_border( border_color );

	Context::Get().GetEngine().ShiftBorderColors( light_border, dark_border, border_color_shift );

	auto left = position.x;
	auto top = position.y;
	auto right = left + size.x;
	auto bottom = top + size.y;

	Primitive::Ptr rect(
		CreateQuad(
			sf::Vector2f( left + border_width, top + border_width ),
			sf::Vector2f( left + border_width, bottom - border_width ),
			sf::Vector2f( right - border_width, bottom - border_width ),
			sf::Vector2f( right - border_width, top + border_width ),
			color
		)
	);

	Primitive::Ptr line_top(
		CreateLine(
			sf::Vector2f( left + border_width / 2.f, top + border_width / 2.f ),
			sf::Vector2f( right - border_width / 2.f, top + border_width / 2.f ),
			light_border,
			border_width
		)
	);

	Primitive::Ptr line_right(
		CreateLine(
			sf::Vector2f( right - border_width / 2.f, top + border_width / 2.f ),
			sf::Vector2f( right - border_width / 2.f, bottom - border_width / 2.f ),
			dark_border,
			border_width
		)
	);

	Primitive::Ptr line_bottom(
		CreateLine(
			sf::Vector2f( right - border_width / 2.f, bottom - border_width / 2.f ),
			sf::Vector2f( left + border_width / 2.f, bottom - border_width / 2.f ),
			dark_border,
			border_width
		)
	);

	Primitive::Ptr line_left(
		CreateLine(
			sf::Vector2f( left + border_width / 2.f, bottom - border_width / 2.f ),
			sf::Vector2f( left + border_width / 2.f, top + border_width / 2.f ),
			light_border,
			border_width
		)
	);

	primitive->Add( *rect.get() );
	primitive->Add( *line_top.get() );
	primitive->Add( *line_right.get() );
	primitive->Add( *line_bottom.get() );
	primitive->Add( *line_left.get() );

	std::vector<Primitive::Ptr>::iterator iter( std::find( m_primitives.begin(), m_primitives.end(), rect ) );

	assert( iter != m_primitives.end() );

	iter = m_primitives.erase( iter ); // rect
	iter = m_primitives.erase( iter ); // line_top
	iter = m_primitives.erase( iter ); // line_right
	iter = m_primitives.erase( iter ); // line_bottom
	m_primitives.erase( iter ); // line_left

	AddPrimitive( primitive );

	return primitive;
}

Primitive::Ptr Renderer::CreateRect( const sf::Vector2f& top_left, const sf::Vector2f& bottom_right, const sf::Color& color ) {
	return CreateQuad(
		sf::Vector2f( top_left.x, top_left.y ),
		sf::Vector2f( top_left.x, bottom_right.y ),
		sf::Vector2f( bottom_right.x, bottom_right.y ),
		sf::Vector2f( bottom_right.x, top_left.y ),
		color
	);
}

Primitive::Ptr Renderer::CreateRect( const sf::FloatRect& rect, const sf::Color& color ) {
	return CreateRect( sf::Vector2f( rect.left, rect.top ), sf::Vector2f( rect.left + rect.width, rect.top + rect.height ), color );
}

Primitive::Ptr Renderer::CreateTriangle( const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color ) {
	auto primitive = std::make_shared<Primitive>( 3 );

	PrimitiveVertex vertex0;
	PrimitiveVertex vertex1;
	PrimitiveVertex vertex2;

	vertex0.position = point0;
	vertex1.position = point1;
	vertex2.position = point2;

	vertex0.color = color;
	vertex1.color = color;
	vertex2.color = color;

	vertex0.texture_coordinate = sf::Vector2f( 0.f, 0.f );
	vertex1.texture_coordinate = sf::Vector2f( 0.f, 1.f );
	vertex2.texture_coordinate = sf::Vector2f( 1.f, 0.f );

	primitive->AddVertex( vertex0 );
	primitive->AddVertex( vertex1 );
	primitive->AddVertex( vertex2 );

	AddPrimitive( primitive );

	return primitive;
}

Primitive::Ptr Renderer::CreateSprite( const sf::FloatRect& rect, PrimitiveTexture::Ptr texture, const sf::FloatRect& subrect, int rotation_turns ) {
	auto offset = texture->offset;

	auto primitive = std::make_shared<Primitive>( 4 );

	PrimitiveVertex vertex0;
	PrimitiveVertex vertex1;
	PrimitiveVertex vertex2;
	PrimitiveVertex vertex3;

	vertex0.position = sf::Vector2f( std::floor( rect.left + .5f ), std::floor( rect.top + .5f ) );
	vertex1.position = sf::Vector2f( std::floor( rect.left + .5f ), std::floor( rect.top + .5f ) + std::floor( rect.height + .5f ) );
	vertex2.position = sf::Vector2f( std::floor( rect.left + .5f ) + std::floor( rect.width + .5f ), std::floor( rect.top + .5f ) );
	vertex3.position = sf::Vector2f( std::floor( rect.left + .5f ) + std::floor( rect.width + .5f ), std::floor( rect.top + .5f ) + std::floor( rect.height + .5f ) );

	vertex0.color = sf::Color( 255, 255, 255, 255 );
	vertex1.color = sf::Color( 255, 255, 255, 255 );
	vertex2.color = sf::Color( 255, 255, 255, 255 );
	vertex3.color = sf::Color( 255, 255, 255, 255 );

	sf::Vector2f coords[4];

	if( ( subrect.left != 0.f ) || ( subrect.top != 0.f ) || ( subrect.width != 0.f ) || ( subrect.height != 0.f ) ) {
		coords[0] = offset + sf::Vector2f( std::floor( subrect.left + .5f ), std::floor( subrect.top + .5f ) );
		coords[3] = offset + sf::Vector2f( std::floor( subrect.left + .5f ), std::floor( subrect.top + .5f ) ) + sf::Vector2f( 0.f, std::floor( subrect.height + .5f ) );
		coords[1] = offset + sf::Vector2f( std::floor( subrect.left + .5f ), std::floor( subrect.top + .5f ) ) + sf::Vector2f( std::floor( subrect.width + .5f ), 0.f );
		coords[2] = offset + sf::Vector2f( std::floor( subrect.left + .5f ), std::floor( subrect.top + .5f ) ) + sf::Vector2f( std::floor( subrect.width + .5f ), std::floor( subrect.height + .5f ) );
	}
	else {
		coords[0] = offset + sf::Vector2f( 0.f, 0.f );
		coords[3] = offset + sf::Vector2f( 0.f, static_cast<float>( texture->size.y ) );
		coords[1] = offset + sf::Vector2f( static_cast<float>( texture->size.x ), 0.f );
		coords[2] = offset + sf::Vector2f( static_cast<float>( texture->size.x ), static_cast<float>( texture->size.y ) );
	}

	// Get rotation_turns into the range [0;3].
	rotation_turns %= 4;

	if( rotation_turns < 0 ) {
		rotation_turns += 4;
	}

	// Perform the circular shift.
	if( rotation_turns != 0 ) {
		std::rotate( coords, coords + rotation_turns, coords + 4 );
	}

	vertex0.texture_coordinate = coords[0];
	vertex1.texture_coordinate = coords[3];
	vertex2.texture_coordinate = coords[1];
	vertex3.texture_coordinate = coords[2];

	primitive->AddVertex( vertex0 );
	primitive->AddVertex( vertex1 );
	primitive->AddVertex( vertex2 );
	primitive->AddVertex( vertex2 );
	primitive->AddVertex( vertex1 );
	primitive->AddVertex( vertex3 );

	primitive->AddTexture( texture );

	AddPrimitive( primitive );

	return primitive;
}

Primitive::Ptr Renderer::CreateLine( const sf::Vector2f& begin, const sf::Vector2f& end, const sf::Color& color, float thickness ) {
	// Get vector perpendicular to direction of the line vector.
	// Vector is rotated CCW 90 degrees and normalized.
	sf::Vector2f normal( end - begin );
	sf::Vector2f unrotated_normal( normal );
	std::swap( normal.x, normal.y );
	auto length = std::sqrt( normal.x * normal.x + normal.y * normal.y );

	if( !( length > 0.f ) ) {
		return std::make_shared<Primitive>();
	}

	normal.x /= -length;
	normal.y /= length;
	unrotated_normal.x /= length;
	unrotated_normal.y /= length;

	sf::Vector2f corner0( begin + normal * ( thickness * .5f ) - unrotated_normal * ( thickness * .5f ) );
	sf::Vector2f corner1( begin - normal * ( thickness * .5f ) - unrotated_normal * ( thickness * .5f ) );
	sf::Vector2f corner2( end - normal * ( thickness * .5f ) + unrotated_normal * ( thickness * .5f ) );
	sf::Vector2f corner3( end + normal * ( thickness * .5f ) + unrotated_normal * ( thickness * .5f ) );

	return CreateQuad( corner3, corner2, corner1, corner0, color );
}

Primitive::Ptr Renderer::CreateGLCanvas( std::shared_ptr<Signal> callback ) {
	auto primitive = std::make_shared<Primitive>();
	primitive->SetCustomDrawCallback( callback );
	AddPrimitive( primitive );
	return primitive;
}

void Renderer::WipeStateCache( sf::RenderTarget& target ) const {
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

/// @cond

sf::Vector2f Renderer::LoadFont( const sf::Font& font, unsigned int size ) {
	// Get the font face that Laurent tries to hide from us.
	struct FontStruct {
		void* library;
		void* font_face; // Authentic SFML comment: implementation details
		void* unused1;
		int* unused2;
		std::string family;

		// Since maps allocate everything non-contiguously on the heap we can use void* instead of Page here.
		mutable std::map<unsigned int, void*> unused3;
		mutable std::vector<sf::Uint8> unused4;
	};

	// All your font face are belong to us too.
	void* face = reinterpret_cast<const FontStruct&>( font ).font_face;

	FontID id( face, size );

	std::map<FontID, PrimitiveTexture::Ptr >::iterator iter( m_fonts.find( id ) );

	if( iter != m_fonts.end() ) {
		return iter->second->offset;
	}

	// Make sure all the glyphs we need are loaded.
	for( sf::Uint32 codepoint = 0; codepoint < 0x0370; ++codepoint ) {
		font.getGlyph( codepoint, size, false );
	}

	// Make a local copy to avoid unnecessary dereferencing.
	for( const auto character_set : m_character_sets ) {
		for( auto codepoint = character_set.first; codepoint < character_set.second; ++codepoint ) {
			font.getGlyph( codepoint, size, false );
		}
	}

	const auto image = font.getTexture( size ).copyToImage();

	const auto handle = LoadTexture( image );

	m_fonts[id] = handle;

	return handle->offset;
}

PrimitiveTexture::Ptr Renderer::LoadTexture( const sf::Texture& texture ) {
	return LoadTexture( texture.copyToImage() );
}

PrimitiveTexture::Ptr Renderer::LoadTexture( const sf::Image& image ) {
	// We insert padding between atlas elements to prevent
	// texture filtering from screwing up our images.
	// If 1 pixel isn't enough, increase.
	const static auto padding = 1;

	auto required_vertical_size = static_cast<int>( image.getSize().y ) + padding;
	auto required_horizontal_size = static_cast<int>( image.getSize().x );

	if( ( required_horizontal_size > max_texture_size ) || ( required_vertical_size > static_cast<int>( max_texture_size ) ) ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: The image you are using is larger than the maximum size supported by your GPU (" << max_texture_size << "x" << max_texture_size << ").\n";
#endif
		return std::make_shared<PrimitiveTexture>();
	}

	// Look for a nice insertion point for our new texture.
	// We use first fit and according to theory it is never
	// worse than double the optimum size.
	auto texture_iter = m_textures.begin();

	auto atlas_last_occupied_location = 0;

	for( ; texture_iter != m_textures.end(); ++texture_iter ) {
		auto space_available = texture_iter->offset.y - atlas_last_occupied_location;

		if( space_available >= required_vertical_size ) {
			// We found a nice spot.
			break;
		}

		atlas_last_occupied_location = texture_iter->offset.y + texture_iter->size.y;
	}

	auto current_page_index = atlas_last_occupied_location / max_texture_size;
	auto current_page_last_occupied_location = atlas_last_occupied_location % max_texture_size;

	// Cache the "temporary" sf::Image so its internal std::vector
	// does not have to constantly be allocated anew.
	static sf::Image new_image;

	if( m_texture_atlas.empty() || ( current_page_last_occupied_location + required_vertical_size > max_texture_size ) ) {
		// We need a new atlas page.

		auto new_texture = std::unique_ptr<sf::Texture>( new sf::Texture );

		// Protection against possible broken cards that don't like huge textures.
		// Disable if having issues.
		static auto create_maximal = true;

		if( create_maximal ) {
			if( !new_texture->create( 1u, static_cast<unsigned int>( max_texture_size ) ) ) {
				create_maximal = false;
			}
		}

		if( !create_maximal && !m_texture_atlas.empty() ) {
			// Make sure the current page vertical size is maximal
			// so we can compute the right page from y texture coordinate.
			auto current_page = m_texture_atlas[static_cast<std::size_t>( current_page_index )].get();
			auto old_image = current_page->copyToImage();

			new_image.create( old_image.getSize().x, static_cast<unsigned int>( max_texture_size ), sf::Color::White );
			new_image.copy( old_image, 0u, 0u );
			new_image.copy( image, 0u, static_cast<unsigned int>( current_page_last_occupied_location ) );

			current_page->loadFromImage( new_image );
		}

		// Insert the new page.
		m_texture_atlas.push_back( std::move( new_texture ) );

		current_page_index = static_cast<int>( m_texture_atlas.size() ) - 1;
		current_page_last_occupied_location = 0;
	}

	auto current_page = m_texture_atlas[static_cast<std::size_t>( current_page_index )].get();
	auto current_page_size_x = static_cast<int>( current_page->getSize().x );
	auto current_page_size_y = static_cast<int>( current_page->getSize().y );

	if( ( required_horizontal_size > current_page_size_x ) || ( current_page_last_occupied_location + required_vertical_size > current_page_size_y ) ) {
		// Image is loaded into atlas after expanding texture atlas.
		auto old_image = current_page->copyToImage();

		new_image.create( static_cast<unsigned int>( std::max( current_page_size_x, required_horizontal_size ) ), static_cast<unsigned int>( std::max( current_page_size_y, current_page_last_occupied_location + required_vertical_size ) ), sf::Color::White );
		new_image.copy( old_image, 0u, 0u );
		new_image.copy( image, 0u, static_cast<unsigned int>( current_page_last_occupied_location ) );

		current_page->loadFromImage( new_image );
	}
	else {
		// Image is loaded into atlas.
		current_page->update( image, 0u, static_cast<unsigned int>( current_page_last_occupied_location ) );
	}

	auto offset = sf::Vector2i( 0, current_page_index * max_texture_size + current_page_last_occupied_location );

	Invalidate( INVALIDATE_TEXTURE );

	auto handle = std::make_shared<PrimitiveTexture>();

	handle->offset = static_cast<sf::Vector2f>( offset );
	handle->size = image.getSize();

	priv::RendererTextureNode texture_node;
	texture_node.offset = offset;
	texture_node.size = static_cast<sf::Vector2i>( image.getSize() ) + sf::Vector2i( 0, padding );

	m_textures.insert( texture_iter, texture_node );

	return handle;
}

void Renderer::UnloadImage( const sf::Vector2f& offset ) {
	sf::Vector2i int_offset( static_cast<int>( std::floor( offset.x + .5f ) ), static_cast<int>( std::floor( offset.y + .5f ) ) );
	for( auto iter = m_textures.begin(); iter != m_textures.end(); ++iter ) {
		if( iter->offset == int_offset ) {
			m_textures.erase( iter );
			return;
		}
	}

// Only enable during development.
//#if defined( SFGUI_DEBUG )
//	std::cerr << "Tried to unload non-existant image at (" << offset.x << "," << offset.y << ").\n";
//#endif
}

void Renderer::UpdateImage( const sf::Vector2f& offset, const sf::Image& data ) {
	// We insert padding between atlas elements to prevent
	// texture filtering from screwing up our images.
	// If 1 pixel isn't enough, increase.
	const static auto padding = 1;

	sf::Vector2i int_offset( static_cast<int>( std::floor( offset.x + .5f ) ), static_cast<int>( std::floor( offset.y + .5f ) ) );
	sf::Vector2i int_size( static_cast<sf::Vector2i>( data.getSize() ) + sf::Vector2i( 0, padding ) );

	for( const auto& texture : m_textures ) {
		if( texture.offset == int_offset ) {
			if( texture.size != int_size ) {
#if defined( SFGUI_DEBUG )
				std::cerr << "Tried to update texture with mismatching image size.\n";
#endif
				return;
			}

			auto page = static_cast<std::size_t>( int_offset.y / max_texture_size );

			m_texture_atlas[page]->update( data, 0u, static_cast<unsigned int>( int_offset.y % max_texture_size ) );

			return;
		}
	}

// Only enable during development.
//#if defined( SFGUI_DEBUG )
//	std::cerr << "Tried to update non-existant image at (" << offset.x << "," << offset.y << ").\n";
//#endif
}

/// @endcond

void Renderer::SortPrimitives() {
	if( m_primitives_sorted ) {
		return;
	}

	std::size_t current_position = 1;
	std::size_t sort_index;

	auto primitives_size = m_primitives.size();

	// Classic insertion sort.
	while( current_position < primitives_size ) {
		sort_index = current_position++;

		while( ( sort_index > 0 ) && ( m_primitives[sort_index - 1]->GetLayer() * 1048576 + m_primitives[sort_index - 1]->GetLevel() > m_primitives[sort_index]->GetLayer() * 1048576 + m_primitives[sort_index]->GetLevel() ) ) {
			m_primitives[sort_index].swap( m_primitives[sort_index - 1] );
			--sort_index;
		}
	}

	m_primitives_sorted = true;
}

void Renderer::AddPrimitive( Primitive::Ptr primitive ) {
	m_primitives.push_back( primitive );

	// Check for alpha values in primitive.
	// Disable depth test if any found.
	const std::vector<PrimitiveVertex>& vertices( primitive->GetVertices() );
	const std::vector<unsigned int>& indices( primitive->GetIndices() );

	m_vertex_count += static_cast<int>( vertices.size() );
	m_index_count += static_cast<int>( indices.size() );

	m_primitives_sorted = false;

	Invalidate( INVALIDATE_ALL );
}

void Renderer::RemovePrimitive( Primitive::Ptr primitive ) {
	std::vector<Primitive::Ptr>::iterator iter( std::find( m_primitives.begin(), m_primitives.end(), primitive ) );

	if( iter != m_primitives.end() ) {
		const std::vector<PrimitiveVertex>& vertices( (*iter)->GetVertices() );
		const std::vector<unsigned int>& indices( (*iter)->GetIndices() );

		assert( m_vertex_count >= static_cast<int>( vertices.size() ) );
		assert( m_index_count >= static_cast<int>( indices.size() ) );

		m_vertex_count -= static_cast<int>( vertices.size() );
		m_index_count -= static_cast<int>( indices.size() );

		m_primitives.erase( iter );
	}

	Invalidate( INVALIDATE_ALL );
}

void Renderer::Invalidate( unsigned char datasets ) {
	InvalidateImpl( datasets );
}

void Renderer::Redraw() {
	m_force_redraw = true;
}

const sf::Vector2i& Renderer::GetWindowSize() const {
	return m_last_window_size;
}

void Renderer::AddCharacterSet( sf::Uint32 low_bound, sf::Uint32 high_bound ) {
	if( high_bound <= low_bound ) {
		return;
	}

	m_character_sets.emplace_back( std::make_pair( low_bound, high_bound ) );
}

void Renderer::InvalidateImpl( unsigned char /*datasets*/ ) {
}

int Renderer::GetMaxTextureSize() const {
	return max_texture_size;
}

}
