#include <SFGUI/DragInfo.hpp>

namespace sfg {

DragInfo::DragInfo( const sf::Vector2f& start ) :
	m_start( start ),
	m_delta( 0, 0 ),
	m_last_position( start )
{
}

void DragInfo::Update( const sf::Vector2f& position ) {
	m_delta = m_last_position - position;
	m_last_position = position;
}

const sf::Vector2f& DragInfo::GetStartPosition() const {
	return m_start;
}

}
