#include <SFGUI/Container.hpp>

#include <boost/foreach.hpp>

namespace sfg {

Container::Container() :
	Widget(),
	m_border_width( 0.f )
{
}

void Container::Add( Widget::Ptr widget ) {
	if( IsChild( widget ) ) {
		return;
	}

	m_children.push_back( widget );
	widget->SetParent( shared_from_this() );
	RequestSize();

	OnAdd();
}

void Container::Remove( Widget::Ptr widget ) {
	WidgetsList::iterator  iter( std::find( m_children.begin(), m_children.end(), widget ) );

	if( iter != m_children.end() ) {
		m_children.erase( iter );
		RequestSize();

		OnRemove();
	}
}

bool Container::IsChild( Widget::Ptr widget ) const {
	WidgetsList::const_iterator  iter( std::find( m_children.begin(), m_children.end(), widget ) );

	return iter != m_children.end();
}

const Container::WidgetsList& Container::GetChildren() const {
	return m_children;
}

void Container::HandleExpose( sf::RenderTarget& target ) {
	WidgetsList::iterator iter( m_children.begin() );
	WidgetsList::iterator iterend( m_children.end() );

	for( ; iter != iterend; ++iter ) {
		(*iter)->Expose( target );
	}
}

bool Container::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) {
	UpdateDrawablePosition();

	return true;
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
	// Ignore event when widget is not visible.
	if( !IsVisible() ) {
		return IgnoreEvent;
	}

	// Process hooks, first.
	if( ProcessHooks( event ) == EatEvent ) {
		return EatEvent;
	}

	// Pass event to children.
	WidgetsList::iterator  iter( m_children.begin() );
	WidgetsList::iterator  iterend( m_children.end() );
	HandleEventResult  result;

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
		result = (*iter)->HandleEvent( local_event );
		
		if( result == EatEvent ) {
			return EatEvent;
		}
	}

	// Process event for own widget.
	result = Widget::HandleEvent( event );
	if( result != PassEvent ) {
		return result;
	}

	return PassEvent;
}

Container::HandleEventResult Container::ProcessHooks( const sf::Event& event ) {
	HooksMap::iterator  iter( m_hooks.find( event.Type ) );

	if( iter != m_hooks.end() ) {
		HookedWidgetsList::iterator  w_iter( iter->second.begin() );

		while( w_iter != iter->second.end() ) {
			// Remove hook if asked for it.
			if( w_iter->remove == true ) {
				w_iter = iter->second.erase( w_iter );
				continue;
			}

			HandleEventResult  result( (*w_iter).widget->HandleEvent( event ) );

			// Stop processing when event has been eaten.
			if( result == EatEvent ) {
				return EatEvent;
			}

			++w_iter;
		}
	}

	return PassEvent;
}

void Container::RegisterEventHook( sf::Event::EventType event_type, Widget::Ptr widget ) {
	if( GetParent() ) {
		GetParent()->RegisterEventHook( event_type, widget );
		return;
	}

	// No need to register hook for self.
	if( widget == shared_from_this() ) {
		return;
	}

	// Check if hook for widget already registered. Clear delete flag if needed.
	HookedWidgetsList&  list( m_hooks[event_type] );
	HookedWidgetsList::iterator  iter( std::find( list.begin(), list.end(), widget ) );

	if( iter != list.end() ) {
		iter->remove = false;
	}
	else {
		m_hooks[event_type].push_back( WidgetBoolPair( widget, false ) );
	}
}

void Container::UnregisterEventHook( sf::Event::EventType event_type, Widget::Ptr widget ) {
	if( GetParent() ) {
		GetParent()->UnregisterEventHook( event_type, widget );
		return;
	}

	HookedWidgetsList&  list( m_hooks[event_type] );
	HookedWidgetsList::iterator  iter( std::find( list.begin(), list.end(), widget ) );

	if( iter != list.end() ) {
		iter->remove = true;
	}
}

Container::WidgetBoolPair::WidgetBoolPair( Widget::Ptr widget_, bool remove_ ) :
	widget( widget_ ),
	remove( remove_ )
{
}

bool Container::WidgetBoolPair::operator==( const WidgetBoolPair& rhs ) {
	return widget == rhs.widget;
}

bool Container::WidgetBoolPair::operator==( const Widget::Ptr& rhs ) {
	return widget == rhs;
}

void Container::UpdateDrawablePosition() const {
	// Update children's drawable positions.
	BOOST_FOREACH( Widget::PtrConst child, m_children ) {
		child->UpdateDrawablePosition();
	}

	// Update own drawable position.
	Widget::UpdateDrawablePosition();
}

}
