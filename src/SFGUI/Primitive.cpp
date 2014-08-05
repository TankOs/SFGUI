#include <SFGUI/Primitive.hpp>
#include <SFGUI/PrimitiveTexture.hpp>
#include <SFGUI/PrimitiveVertex.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Signal.hpp>

namespace sfg {

Primitive::Primitive( std::size_t vertex_reserve ) :
	m_layer( 0 ),
	m_level( 0 ),
	m_synced( false ),
	m_visible( true )
{
	m_viewport = Renderer::Get().GetDefaultViewport();

	if( vertex_reserve > 0 ) {
		m_vertices.reserve( vertex_reserve );
	}
}

void Primitive::Add( Primitive& primitive ) {
	auto current_index = m_vertices.size();

	for( const auto& vertex : primitive.GetVertices() ) {
		m_vertices.push_back( vertex );
	}

	for( const auto& index : primitive.GetIndices() ) {
		m_indices.push_back( static_cast<unsigned int>( current_index + index ) );
	}
}

void Primitive::AddVertex( const PrimitiveVertex& vertex ) {
	m_synced = false;

	auto vertice_count = m_vertices.size();

	// Skip the duplicate search if this vertex is part of the first triangle.
	if( vertice_count >= 3 ) {
		for( std::size_t index = 0; index < vertice_count; ++index ) {
			if( m_vertices[index] == vertex ) {
				// Vertex already part of this primitive. Index it.
				m_indices.push_back( static_cast<unsigned int>( index ) );

				return;
			}
		}
	}

	m_indices.push_back( static_cast<unsigned int>( vertice_count ) );
	m_vertices.push_back( vertex );
}

void Primitive::AddTexture( PrimitiveTexture::Ptr texture ) {
	m_textures.push_back( texture );
}

void Primitive::SetPosition( const sf::Vector2f& position ) {
	m_position = position;

	m_synced = false;
}

const sf::Vector2f& Primitive::GetPosition() const {
	return m_position;
}

void Primitive::SetViewport( RendererViewport::Ptr viewport ) {
	m_viewport = viewport;

	m_synced = false;
}

RendererViewport::Ptr Primitive::GetViewport() const {
	return m_viewport;
}

void Primitive::SetLayer( int layer ) {
	m_layer = layer;

	m_synced = false;
}

int Primitive::GetLayer() const {
	return m_layer;
}

void Primitive::SetLevel( int level ) {
	m_level = level;

	m_synced = false;
}

int Primitive::GetLevel() const {
	return m_level;
}

std::vector<PrimitiveVertex>& Primitive::GetVertices() {
	return m_vertices;
}

std::vector<PrimitiveTexture::Ptr>& Primitive::GetTextures() {
	return m_textures;
}

const std::vector<unsigned int>& Primitive::GetIndices() const {
	return m_indices;
}

void Primitive::SetSynced( bool synced ) {
	m_synced = synced;
}

bool Primitive::IsSynced() const {
	return m_synced;
}

void Primitive::SetVisible( bool visible ) {
	m_visible = visible;

	m_synced = false;
}

bool Primitive::IsVisible() const {
	return m_visible;
}

void Primitive::SetCustomDrawCallback( std::shared_ptr<Signal> callback ) {
	m_custom_draw_callback = callback;
}

std::shared_ptr<Signal> Primitive::GetCustomDrawCallback() const {
	return m_custom_draw_callback;
}

void Primitive::Clear() {
	m_vertices.clear();
	m_textures.clear();
	m_indices.clear();

	m_position = sf::Vector2f( 0.f, 0.f );
	m_layer = 0;
	m_level = 0;
	m_synced = false;
	m_visible = true;

	m_viewport = Renderer::Get().GetDefaultViewport();
	m_custom_draw_callback.reset();
}

}
