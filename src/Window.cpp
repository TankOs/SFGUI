#include <SFGUI/Window.hpp>
#include <SFGUI/RenderEngine.hpp>
#include <SFGUI/Context.hpp>
#include <iostream>

namespace sfg {

Window::Window() :
	Bin(),
	m_skipreallocation( false ),
	m_style( Toplevel )
{
	SetFlags( Draggable );

	OnAdd.Connect( &Window::HandleAdd, this );
	OnSizeAllocate.Connect( &Window::HandleSizeAllocate, this );
	OnDragMove.Connect( &Window::HandleDragMove, this );
}

Window::Ptr Window::Create() {
	Window::Ptr  window( new Window );

	// Allocate minimum size.
	window->AllocateSize( sf::FloatRect( 0, 0, 1, 1 ) );

	return window;
}

sf::Drawable* Window::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetRenderEngine().CreateWindowDrawable( boost::shared_dynamic_cast<Window>( shared_from_this() ), target );
}

void Window::SetTitle( const sf::String& title ) {
	m_title = title;
	Invalidate();
}

const sf::String& Window::GetTitle() const {
	return m_title;
}

sf::FloatRect Window::GetClientRect() const {
	sf::FloatRect  clientrect( GetAllocation() );
	float  title_height( HasStyle( Titlebar ) ? Context::Get().GetRenderEngine().GetProperty<float>( "Window.Title.Height" ) : 0.f );
	float  border_width( Context::Get().GetRenderEngine().GetProperty<float>( "Window.BorderWidth" ) );

	clientrect.Left += border_width + GetBorderWidth();
	clientrect.Top += title_height + border_width + GetBorderWidth();
	clientrect.Width -= 2 * border_width + 2 * GetBorderWidth();
	clientrect.Height -= title_height + 2 * border_width + 2 * GetBorderWidth();

	return clientrect;
}

void Window::HandleAdd( Widget::Ptr /*widget*/, Widget::Ptr /*child*/ ) {
	RequestSize();
}

void Window::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& /*oldallocation*/ ) {
	if( !GetChild() || m_skipreallocation ) {
		m_skipreallocation = false;
		return;
	}

	// This is only called when the window's allocation has been changed from the
	// outside, i.e. not requested by a child.
	GetChild()->AllocateSize( GetClientRect() );
}

void Window::SetStyle( int style ) {
	m_style = style;
	RequestSize();
	Invalidate();
}

int Window::GetStyle() const {
	return m_style;
}

bool Window::HasStyle( Style style ) const {
	return m_style & style;
}

sf::Vector2f Window::GetRequisitionImpl() const {
	if( !GetChild() ) {
		return sf::Vector2f( 0, 0 );
	}

	sf::Vector2f  requisition( 2 * GetBorderWidth(), 2 * GetBorderWidth() );

	requisition += GetChild()->GetRequisition();

	if( HasStyle( Titlebar ) ) {
		float  visual_border_width( Context::Get().GetRenderEngine().GetWidgetProperty<float>( shared_from_this(), "Window.BorderWidth" ) );
		float  title_height( Context::Get().GetRenderEngine().GetWidgetProperty<float>( shared_from_this(), "Window.Title.Height" ) );

		requisition.x += visual_border_width;
		requisition.y += visual_border_width + title_height;
	}

	return requisition;
}

void Window::HandleDragMove( Widget::Ptr /*widget*/, const DragInfo& drag_info ) {
	if( HasStyle( Titlebar ) ) {
		SetPosition(
			sf::Vector2f(
				GetAllocation().Left + drag_info.GetDelta().x,
				GetAllocation().Top + drag_info.GetDelta().y
			)
		);
	}
}

}
