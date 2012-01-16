#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Context.hpp>

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
	m_viewport = Context::Get().GetRenderer().GetDefaultViewport();
}

RenderQueue::~RenderQueue() {
	while( !m_primitives.empty() ) {
		Context::Get().GetRenderer().RemovePrimitive( m_primitives.back() );
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
	primitive->level = m_level;
	primitive->position = m_position;
	primitive->viewport = m_viewport;
	primitive->visible = m_show;
	primitive->synced = false;

	Context::Get().GetRenderer().InvalidateVBO();
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
		m_primitives[index]->position = position;
		m_primitives[index]->synced = false;
	}

	Context::Get().GetRenderer().InvalidateVBO();
}

const std::vector<Primitive::Ptr>& RenderQueue::GetPrimitives() const {
	return m_primitives;
}

int RenderQueue::GetZOrder() const {
	return m_z_order;
}

void RenderQueue::SetZOrder( int z_order ) {
	m_z_order = z_order;
}

void RenderQueue::Show( bool show ) {
	m_show = show;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->visible = show;
		m_primitives[index]->synced = false;
	}

	Context::Get().GetRenderer().InvalidateVBO();
}

void RenderQueue::SetLevel( int level ) {
	m_level = level;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->level = level;
		m_primitives[index]->synced = false;
	}

	Context::Get().GetRenderer().InvalidateVBO();
}

void RenderQueue::SetViewport( const RendererViewport::Ptr& viewport ) {
	m_viewport = viewport;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->viewport = m_viewport;
		m_primitives[index]->synced = false;
	}

	Context::Get().GetRenderer().InvalidateVBO();
}

const RendererViewport::Ptr& RenderQueue::GetViewport() const {
	return m_viewport;
}

}
