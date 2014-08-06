#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>

namespace {

std::size_t last_id = 0;

}

namespace sfg {

RendererViewport::RendererViewport()
{
	m_id = last_id++;
}

void RendererViewport::SetSourceOrigin( const sf::Vector2f& origin ) {
	m_source_origin = origin;

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RendererViewport::GetSourceOrigin() const {
	return m_source_origin;
}

void RendererViewport::SetDestinationOrigin( const sf::Vector2f& origin ) {
	m_destination_origin = origin;

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RendererViewport::GetDestinationOrigin() const {
	return m_destination_origin;
}

void RendererViewport::SetSize( const sf::Vector2f& size ) {
	m_size = size;

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RendererViewport::GetSize() const {
	return m_size;
}

bool RendererViewport::operator==( const RendererViewport& other ) const {
	return ( m_source_origin == other.m_source_origin ) && ( m_destination_origin == other.m_destination_origin ) && ( m_size == other.m_size );
}

bool RendererViewport::operator!=( const RendererViewport& other ) const {
	return ( m_source_origin != other.m_source_origin ) || ( m_destination_origin != other.m_destination_origin ) || ( m_size != other.m_size );
}

}
