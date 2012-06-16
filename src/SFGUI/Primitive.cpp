#include <SFGUI/Primitive.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>

namespace sfg {

Primitive::Vertex::Vertex() :
	position( sf::Vector2f( 0.f, 0.f ) ),
	color( sf::Color::White ),
	texture_coordinate( sf::Vector2f( 0.f, 0.f ) )
{
}

Primitive::Vertex::Vertex( const Primitive::Vertex& other ) :
	position( other.position ),
	color( other.color ),
	texture_coordinate( other.texture_coordinate )
{
}

bool Primitive::Vertex::operator==( const Primitive::Vertex& other ) const {
	if( position != other.position ) {
		return false;
	}

	if( texture_coordinate != other.texture_coordinate ) {
		return false;
	}

	if( color != other.color ) {
		return false;
	}

	return true;
}

Primitive::Primitive() :
	m_position( sf::Vector2f( 0.f, 0.f ) ),
	m_layer( 0 ),
	m_level( 0 ),
	m_synced( false ),
	m_visible( true )
{
	m_viewport = Renderer::Get().GetDefaultViewport();
}

void Primitive::Add( Primitive& primitive ) {
	const std::vector<Vertex>& vertices( primitive.GetVertices() );
	const std::vector<GLuint>& indices( primitive.GetIndices() );

	std::size_t index_count = indices.size();

	for( std::size_t index_index = 0; index_index < index_count; ++index_index ) {
		AddVertex( vertices[ indices[index_index] ] );
	}
}

void Primitive::AddVertex( const Vertex& vertex ) {
	m_synced = false;

	std::size_t vertice_count = m_vertices.size();

	// Skip the duplicate search if this vertex is part of the first triangle.
	if( vertice_count >= 3 ) {
		for( std::size_t index = 0; index < vertice_count; ++index ) {
			if( m_vertices[index] == vertex ) {
				// Vertex already part of this primitive. Index it.
				m_indices.push_back( static_cast<GLuint>( index ) );

				return;
			}
		}
	}

	m_indices.push_back( vertice_count );
	m_vertices.push_back( vertex );
}

void Primitive::SetPosition( const sf::Vector2f& position ) {
	m_position = position;

	m_synced = false;
}

const sf::Vector2f& Primitive::GetPosition() const {
	return m_position;
}

void Primitive::SetViewport( const RendererViewport::Ptr& viewport ) {
	m_viewport = viewport;

	m_synced = false;
}

const RendererViewport::Ptr& Primitive::GetViewport() const {
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

std::vector<Primitive::Vertex>& Primitive::GetVertices() {
	return m_vertices;
}

const std::vector<GLuint>& Primitive::GetIndices() const {
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

}
