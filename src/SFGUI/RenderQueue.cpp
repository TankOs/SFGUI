#include <SFGUI/RenderQueue.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

RenderQueue::RenderQueue() :
	m_display_list( 0 ),
	m_display_list_compiled( false )
{
	m_display_list = glGenLists( 1 );
}

RenderQueue::~RenderQueue() {
	glDeleteLists( m_display_list, 1 );

	DrawablesVector::iterator d_iter( m_children.begin() );
	DrawablesVector::iterator d_iter_end( m_children.end() );

	for( ; d_iter != d_iter_end; ++d_iter ) {
		delete d_iter->first;
	}
}

void RenderQueue::Add( sf::Shape* shape ) {
	sf::FloatRect aabb( .0f, .0f, .0f, .0f );

	unsigned int points_count = shape->GetPointsCount();

	for( unsigned int point_index = 0; point_index < points_count; ++point_index ) {
		sf::Vector2f point = shape->GetPointPosition( point_index );

		aabb.Left = std::min( point.x, aabb.Left );
		aabb.Top = std::min( point.y, aabb.Top );
		aabb.Width = std::max( point.x - aabb.Left, aabb.Width );
		aabb.Height = std::max( point.y - aabb.Top, aabb.Height );
	}

	m_children.push_back( std::pair<sf::Drawable*, sf::FloatRect>( shape, aabb ) );
}

void RenderQueue::Add( sf::Sprite* sprite ) {
	sf::FloatRect aabb( sprite->GetPosition(), sprite->GetSize() );
	m_children.push_back( std::pair<sf::Drawable*, sf::FloatRect>( sprite, aabb ) );
}

void RenderQueue::Add( sf::Text* text ) {
	sf::FloatRect aabb( text->GetRect() );
	m_children.push_back( std::pair<sf::Drawable*, sf::FloatRect>( text, aabb ) );
}

void RenderQueue::Add( RenderQueue* queue ) {
	DrawablesVector::iterator iter( queue->m_children.begin() );
	DrawablesVector::iterator iter_end( queue->m_children.end() );

	for( ; iter != iter_end; ++iter ) {
		m_children.push_back( *iter );
	}

	queue->m_children.clear();
	delete queue;
}

const RenderQueue::DrawablesVector& RenderQueue::GetDrawables() const {
	return m_children;
}

void RenderQueue::Render( sf::RenderTarget& target, sf::Renderer& /*renderer*/ ) const {
	if( !m_display_list ) {
		// Display list couldn't be created, render normally.

		std::size_t children_size = m_children.size();

		for( DrawablesVector::size_type index = 0; index < children_size; ++index ) {
			target.Draw( *( m_children[index].first ) );
		}
	}
	else if( !m_display_list_compiled ) {
		glNewList( m_display_list, GL_COMPILE_AND_EXECUTE );

		std::size_t children_size = m_children.size();

		for( DrawablesVector::size_type index = 0; index < children_size; ++index ) {
			target.Draw( *( m_children[index].first ) );
		}

		glEndList();

		m_display_list_compiled = true;
	}
	else {
		glCallList( m_display_list );
	}
}

void RenderQueue::SetPosition( const sf::Vector2f& position ) {
	m_display_list_compiled = false;

	sf::Drawable::SetPosition( position );
}

}
