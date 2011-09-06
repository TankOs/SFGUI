#include <SFGUI/RenderQueue.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

void RenderQueue::Add( sf::Drawable* drawable ) {
	m_children.push_back( drawable );
}

void RenderQueue::Render( sf::RenderTarget& target, sf::Renderer& /*renderer*/ ) const {
	for( DrawablesVector::size_type i = 0; i < m_children.size(); i++ ) {
		target.Draw( m_children[i] );
	}
}

}
