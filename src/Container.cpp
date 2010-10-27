#include <SFGUI/Container.hpp>

namespace sfg {

Container::Container() :
	Widget(),
	m_border_width( 0.f )
{
	OnExpose.Connect( &Container::HandleExpose, this );
	OnSizeAllocate.Connect( &Container::HandleSizeAllocate, this );
}

void Container::Add( Widget::Ptr widget ) {
	if( IsChild( widget ) ) {
		return;
	}

	m_children.push_back( widget );
	widget->SetParent( shared_from_this() );
	RequestSize();

	OnAdd.Sig( shared_from_this(), widget );
}

void Container::Remove( Widget::Ptr widget ) {
	WidgetsList::iterator  iter( std::find( m_children.begin(), m_children.end(), widget ) );

	if( iter != m_children.end() ) {
		m_children.erase( iter );
		RequestSize();

		OnRemove.Sig( shared_from_this(), widget );
	}
}

bool Container::IsChild( Widget::Ptr widget ) const {
	WidgetsList::const_iterator  iter( std::find( m_children.begin(), m_children.end(), widget ) );

	return iter != m_children.end();
}

const Container::WidgetsList& Container::GetChildren() const {
	return m_children;
}

Container::WidgetsList& Container::GetChildren() {
	return m_children;
}

void Container::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& target ) {
	WidgetsList::iterator  iter( m_children.begin() );
	WidgetsList::iterator  iterend( m_children.end() );

	for( ; iter != iterend; ++iter ) {
		(*iter)->Expose( target );
	}
}

void Container::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& oldallocation ) {
	if( GetChildren().size() > 0 && (GetAllocation().Left != oldallocation.Left || GetAllocation().Top != oldallocation.Top) ) {
		WidgetsList::iterator  iter( m_children.begin() );
		WidgetsList::iterator  iterend( m_children.end() );
		sf::Vector2f  delta( GetAllocation().Left - oldallocation.Left, GetAllocation().Top - oldallocation.Top );

		// Move children accordingly.
		for( ; iter != iterend; ++iter ) {
			(*iter)->SetPosition(
				sf::Vector2f(
					(*iter)->GetAllocation().Left + delta.x,
					(*iter)->GetAllocation().Top + delta.x
				)
			);
		}
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

Container::HandleEventResult Container::HandleEvent( const sf::Event& event ) {
	// Handle event for hooks and own widget first before passing it to children.
	HandleEventResult  result( Widget::HandleEvent( event ) );

	if( result != PassEvent ) {
		return PassEvent;
	}

	WidgetsList::iterator  iter( m_children.begin() );
	WidgetsList::iterator  iterend( m_children.end() );

	for( ; iter != iterend; ++iter ) {
		result = (*iter)->HandleEvent( event );
		
		if( result == EatEvent ) {
			return EatEvent;
		}
	}

	return PassEvent;
}

}
