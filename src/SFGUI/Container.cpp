#include <SFGUI/Container.hpp>

namespace sfg {

Container::Container() :
	Widget(),
	m_border_width( 0.f )
{
	OnExpose.Connect( &Container::HandleExpose, this );
	OnSizeAllocate.Connect( &Container::HandleSizeAllocate, this );
	OnPositionChange.Connect( &Container::HandlePositionChange, this );
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

	for( ; iter != iterend; ++iter ) {
		result = (*iter)->HandleEvent( event );
		
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

void Container::HandlePositionChange( Widget::Ptr /*widget*/, const sf::FloatRect& oldallocation ) {
	// Move all children by difference of new and old position.
	sf::Vector2f delta( GetAllocation().Left - oldallocation.Left, GetAllocation().Top - oldallocation.Top );
	WidgetsList::iterator iter( m_children.begin() );
	WidgetsList::iterator iter_end( m_children.end() );

	for( ; iter != iter_end; ++iter ) {
		(*iter)->SetPosition(
			sf::Vector2f(
				(*iter)->GetAllocation().Left + delta.x,
				(*iter)->GetAllocation().Top + delta.y
			)
		);
	}
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


}
