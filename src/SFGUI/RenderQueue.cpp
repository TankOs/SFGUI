#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sfg {

RenderQueue::RenderQueue() :
	m_z_order( 0 ),
	m_level( 0 )
{
}

RenderQueue::~RenderQueue() {
}

void RenderQueue::Add( RenderQueue* queue ) {
	std::size_t primitive_count = queue->m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives.push_back( queue->m_primitives[index] );
	}

	queue->m_primitives.clear();
	delete queue;
}

void RenderQueue::Add( ProjectO::PrimitivePtr primitive ) {
	m_primitives.push_back( primitive );
	primitive->level = m_level;
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
}

const std::vector<ProjectO::PrimitivePtr>& RenderQueue::GetPrimitives() const {
	return m_primitives;
}

int RenderQueue::GetZOrder() const {
	return m_z_order;
}

void RenderQueue::SetZOrder( int z_order ) {
	m_z_order = z_order;
}

void RenderQueue::Show( bool show ) {
	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->visible = show;
		m_primitives[index]->synced = false;
	}
}

void RenderQueue::SetLevel( int level ) {
	m_level = level;

	std::size_t primitive_count = m_primitives.size();

	for( std::size_t index = 0; index < primitive_count; ++index ) {
		m_primitives[index]->level = level;
		m_primitives[index]->synced = false;
	}
}

}
