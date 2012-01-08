#include <SFGUI/Desktop.hpp>
#include <SFGUI/Container.hpp>

#include <SFML/Window.hpp>
#include <limits>

namespace sfg {

Desktop::Desktop( const sf::FloatRect& viewport ) :
	m_view( viewport ),
	m_engine( 0 ),
	m_last_mouse_pos( 0, 0 )
{
}

Desktop::Desktop( const sf::Window& window ) :
	m_view( sf::FloatRect( 0.f, 0.f, static_cast<float>( window.GetWidth() ), static_cast<float>( window.GetHeight() ) ) ),
	m_engine( 0 ),
	m_last_mouse_pos( 0, 0 )
{
}

Desktop::~Desktop() {
	delete m_engine;
}

void Desktop::UpdateViewRect( const sf::FloatRect& rect ) {
	m_view.Reset( rect );
}

void Desktop::Update( float seconds ) {
	for( int index = static_cast<int>( m_children.size() ) - 1; index >= 0; --index ) {
		m_children[index]->Update( seconds );
	}
}

sf::Vector2f Desktop::TransformToLocal( const sf::Vector2f& global ) const {
	return sf::Vector2f(
		global.x - m_view.GetViewport().Left,
		global.y - m_view.GetViewport().Top
	);
}

void Desktop::HandleEvent( const sf::Event& event ) {
	// Activate context.
	Context::Activate( m_context );

	sf::Vector2f local_pos;
	bool check_inside( false );
	Widget::Ptr last_receiver( m_last_receiver.lock() );

	// If we've got a mouse event, get local mouse position and mark event for being checked against widget's allocation.
	if( event.Type == sf::Event::MouseMoved ) {
		m_last_mouse_pos.x = event.MouseMove.X;
		m_last_mouse_pos.y = event.MouseMove.Y;
		local_pos = TransformToLocal( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) );
		check_inside = true;
	}
	else if( event.Type == sf::Event::MouseButtonPressed || event.Type == sf::Event::MouseButtonReleased ) {
		m_last_mouse_pos.x = event.MouseButton.X;
		m_last_mouse_pos.y = event.MouseButton.Y;
		local_pos = TransformToLocal( sf::Vector2f( static_cast<float>( event.MouseButton.X ), static_cast<float>( event.MouseButton.Y ) ) );
		check_inside = true;
	}

	for( std::size_t index = 0; index < m_children.size(); ++index ) {
		Widget::Ptr widget( m_children[index] );

		// Skip widget if not visible.
		if( !widget->IsVisible() ) {
			continue;
		}

		bool is_inside( widget->GetAllocation().Contains( local_pos ) );

		// If the event is a mouse button press, check if we need to focus another widget.
		if(
			index > 0 &&
			event.Type == sf::Event::MouseButtonPressed &&
			is_inside
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
		if( event.Type == sf::Event::MouseMoved && last_receiver && last_receiver != widget && last_receiver != m_children.front() ) {
			SendFakeMouseMoveEvent( last_receiver );
			m_last_receiver = widget;
			last_receiver = widget;
		}

		widget->HandleEvent( event );

		if( check_inside && is_inside ) {
			if( index < m_children.size() && widget == m_children[index] ) {
				m_last_receiver = widget;
			}
			break;
		}
	}

	// Restore previous context.
	Context::Deactivate();
}

void Desktop::Add( SharedPtr<Widget> widget ) {
	if( std::find( m_children.begin(), m_children.end(), widget ) != m_children.end() ) {
		return;
	}

	// Get old focused widget out of PRELIGHT state if mouse is inside the new
	// widget.
	if( m_children.size() ) {
		if( widget->GetAllocation().Contains( static_cast<float>( m_last_mouse_pos.x ), static_cast<float>( m_last_mouse_pos.y ) ) ) {
			SendFakeMouseMoveEvent( m_children.front() );
		}
	}

	m_children.push_front( widget );

	RecalculateWidgetLevels();

	widget->Refresh();
	ResendMouseMoveEvent();
}

void Desktop::Remove( SharedPtr<Widget> widget ) {
	WidgetsList::iterator iter( std::find( m_children.begin(), m_children.end(), widget ));

	if( iter != m_children.end() ) {
		m_children.erase( iter );
	}

	if( m_last_receiver.lock() == widget ) {
		m_last_receiver.reset();
	}

	RecalculateWidgetLevels();

	if( m_children.size() ) {
		ResendMouseMoveEvent();
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

	for( int index = static_cast<int>( m_children.size() ) - 1; index >= 0; --index ) {
		m_children[index]->Refresh();
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

void Desktop::BringToFront( SharedPtr<const Widget> child ) {
	WidgetsList::iterator iter( std::find( m_children.begin(), m_children.end(), child ) );

	if( iter == m_children.end() || iter == m_children.begin() ) {
		return;
	}

	if( child->GetAllocation().Contains( static_cast<float>( m_last_mouse_pos.x ), static_cast<float>( m_last_mouse_pos.y ) ) ) {
		SendFakeMouseMoveEvent( m_children.front() );
	}

	Widget::Ptr ptr( *iter );
	m_children.erase( iter );
	m_children.push_front( ptr );

	RecalculateWidgetLevels();

	ResendMouseMoveEvent();
}

void Desktop::SendFakeMouseMoveEvent( SharedPtr<Widget> widget, int x, int y ) const {
	sf::Event fake_event;
	fake_event.Type = sf::Event::MouseMoved;
	fake_event.MouseMove.X = x;
	fake_event.MouseMove.Y = y;
	widget->HandleEvent( fake_event );
}

void Desktop::ResendMouseMoveEvent() {
	sf::Event event;
	event.Type = sf::Event::MouseMoved;
	event.MouseMove.X = m_last_mouse_pos.x;
	event.MouseMove.Y = m_last_mouse_pos.y;

	sf::Vector2f mouse_pos( static_cast<float>( m_last_mouse_pos.x ), static_cast<float>( m_last_mouse_pos.y ) );

	for( std::size_t index = 0; index < m_children.size(); ++index ) {
		if( m_children[index]->GetAllocation().Contains( mouse_pos ) ) {
			m_children[index]->HandleEvent( event );
			break;
		}
	}
}

void Desktop::RecalculateWidgetLevels() {
	std::size_t children_size = m_children.size();

	int current_level = 0;

	for( int index = static_cast<int>( children_size ) - 1; index >= 0; --index ) {
		m_children[index]->SetHierarchyLevel( current_level );

		current_level += std::numeric_limits<int>::max() / static_cast<int>( children_size );
	}
}

}
