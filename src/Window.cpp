#include <SFGUI/Window.hpp>
#include <SFGUI/RenderEngine.hpp>
#include <iostream>

namespace sfg {

Window::Window() :
	Bin()
{
}

Window::Ptr Window::Create() {
	Window::Ptr  window( new Window );

	// Allocate minimum size.
	window->AllocateSize( sf::FloatRect( 0, 0, 50, 50 ) );

	return window;
}

sf::Drawable* Window::InvalidateImpl() {
	return GetRenderEngine()->CreateWindowDrawable( boost::shared_dynamic_cast<Window>( shared_from_this() ) );
}

void Window::SetTitle( const sf::String& title ) {
	m_title = title;
	Invalidate();
}

const sf::String& Window::GetTitle() const {
	return m_title;
}

sf::FloatRect Window::GetClientRect() const {
	if( !GetRenderEngine() ) {
		return GetAllocation();
	}

	sf::FloatRect  clientrect( GetAllocation() );
	float  title_size( GetRenderEngine()->GetProperty( "Window.title-size", 10.f ) );
	float  border_width( GetRenderEngine()->GetProperty( "Window.border-width", 2.f ) );

	clientrect.Left += border_width;
	clientrect.Top += title_size + border_width;
	clientrect.Width -= 2 * border_width;
	clientrect.Height -= title_size + 2 * border_width;

	return clientrect;
}

}
