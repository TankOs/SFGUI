#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Primitive.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iterator>

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

void RenderQueue::Add( std::unique_ptr<RenderQueue> queue ) {
	auto primitives = std::move( queue->m_primitives );
	m_primitives.reserve( m_primitives.size() + primitives.size() );
	std::move( primitives.begin(), primitives.end(), std::back_inserter( m_primitives ) );
}

void RenderQueue::Add( Primitive::Ptr primitive ) {
	m_primitives.push_back( primitive );
	primitive->SetLevel( m_level );
	primitive->SetPosition( m_position );
	primitive->SetViewport( m_viewport );
	primitive->SetVisible( m_show );

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

const sf::Vector2f& RenderQueue::GetPosition() const {
	return m_position;
}

void RenderQueue::SetPosition( const sf::Vector2f& position ) {
	if( position == m_position ) {
		return;
	}

	m_position = position;

	for( const auto& primitive : m_primitives ) {
		primitive->SetPosition( position );
	}

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_VERTEX );
}

const std::vector<Primitive::Ptr>& RenderQueue::GetPrimitives() const {
	return m_primitives;
}

int RenderQueue::GetZOrder() const {
	return m_z_order;
}

void RenderQueue::SetZOrder( int z_order ) {
	m_z_order = z_order;

	for( const auto& primitive : m_primitives ) {
		primitive->SetLayer( z_order );
	}

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

void RenderQueue::Show( bool show ) {
	m_show = show;

	for( const auto& primitive : m_primitives ) {
		primitive->SetVisible( show );
	}

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

void RenderQueue::SetLevel( int level ) {
	m_level = level;

	for( const auto& primitive : m_primitives ) {
		primitive->SetLevel( level );
	}

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

void RenderQueue::SetViewport( RendererViewport::Ptr viewport ) {
	m_viewport = viewport;

	for( const auto& primitive : m_primitives ) {
		primitive->SetViewport( m_viewport );
	}

	Renderer::Get().Invalidate( sfg::Renderer::INVALIDATE_ALL );
}

RendererViewport::Ptr RenderQueue::GetViewport() const {
	return m_viewport;
}

}
