#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sfg {

RenderQueue::RenderQueue() :
	m_z_order( 0 ),
	m_level( 0 ),
	m_show( true )
{
	m_viewport = Renderer::Get().GetDefaultViewport();
}

RenderQueue::~RenderQueue() {
	while( !m_primitives.empty() ) {
		if( sfg::Renderer::Exists() ) {
			Renderer::Get().RemovePrimitive( m_primitives.back() );
		}

		m_primitives.pop_back();
	}
}

void RenderQueue::Add( RenderQueue* queue ) {
	std::size_t primitive_count = queue->m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives.push_back( queue->m_primitives[index] );
	}

	queue->m_primitives.clear();
	delete queue;
}

void RenderQueue::Add( Primitive::Ptr primitive ) {
	m_primitives.push_back( primitive );
	primitive->SetLevel( m_level );
	primitive->SetPosition( m_position );
	primitive->SetViewport( m_viewport );
	primitive->SetVisible( m_show );

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RenderQueue::GetPosition() const {
	return m_position;
}

void RenderQueue::SetPosition( const sf::Vector2f& position ) {
	if( position == m_position ) {
		return;
	}

	m_position = position;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->SetPosition( position );
	}

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_VERTEX );
}

const std::vector<Primitive::Ptr>& RenderQueue::GetPrimitives() const {
	return m_primitives;
}

int RenderQueue::GetZOrder() const {
	return m_z_order;
}

void RenderQueue::SetZOrder( int z_order ) {
	m_z_order = z_order;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->SetLayer( z_order );
	}

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

void RenderQueue::Show( bool show ) {
	m_show = show;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->SetVisible( show );
	}

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

void RenderQueue::SetLevel( int level ) {
	m_level = level;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->SetLevel( level );
	}

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

void RenderQueue::SetViewport( const RendererViewport::Ptr& viewport ) {
	m_viewport = viewport;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->SetViewport( m_viewport );
	}

	Renderer::Get().InvalidateVBO( sfg::Renderer::INVALIDATE_ALL );
}

const RendererViewport::Ptr& RenderQueue::GetViewport() const {
	return m_viewport;
}

}
