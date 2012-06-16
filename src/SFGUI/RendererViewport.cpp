#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>

namespace sfg {

std::size_t RendererViewport::last_id = 0;

RendererViewport::RendererViewport() :
	m_source_origin( sf::Vector2f( 0.f, 0.f ) ),
	m_destination_origin( sf::Vector2f( 0.f, 0.f ) ),
	m_size( sf::Vector2f( 0.f, 0.f ) )
{
	m_id = last_id++;
}

void RendererViewport::SetSourceOrigin( const sf::Vector2f& origin ) {
	m_source_origin = origin;

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RendererViewport::GetSourceOrigin() const {
	return m_source_origin;
}

void RendererViewport::SetDestinationOrigin( const sf::Vector2f& origin ) {
	m_destination_origin = origin;

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RendererViewport::GetDestinationOrigin() const {
	return m_destination_origin;
}

void RendererViewport::SetSize( const sf::Vector2f& size ) {
	m_size = size;

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RendererViewport::GetSize() const {
	return m_size;
}

}
