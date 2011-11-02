#include <SFGUI/Window.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Window::Window() :
	Bin(),
	m_skipreallocation( false ),
	m_style( Toplevel )
{
	SetFlags( Draggable );
}

Window::Ptr Window::Create() {
	Window::Ptr  window( new Window );

	// Allocate minimum size.
	window->AllocateSize( sf::FloatRect( 0, 0, 1, 1 ) );

	return window;
}

sf::Drawable* Window::InvalidateImpl() {
	return Context::Get().GetEngine().CreateWindowDrawable( boost::shared_dynamic_cast<Window>( shared_from_this() ) );
}

void Window::SetTitle( const sf::String& title ) {
	m_title = title;
	Invalidate();
}

const sf::String& Window::GetTitle() const {
	return m_title;
}

sf::FloatRect Window::GetClientRect() const {
	sf::FloatRect  clientrect( 0, 0, GetAllocation().Width, GetAllocation().Height );
	float  title_height( HasStyle( Titlebar ) ? Context::Get().GetEngine().GetProperty<float>( "TitleHeight", shared_from_this() ) : 0.f );
	float  border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	clientrect.Left += border_width + GetBorderWidth();
	clientrect.Top += title_height + border_width + GetBorderWidth();
	clientrect.Width -= 2 * border_width + 2 * GetBorderWidth();
	clientrect.Height -= title_height + 2 * border_width + 2 * GetBorderWidth();

	return clientrect;
}

void Window::HandleAdd( Widget::Ptr /*child*/ ) {
	RequestSize();
}

bool Window::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) {
	if( !GetChild() || m_skipreallocation ) {
		m_skipreallocation = false;
		return false;
	}

	// This is only called when the window's allocation has been changed from the
	// outside, i.e. not requested by a child.
	GetChild()->AllocateSize( GetClientRect() );

	return true;
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
		float  visual_border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
		float  title_height( Context::Get().GetEngine().GetProperty<float>( "TitleHeight", shared_from_this() ) );

		requisition.x += visual_border_width;
		requisition.y += visual_border_width + title_height;
	}

	return requisition;
}

bool Window::HandleDragOperation( DragInfo::State state, const DragInfo& drag_info ) {
	if( state == DragInfo::Move && HasStyle( Titlebar ) ) {
		SetPosition(
			sf::Vector2f(
				GetAllocation().Left + drag_info.GetDelta().x,
				GetAllocation().Top + drag_info.GetDelta().y
			)
		);
	}

	return true;
}

const std::string& Window::GetName() const {
	static const std::string name( "Window" );
	return name;
}

}
