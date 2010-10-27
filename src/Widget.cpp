#include <SFGUI/Widget.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/Context.hpp>
#include <iostream>

namespace sfg {

Widget::Widget() :
	m_sensitive( true ),
	m_visible( true ),
	m_state( Normal ),
	m_mouse_in( false ),
	m_mouse_button_down( -1 ),
	m_allocation( 0, 0, 0, 0 ),
	m_requisition( 0, 0 ),
	m_invalidated( false )
{
}

Widget::~Widget() {
}

bool Widget::IsSensitive() const {
	return m_sensitive;
}

bool Widget::IsVisible() const {
	return m_visible;
}

void Widget::SetName( const std::string& name ) {
	m_name = name;
}

const std::string& Widget::GetName() const {
	return m_name;
}

void Widget::GrabFocus( Ptr widget ) {
	if( !m_parent ) {
		// Notify old focused widget.
		if( m_focus_widget ) {
			m_focus_widget->OnFocusChange.Sig( m_focus_widget );
		}

		m_focus_widget = widget;
		m_focus_widget->OnFocusChange.Sig( m_focus_widget );
	}
	else {
		m_parent->GrabFocus( widget );
	}
}

void Widget::AllocateSize( const sf::FloatRect& rect ) {
	sf::FloatRect  oldallocation( m_allocation );

	m_allocation = rect;
	OnSizeAllocate.Sig( shared_from_this(), oldallocation );

	Invalidate();
}

void Widget::RequestSize() {
	if( m_parent ) {
		m_parent->RequestSize();
	}
	else {
		sf::Vector2f  requisition( GetRequisition() );

		sf::FloatRect  allocation(
			GetAllocation().Left,
			GetAllocation().Top,
			std::max( GetAllocation().Width, requisition.x ),
			std::max( GetAllocation().Height, requisition.y )
		);

		AllocateSize( allocation );
	}
}


const sf::FloatRect& Widget::GetAllocation() const {
	return m_allocation;
}

void Widget::Expose( sf::RenderTarget& target ) {
	if( m_invalidated ) {
		m_invalidated = false;

		m_drawable.reset( InvalidateImpl() );

		if( m_drawable ) {
			m_drawable->SetPosition( GetAllocation().Left, GetAllocation().Top );
		}
	}

	if( IsVisible() ) {
		if( m_drawable ) {
			target.Draw( *m_drawable );
		}

		OnExpose.Sig( shared_from_this(), target );
	}
}

void Widget::Invalidate() {
	m_invalidated = true;
}

sf::Drawable* Widget::InvalidateImpl() {
	return 0;
}

void Widget::SetParent( Widget::Ptr parent ) {
	Container::Ptr  cont( boost::shared_dynamic_cast<Container>( parent ) );

	if( !cont ) {
		return;
	}

	if( m_parent ) {
		Container::Ptr  oldparent( boost::shared_dynamic_cast<Container>( m_parent ) );
		oldparent->Remove( shared_from_this() );
	}

	m_parent = cont;
}

void Widget::SetPosition( const sf::Vector2f& position ) {
	sf::FloatRect  oldallocation( GetAllocation() );

	m_allocation.Left = position.x;
	m_allocation.Top = position.y;

	if( m_drawable ) {
		m_drawable->SetPosition( position );
	}

	OnSizeAllocate.Sig( shared_from_this(), oldallocation );
}

Widget::HandleEventResult Widget::HandleEvent( const sf::Event& event ) {
	if( !IsVisible() ) {
		return DropEvent;
	}

	// Hooks come first.
	HooksMap::iterator  iter( m_hooks.find( event.Type ) );

	if( iter != m_hooks.end() ) {
		WidgetsList::iterator  w_iter( iter->second.begin() );

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

	if( event.Type == sf::Event::MouseMoved ) {
		// Check if pointer inside of widget's allocation.
		if( GetAllocation().Contains( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) {
			// Check for enter event.
			if( m_mouse_in == false ) {
				m_mouse_in = true;
				OnMouseEnter.Sig( shared_from_this(), event.MouseMove.X, event.MouseMove.Y );

				// Register hook to get notified when mouse leaves the widget.
				RegisterEventHook( sf::Event::MouseMoved, shared_from_this() );
			}

			OnMouseMove.Sig( shared_from_this(), event.MouseMove.X, event.MouseMove.Y );
		}
		else if( m_mouse_in == true ) { // Check for leave event.
			m_mouse_in = false;
			UnregisterEventHook( sf::Event::MouseMoved, shared_from_this() );

			OnMouseLeave.Sig( shared_from_this(), event.MouseMove.X, event.MouseMove.Y );

			// Mouse left the widget's region, so don't continue to pass the event to
			// children. The event only reached the widget because the event got
			// hooked.
			return DropEvent;
		}
		else {
			return DropEvent;
		}

	}
	else if( event.Type == sf::Event::MouseButtonPressed  ) {
		// If a mouse button has already been pressed for this widget, drop further
		// presses. This maybe needs changing, but up to now, I can't think of any
		// cases where it would be useful to have such a functionality.
		if( m_mouse_button_down == -1 ) {
			if( m_mouse_in ) {
				m_mouse_button_down = event.MouseButton.Button;
				RegisterEventHook( sf::Event::MouseButtonReleased, shared_from_this() );

				OnMouseButtonPress.Sig( shared_from_this(), event.MouseButton.X, event.MouseButton.Y, event.MouseButton.Button );
			}
			else {
				return DropEvent;
			}
		}
	}
	else if( event.Type == sf::Event::MouseButtonReleased ) {
		// Only process when mouse button has been clicked inside the widget before.
		if( m_mouse_button_down == event.MouseButton.Button ) {
			m_mouse_button_down = -1;
			UnregisterEventHook( sf::Event::MouseButtonReleased, shared_from_this() );

			// When released inside the widget, the event can be considered a click.
			if( m_mouse_in ) {
				OnMouseButtonClick.Sig( shared_from_this(), event.MouseButton.X, event.MouseButton.Y, event.MouseButton.Button );
			}

			OnMouseButtonRelease.Sig( shared_from_this(), event.MouseButton.X, event.MouseButton.Y, event.MouseButton.Button );
		}
		else if( !m_mouse_in ) {
			return DropEvent;
		}
	}

	return PassEvent;
}

void Widget::SetState( State state ) {
	State  oldstate( m_state );
	m_state = state;

	OnStateChange.Sig( shared_from_this(), oldstate );
}

Widget::State Widget::GetState() const {
	return m_state;
}

Widget::Ptr Widget::GetParent() const {
	return m_parent;
}

void Widget::GrabFocus() {
	GrabFocus( shared_from_this() );
}

void Widget::RegisterEventHook( sf::Event::EventType event_type, Ptr widget ) {
	if( m_parent ) {
		m_parent->RegisterEventHook( event_type, widget );
		return;
	}

	// No need to register hook for self.
	if( widget == shared_from_this() ) {
		return;
	}

	// Check if hook for widget already registered. Clear delete flag if needed.
	WidgetsList&  list( m_hooks[event_type] );
	WidgetsList::iterator  iter( std::find( list.begin(), list.end(), widget ) );

	if( iter != list.end() ) {
		iter->remove = false;
	}
	else {
		m_hooks[event_type].push_back( WidgetBoolPair( widget, false ) );
	}
}

void Widget::UnregisterEventHook( sf::Event::EventType event_type, Ptr widget ) {
	if( m_parent ) {
		m_parent->UnregisterEventHook( event_type, widget );
		return;
	}

	WidgetsList&  list( m_hooks[event_type] );
	WidgetsList::iterator  iter( std::find( list.begin(), list.end(), widget ) );

	if( iter != list.end() ) {
		iter->remove = true;
	}
}

bool Widget::IsMouseInWidget() const {
	return m_mouse_in;
}

Widget::WidgetBoolPair::WidgetBoolPair( Ptr widget_, bool remove_ ) :
	widget( widget_ ),
	remove( remove_ )
{
}

bool Widget::WidgetBoolPair::operator==( const WidgetBoolPair& rhs ) {
	return widget == rhs.widget;
}

bool Widget::WidgetBoolPair::operator==( const Ptr& rhs ) {
	return widget == rhs;
}

void Widget::Show( bool show ) {
	m_visible = show;
}

}
