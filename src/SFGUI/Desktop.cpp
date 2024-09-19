#include <SFGUI/Desktop.hpp>
#include <SFGUI/Widget.hpp>

#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <iterator>
#include <limits>

namespace sfg {

void Desktop::Update( float seconds ) {
	Context::Activate( m_context );

	std::reverse_iterator<WidgetsList::iterator> iter( std::end( m_children ) );
	std::reverse_iterator<WidgetsList::iterator> finish( std::begin( m_children ) );

	for( ; iter != finish; ++iter ) {
		(*iter)->Update( seconds );
	}

	Context::Deactivate();
}

void Desktop::HandleEvent( const sf::Event& event ) {
	// Activate context.
	Context::Activate( m_context );

	sf::Vector2f position;
	bool check_inside( false );
	Widget::Ptr last_receiver( m_last_receiver.lock() );

	// If we've got a mouse event, get local mouse position and mark event for being checked against widget's allocation.
	const auto onMouseButtonPressedOrRelease = [&]( auto& mouseButtonEvent ) {
		m_last_mouse_pos = mouseButtonEvent.position;
		position = sf::Vector2f( mouseButtonEvent.position );
		check_inside = true;
	};
	if( const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>() ) {
		m_last_mouse_pos = mouseMoved->position;
		position = sf::Vector2f( mouseMoved->position );
		check_inside = true;
	}
	else if( auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>() ) {
		onMouseButtonPressedOrRelease( *mouseButtonPressed );
	}
	else if( auto* mouseButtonRelease = event.getIf<sf::Event::MouseButtonReleased>() ) {
		onMouseButtonPressedOrRelease( *mouseButtonRelease );
	}

	for( int index = 0; index < static_cast<int>( m_children.size() ); ++index ) {
		Widget::Ptr widget( m_children[static_cast<std::size_t>( index )] );

		// Skip widget if not visible or is insensitive.
		if( !widget->IsLocallyVisible() || widget->GetState() == Widget::State::INSENSITIVE ) {
			continue;
		}

		bool is_inside( widget->GetAllocation().contains( position ) );

		// If the event is a mouse button press, check if we need to focus another widget.
		// If there is a modal widget, skip reordering.
		if(
			index > 0 &&
			event.is<sf::Event::MouseButtonPressed>() &&
			is_inside &&
			!Widget::HasModal()
		) {
			m_children.erase( m_children.begin() + index );
			m_children.push_front( widget );

			RecalculateWidgetLevels();
		}

		// If inside check is needed, do so for all widgets except the top window.
		// Else it would run into trouble when moving the window, for example,
		// where the mouse may be outside the widget's allocation.
		if( check_inside && !is_inside && index > 0 ) {
			continue;
		}

		// If last receiver is different from current, fake a mouse move event so
		// that states are reset correctly. Warning, this is a hack, but it works™.
		// The fake event is also sent when the last mouse move event receiver
		// isn't the current and top one.
		if( event.is<sf::Event::MouseMoved>() && last_receiver && last_receiver != widget && last_receiver != m_children.front() ) {
			SendFakeMouseMoveEvent( last_receiver );
			m_last_receiver = widget;
			last_receiver = widget;
		}

		widget->HandleEvent( event );

		if( check_inside && is_inside ) {
			if( index < static_cast<int>( m_children.size() ) && widget == m_children[static_cast<std::size_t>( index )] ) {
				m_last_receiver = widget;
			}
			break;
		}
	}

	// Restore previous context.
	Context::Deactivate();
}

void Desktop::Add( std::shared_ptr<Widget> widget ) {
	if( std::find( m_children.begin(), m_children.end(), widget ) != m_children.end() ) {
		return;
	}

	// Get old focused widget out of State::PRELIGHT state if mouse is inside the new
	// widget.
	if( m_children.size() ) {
		if( widget->GetAllocation().contains( sf::Vector2f( m_last_mouse_pos ) ) ) {
			SendFakeMouseMoveEvent( m_children.front() );
		}
	}

	m_children.push_front( widget );

	RecalculateWidgetLevels();

	if( widget->GetAllocation().contains( sf::Vector2f (m_last_mouse_pos ) ) ) {
		SendFakeMouseMoveEvent( widget, m_last_mouse_pos.x, m_last_mouse_pos.y );
	}

	// Activate context.
	Context::Activate( m_context );

	widget->Refresh();

	// Restore previous context.
	Context::Deactivate();
}

void Desktop::Remove( std::shared_ptr<Widget> widget ) {
	WidgetsList::iterator iter( std::find( m_children.begin(), m_children.end(), widget ));

	if( iter != m_children.end() ) {
		m_children.erase( iter );
	}

	if( m_last_receiver.lock() == widget ) {
		m_last_receiver.reset();
	}

	RecalculateWidgetLevels();

	if( !m_children.empty() &&  m_children.front()->GetAllocation().contains( sf::Vector2f( m_last_mouse_pos ) ) ) {
		SendFakeMouseMoveEvent( m_children.front(), m_last_mouse_pos.x, m_last_mouse_pos.y );
	}
}

void Desktop::RemoveAll() {
	m_children.clear();
	m_last_receiver.reset();
}

void Desktop::Refresh() {
	// Activate context.
	Context::Activate( m_context );

	RecalculateWidgetLevels();

	std::reverse_iterator<WidgetsList::iterator> iter( std::end( m_children ) );
	std::reverse_iterator<WidgetsList::iterator> finish( std::begin( m_children ) );

	for( ; iter != finish; ++iter ) {
		(*iter)->Refresh();
	}

	// Restore previous context.
	Context::Deactivate();
}

bool Desktop::LoadThemeFromFile( const std::string& filename ) {
	bool result( m_context.GetEngine().LoadThemeFromFile( filename ) );
	if( !result ) {
		return false;
	}

	Refresh();
	return true;
}

Engine& Desktop::GetEngine() {
	return m_context.GetEngine();
}

void Desktop::BringToFront( std::shared_ptr<const Widget> child ) {
	WidgetsList::iterator iter( std::find( m_children.begin(), m_children.end(), child ) );

	if( iter == m_children.end() || iter == m_children.begin() ) {
		return;
	}

	if( child->GetAllocation().contains( sf::Vector2f( m_last_mouse_pos ) ) ) {
		SendFakeMouseMoveEvent( m_children.front() );
	}

	Widget::Ptr ptr( *iter );
	m_children.erase( iter );
	m_children.push_front( ptr );

	RecalculateWidgetLevels();

	if( child->GetAllocation().contains( sf::Vector2f( m_last_mouse_pos ) ) ) {
		SendFakeMouseMoveEvent( ptr, m_last_mouse_pos.x, m_last_mouse_pos.y );
	}
}

void Desktop::SendFakeMouseMoveEvent( std::shared_ptr<Widget> widget, int x, int y ) const {
	widget->HandleEvent( sf::Event::MouseMoved{ { x, y } } );
}

void Desktop::RecalculateWidgetLevels() {
	auto children_size = m_children.size();
	auto current_level = 0;

	std::reverse_iterator<WidgetsList::iterator> iter( std::end( m_children ) );
	std::reverse_iterator<WidgetsList::iterator> finish( std::begin( m_children ) );

	for( ; iter != finish; ++iter ) {
		(*iter)->SetHierarchyLevel( current_level );
		(*iter)->Invalidate();
		current_level += std::numeric_limits<int>::max() / static_cast<int>( children_size );
	}
}

bool Desktop::SetProperties( const std::string& properties ) {
	if( !m_context.GetEngine().SetProperties( properties ) ) {
		return false;
	}

	Refresh();
	return true;
}

}
