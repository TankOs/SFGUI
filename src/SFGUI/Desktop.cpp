#include <SFGUI/Desktop.hpp>
#include <SFGUI/Container.hpp>

#include <SFML/Window.hpp>

namespace sfg {

Desktop::Desktop( const sf::FloatRect& viewport ) :
	m_view( viewport ),
	m_do_refresh( false )
{
}

Desktop::Desktop( const sf::Window& window ) :
	m_view( sf::FloatRect( 0.f, 0.f, static_cast<float>( window.GetWidth() ), static_cast<float>( window.GetHeight() ) ) )
{
}

void Desktop::Expose( sf::RenderTarget& target ) const {
	CullingTarget culling_target( target );
	culling_target.Cull( false );

	Expose( culling_target );
}

void Desktop::Expose( CullingTarget& target ) const {
	if( m_do_refresh ) {
		Refresh();
		m_do_refresh = false;
	}

	// Cache current view and set ours.
	const sf::View& old_view( target.GetView() );
	target.SetView( m_view );

	// Activate context.
	Context::Activate( m_context );

	// Expose children.
	WidgetsList::const_reverse_iterator w_iter( m_children.end() );
	WidgetsList::const_reverse_iterator w_iter_end( m_children.begin() );

	for( ; w_iter != w_iter_end; ++w_iter ) {
		(*w_iter)->Expose( target );
	}

	// Restore previous context.
	Context::Deactivate();

	// Restore previous view.
	target.SetView( old_view );
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

	RemoveObsoleteChildren();

	sf::Vector2f local_pos;
	WidgetsList::iterator w_iter( m_children.begin() );
	WidgetsList::iterator w_iter_end( m_children.end() );
	bool check_inside( false );
	WidgetsList::iterator new_top_iter( m_children.end() );
	Widget::Ptr last_receiver( m_last_receiver.lock() );

	// If we've got a mouse event, get local mouse position and mark event for being checked against widget's allocation.
	if( event.Type == sf::Event::MouseMoved ) {
		local_pos = TransformToLocal( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) );
		check_inside = true;
	}
	else if( event.Type == sf::Event::MouseButtonPressed || event.Type == sf::Event::MouseButtonReleased ) {
		local_pos = TransformToLocal( sf::Vector2f( static_cast<float>( event.MouseButton.X ), static_cast<float>( event.MouseButton.Y ) ) );
		check_inside = true;
	}

	for( ; w_iter != w_iter_end; ++w_iter ) {
		// Skip widget if not visible.
		if( !(*w_iter)->IsVisible() ) {
			continue;
		}

		// If the event is a mouse button press, check if we need to focus another widget.
		if(
			new_top_iter == w_iter_end &&
			event.Type == sf::Event::MouseButtonPressed &&
			w_iter != m_children.begin()
		) {
			if( (*w_iter)->GetAllocation().Contains( local_pos ) ) {
				// Mark this widget as new top widget.
				new_top_iter = w_iter;
			}
		}

		bool is_inside( (*w_iter)->GetAllocation().Contains( local_pos ) );

		// If inside check is needed, do so for all widgets except the top window.
		// Else it would run into trouble when moving the window, for example,
		// where the mouse may be outside the widget's allocation.
		if( check_inside && !is_inside && w_iter != m_children.begin() ) {
			continue;
		}

		// If last receiver is different from current, fake a out-of-scope mouse
		// move event so that states are reset correctly. Warning, this is a hack,
		// but it works™.
		// The fake event is also sent when the last mouse move event receiver
		// isn't the current and top one.
		if( event.Type == sf::Event::MouseMoved && last_receiver && last_receiver != *w_iter && last_receiver != m_children.front() ) {
			sf::Event fake_event;
			fake_event.Type = sf::Event::MouseMoved;
			fake_event.MouseMove.X = -1337;
			fake_event.MouseMove.Y = -1337;
			last_receiver->HandleEvent( fake_event );

			m_last_receiver = *w_iter;
		}

		(*w_iter)->HandleEvent( event );

		if( check_inside && is_inside ) {
			m_last_receiver = *w_iter;
			break;
		}
	}

	if( new_top_iter != w_iter_end ) {
		sfg::Widget::Ptr widget( *new_top_iter );
		m_children.erase( new_top_iter );
		m_children.push_front( widget );
	}

	// Restore previous context.
	Context::Deactivate();
}

void Desktop::Add( std::shared_ptr<Widget> widget ) {
	m_children.push_front( widget );
	widget->Refresh();
}

void Desktop::Remove( std::shared_ptr<Widget> widget ) {
	// We do this in an isolated method to keep iterators valid.
	m_obsolete_children.push_back( widget );

	widget->Show( false );
}

void Desktop::RemoveObsoleteChildren() {
	WidgetsList::iterator obs_iter( m_obsolete_children.begin() );
	WidgetsList::iterator obs_iter_end( m_obsolete_children.end() );
	Widget::Ptr last_receiver( m_last_receiver.lock() );

	for( ; obs_iter != obs_iter_end; ++obs_iter ) {
		WidgetsList::iterator w_iter( std::find( m_children.begin(), m_children.end(), *obs_iter ) );

		if( w_iter == m_children.end() ) {
			continue;
		}

		// Reset last receiver if it's the widget to be removed.
		if( last_receiver && last_receiver == *w_iter ) {
			m_last_receiver.reset();
		}

		m_children.erase( w_iter );
	}

	m_obsolete_children.clear();
}


void Desktop::Refresh() const {
	// Activate context.
	Context::Activate( m_context );

	WidgetsList::const_reverse_iterator w_iter( m_children.end() );
	WidgetsList::const_reverse_iterator w_iter_end( m_children.begin() );

	for( ; w_iter != w_iter_end; ++w_iter ) {
		(*w_iter)->Refresh();
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

}
