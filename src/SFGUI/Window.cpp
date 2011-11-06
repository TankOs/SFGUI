#include <SFGUI/Window.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Window::Window() :
	Bin(),
	m_skipreallocation( false ),
	m_style( Toplevel ),
	m_dragging( false )
{
	//SetFlags( Draggable ); // TODO: Do it via StartDrag() operation, not flags.
}

Window::Ptr Window::Create() {
	Window::Ptr  window( new Window );

	// Allocate minimum size.
	window->AllocateSize( sf::FloatRect( 0, 0, 1, 1 ) );

	return window;
}

sf::Drawable* Window::InvalidateImpl() {
	return Context::Get().GetEngine().CreateWindowDrawable( std::dynamic_pointer_cast<Window>( shared_from_this() ) );
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

void Window::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) {
	if( !GetChild() || m_skipreallocation ) {
		m_skipreallocation = false;
		return;
	}

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
	return (m_style & style) == style;
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

const std::string& Window::GetName() const {
	static const std::string name( "Window" );
	return name;
}

void Window::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) {
	if( button != sf::Mouse::Left ) {
		return;
	}

	// Check for mouse being inside the title area.
	sf::FloatRect title_area(
		GetAllocation().Left,
		GetAllocation().Top,
		GetAllocation().Width,
		Context::Get().GetEngine().GetProperty<float>( "TitleHeight", shared_from_this() )
	);

	if( !title_area.Contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
		m_dragging = false;
		return;
	}

	m_drag_offset = sf::Vector2f(
		static_cast<float>( x ) - GetAllocation().Left,
		static_cast<float>( y ) - GetAllocation().Top
	);

	m_dragging = press;
}

void Window::HandleMouseMoveEvent( int x, int y ) {
	if( !m_dragging ) {
		return;
	}

	SetPosition(
		sf::Vector2f(
			static_cast<float>( x ) - m_drag_offset.x,
			static_cast<float>( y ) - m_drag_offset.y
		)
	);
}

}
