#include <SFGUI/Misc.hpp>

namespace sfg {

Misc::Misc() {
}

Misc::~Misc() {
}

void Misc::SetAlignment( const sf::Vector2f& alignment ) {
	m_alignment.x = std::max( 0.f, std::min( 1.f, alignment.x ) );
	m_alignment.y = std::max( 0.f, std::min( 1.f, alignment.y ) );
}

const sf::Vector2f& Misc::GetAlignment() const {
	return m_alignment;
}

}
