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

template< class U > U* RenderQueue::heap_clone_allocator::allocate_clone( const U& r ) {
	// The one true way :P
	// We don't need to do anything fancy here because the container
	// isn't supposed to allocate anything by itself anyway. (We give
	// it everything it should hold ourselves.) On destruction it will
	// call delete 0 which is defined as a no-op and since we don't
	// plan on dereferencing anything we didn't initially push into
	// the container we won't get nullptr exceptions either.
	return 0;
}

template< class U > void RenderQueue::heap_clone_allocator::deallocate_clone( const U* r ) {
	boost::delete_clone( r );
}

}
