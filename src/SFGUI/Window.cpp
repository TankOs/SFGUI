#include <SFGUI/Window.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Window::Window() :
	Bin(),
	m_style( Toplevel ),
	m_dragging( false ),
	m_resizing( false )
{
	//SetFlags( Draggable ); // TODO: Do it via StartDrag() operation, not flags.
}

Window::~Window() {
}

Window::Ptr Window::Create() {
	Window::Ptr  window( new Window );

	window->RequestSize();

	return window;
}

RenderQueue* Window::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateWindowDrawable( std::dynamic_pointer_cast<const Window>( shared_from_this() ) );
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

void Window::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) const {
	if( !GetChild() ) {
		return;
	}

	GetChild()->AllocateSize( GetClientRect() );
}

void Window::SetStyle( int style ) {
	m_style = style;

	// Make sure dragging and resizing operations are cancelled.
	m_dragging = false;
	m_resizing = false;

	RequestSize();
	Invalidate();

	if( GetChild() ) {
		GetChild()->AllocateSize( GetClientRect() );
	}
}

int Window::GetStyle() const {
	return m_style;
}

bool Window::HasStyle( Style style ) const {
	return (m_style & style) == style;
}

sf::Vector2f Window::GetRequisitionImpl() const {
	sf::Vector2f requisition( 2 * GetBorderWidth(), 2 * GetBorderWidth() );

	if( HasStyle( Titlebar ) ) {
		float  visual_border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
		float  title_height( Context::Get().GetEngine().GetProperty<float>( "TitleHeight", shared_from_this() ) );

		requisition.x += visual_border_width;
		requisition.y += visual_border_width + title_height;
	}

	if( GetChild() ) {
		requisition += GetChild()->GetRequisition();
	}
	else {
		requisition.x = std::max( 50.f, requisition.x );
		requisition.y = std::max( 50.f, requisition.y * 2.f );
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

	if( !press ) {
		m_dragging = false;
		m_resizing = false;
		return;
	}

	// Check for mouse being inside the title area.
	sf::FloatRect area(
		GetAllocation().Left,
		GetAllocation().Top,
		GetAllocation().Width,
		Context::Get().GetEngine().GetProperty<float>( "TitleHeight", shared_from_this() )
	);

	if( area.Contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
		if( HasStyle( Titlebar ) && !m_dragging ) {
			m_dragging = true;
			m_resizing = false;

			m_drag_offset = sf::Vector2f(
				static_cast<float>( x ) - GetAllocation().Left,
				static_cast<float>( y ) - GetAllocation().Top
			);
		}
	}
	else {
		float handle_size( Context::Get().GetEngine().GetProperty<float>( "HandleSize", shared_from_this() ) );

		area.Left = GetAllocation().Left + GetAllocation().Width - handle_size;
		area.Top = GetAllocation().Top + GetAllocation().Height - handle_size;
		area.Width = handle_size;
		area.Height = handle_size;

		if( area.Contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			m_dragging = false;
			m_resizing = true;

			m_drag_offset = sf::Vector2f(
				handle_size - static_cast<float>( x ) + GetAllocation().Left + GetAllocation().Width - handle_size,
				handle_size - static_cast<float>( y ) + GetAllocation().Top + GetAllocation().Height - handle_size
			);
		}
	}

}

void Window::HandleMouseMoveEvent( int x, int y ) {
	if( m_dragging ) {
		SetPosition(
			sf::Vector2f(
				static_cast<float>( x ) - m_drag_offset.x,
				static_cast<float>( y ) - m_drag_offset.y
			)
		);
	}
	else if( m_resizing ) {
		AllocateSize(
			sf::FloatRect(
				GetAllocation().Left,
				GetAllocation().Top,
				std::max( GetRequisition().x, static_cast<float>( x ) + m_drag_offset.x - GetAllocation().Left ),
				std::max( GetRequisition().y, static_cast<float>( y ) + m_drag_offset.y - GetAllocation().Top )
			)
		);
	}
}

void Window::HandleAdd( Widget::Ptr child ) {
	Bin::HandleAdd( child );

	if( GetChild() ) {
		// Reset allocation so the window will be as large as required.
		AllocateSize( sf::FloatRect( GetAllocation().Left, GetAllocation().Top, 1.f, 1.f ) );
		RequestSize();
	}
}

}
