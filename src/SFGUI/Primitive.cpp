#include <SFGUI/Primitive.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>

namespace sfg {

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

	std::size_t vertice_count = vertices.size();

	for( std::size_t index = 0; index < vertice_count; ++index ) {
		m_vertices.push_back( vertices[index] );
	}

	m_synced = false;
}

void Primitive::AddVertex( const Vertex& vertex ) {
	m_vertices.push_back( vertex );

	m_synced = false;
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
