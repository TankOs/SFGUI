#include <SFGUI/Misc.hpp>

#include <algorithm>

namespace sfg {

	bool operator==( Misc::Alignment& a1, Misc::Alignment& a2 ) {
		return ( a1.position.x == a2.position.x &&
			a1.position.y == a2.position.y &&
			a1.justification == a2.justification );
	}

	bool operator!=( Misc::Alignment& a1, Misc::Alignment& a2 ) {
		return ( a1.position.x != a2.position.x &&
			a1.position.y != a2.position.y &&
			a1.justification != a2.justification );
	}
	
	void Misc::SetAlignment( const sf::Vector2f position, const Misc::Justify justification ) {
		Misc::Alignment old_alignment( m_alignment );

		m_alignment.position.x = std::max( 0.f, std::min( 1.f, position.x ) );
		m_alignment.position.y = std::max( 0.f, std::min( 1.f, position.y ) );
		m_alignment.justification = justification;

		if ( old_alignment != m_alignment ) {
			HandleAlignmentChange( old_alignment );
		}
	}

const Misc::Alignment& Misc::GetAlignment() const {
	return m_alignment;
}

void Misc::HandleAlignmentChange( const Misc::Alignment& /*old_alignment*/ ) {
}

}
