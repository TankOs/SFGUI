#include <SFGUI/Container.hpp>

namespace sfg {

Container::Container() :
	Widget(),
	m_border_width( 0.f )
{
}

Container::~Container() {
}

void Container::Add( Widget::Ptr widget ) {
	if( IsChild( widget ) ) {
		return;
	}

	m_children.push_back( widget );
	HandleAdd( widget );

	// Check if HandleAdd still wants the little boy.
	if( IsChild( widget ) ) {
		widget->SetParent( shared_from_this() );
		RequestSize();
	}
}

void Container::Remove( Widget::Ptr widget ) {
	WidgetsList::iterator  iter( std::find( m_children.begin(), m_children.end(), widget ) );

	if( iter != m_children.end() ) {
		m_children.erase( iter );
		RequestSize();

		HandleRemove( widget );
	}
}

bool Container::IsChild( Widget::Ptr widget ) const {
	WidgetsList::const_iterator  iter( std::find( m_children.begin(), m_children.end(), widget ) );

	return iter != m_children.end();
}

const Container::WidgetsList& Container::GetChildren() const {
	return m_children;
}

void Container::HandleExpose( CullingTarget& target ) const {
	WidgetsList::const_iterator iter( m_children.begin() );
	WidgetsList::const_iterator iterend( m_children.end() );

	for( ; iter != iterend; ++iter ) {
		(*iter)->Expose( target );
	}
}

void Container::SetBorderWidth( float width ) {
	m_border_width = width;
	RequestSize();
	Invalidate();
}

float Container::GetBorderWidth() const {
	return m_border_width;
}

void Container::Refresh() const {
	WidgetsList::const_iterator w_iter( m_children.begin() );
	WidgetsList::const_iterator w_iter_end( m_children.end() );

	for( ; w_iter != w_iter_end; ++w_iter ) {
		(*w_iter)->Refresh();
	}

	Widget::Refresh();
}

void Container::HandleEvent( const sf::Event& event ) {
	// Ignore event when widget is not visible.
	if( !IsVisible() ) {
		return;
	}

	// Pass event to children.
	WidgetsList::iterator  iter( m_children.begin() );
	WidgetsList::iterator  iterend( m_children.end() );

	// Create a copy of the event and transform mouse coordinates to local
	// coordinates if event is a mouse event.
	sf::Event local_event( event );

	if( local_event.Type == sf::Event::MouseMoved ) {
		local_event.MouseMove.X -= static_cast<int>( GetAllocation().Left );
		local_event.MouseMove.Y -= static_cast<int>( GetAllocation().Top );
	}

	if(
		local_event.Type == sf::Event::MouseButtonPressed ||
		local_event.Type == sf::Event::MouseButtonReleased
	) {
		local_event.MouseButton.X -= static_cast<int>( GetAllocation().Left );
		local_event.MouseButton.Y -= static_cast<int>( GetAllocation().Top );
	}

	for( ; iter != iterend; ++iter ) {
		(*iter)->HandleEvent( local_event );
	}

	// Process event for own widget.
	Widget::HandleEvent( event );
}

void Container::HandleAdd( Widget::Ptr /*child*/ ) {
}

void Container::HandleRemove( Widget::Ptr /*child*/ ) {
}

void Container::HandleChildInvalidate( Widget::PtrConst child  ) const {
	Container::PtrConst parent = GetParent();

	if( parent ) {
		parent->HandleChildInvalidate( child );
	}
}

void Container::HandleAbsolutePositionChange() const {
	WidgetsList::const_iterator iter( m_children.begin() );
	WidgetsList::const_iterator iter_end( m_children.end() );

	// Update children's drawable positions.
	for( ; iter != iter_end; ++iter ) {
		(*iter)->HandleAbsolutePositionChange();
	}

	// Update own drawable position.
	UpdateDrawablePosition();
}

}
