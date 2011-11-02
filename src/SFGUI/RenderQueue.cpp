#include <SFGUI/RenderQueue.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

RenderQueue::~RenderQueue() {
	DrawablesVector::iterator iter( m_children.begin() );
	DrawablesVector::iterator iter_end( m_children.end() );

	for( ; iter != iter_end; ++iter ) {
		delete *iter;
	}
}

void RenderQueue::Add( sf::Drawable* drawable ) {
	m_children.push_back( drawable );
}

void RenderQueue::Render( sf::RenderTarget& target, sf::Renderer& /*renderer*/ ) const {
	for( DrawablesVector::size_type index = 0; index < m_children.size(); ++index ) {
		target.Draw( *m_children[index] );
	}
}

}
