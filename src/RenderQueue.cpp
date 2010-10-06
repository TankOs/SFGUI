#include <SFGUI/RenderQueue.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

void RenderQueue::Add( sf::Drawable* drawable ) {
	m_children.push_back( drawable );
}

void RenderQueue::Render( sf::RenderTarget& target, sf::Renderer& /*renderer*/ ) const {
	DrawablesList::const_iterator  citer( m_children.begin() );
	DrawablesList::const_iterator  citerend( m_children.end() );

	for( ; citer != citerend; ++citer ) {
		target.Draw( *citer );
	}
}

}
