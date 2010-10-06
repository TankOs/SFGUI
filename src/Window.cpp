#include <SFGUI/Window.hpp>
#include <SFGUI/RenderEngine.hpp>

namespace sfg {

Window::Window() :
	Bin()
{
}

Window::Ptr Window::Create( const sf::FloatRect& rect ) {
	Window::Ptr  window( new Window );

	window->AllocateSize( rect );

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

}
