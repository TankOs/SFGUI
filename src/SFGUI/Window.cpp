#include <SFGUI/Window.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Window::Window( int style ) :
	Bin(),
	m_style( style ),
	m_dragging( false ),
	m_resizing( false )
{
}

Window::~Window() {
}

Window::Ptr Window::Create( int style ) {
	Window::Ptr window( new Window( style ) );

	window->RequestResize();

	return window;
}

RenderQueue* Window::InvalidateImpl() const {
	if( GetChild() ) {
		GetChild()->SetAllocation( GetClientRect() );
	}

	return Context::Get().GetEngine().CreateWindowDrawable( DynamicPointerCast<const Window>( shared_from_this() ) );
}

void Window::SetTitle( const sf::String& title ) {
	m_title = title;
	Invalidate();
}

const sf::String& Window::GetTitle() const {
	return m_title;
}

sf::FloatRect Window::GetClientRect() const {
	sf::FloatRect clientrect( 0, 0, GetAllocation().width, GetAllocation().height );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float gap( Context::Get().GetEngine().GetProperty<float>( "Gap", shared_from_this() ) );

	clientrect.left += border_width + gap;
	clientrect.top += border_width + gap;
	clientrect.width -= 2 * border_width + 2 * gap;
	clientrect.height -= 2 * border_width + 2 * gap;

	if( HasStyle( TITLEBAR ) ) {
		unsigned int title_font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
		const sf::Font& title_font( *Context::Get().GetEngine().GetResourceManager().GetFont( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) ) );
		float title_height(
			Context::Get().GetEngine().GetFontLineHeight( title_font, title_font_size ) +
			2 * Context::Get().GetEngine().GetProperty<float>( "TitlePadding", shared_from_this() )
		);

		clientrect.top += title_height;
		clientrect.height -= title_height;
	}

	return clientrect;
}

void Window::HandleSizeChange() {
	if( !GetChild() ) {
		return;
	}

	GetChild()->SetAllocation( GetClientRect() );
}

void Window::SetStyle( int style ) {
	m_style = style;

	// Make sure dragging and resizing operations are cancelled.
	m_dragging = false;
	m_resizing = false;

	RequestResize();
	Invalidate();

	if( GetChild() ) {
		GetChild()->SetAllocation( GetClientRect() );
	}
}

int Window::GetStyle() const {
	return m_style;
}

bool Window::HasStyle( Style style ) const {
	return (m_style & style) == style;
}

sf::Vector2f Window::CalculateRequisition() {
	float visual_border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float gap( Context::Get().GetEngine().GetProperty<float>( "Gap", shared_from_this() ) );
	sf::Vector2f requisition( 2 * visual_border_width + 2 * gap, 2 * visual_border_width + 2 * gap );

	if( HasStyle( TITLEBAR ) ) {
		unsigned int title_font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
		const sf::Font& title_font( *Context::Get().GetEngine().GetResourceManager().GetFont( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) ) );
		float title_height(
			Context::Get().GetEngine().GetFontLineHeight( title_font, title_font_size ) +
			2 * Context::Get().GetEngine().GetProperty<float>( "TitlePadding", shared_from_this() )
		);

		requisition.y += title_height;
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

	unsigned int title_font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& title_font( *Context::Get().GetEngine().GetResourceManager().GetFont( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) ) );
	float title_height(
		Context::Get().GetEngine().GetFontLineHeight( title_font, title_font_size ) +
		2 * Context::Get().GetEngine().GetProperty<float>( "TitlePadding", shared_from_this() )
	);

	// Check for mouse being inside the title area.
	sf::FloatRect area(
		GetAllocation().left,
		GetAllocation().top,
		GetAllocation().width,
		title_height
	);

	if( area.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
		if( HasStyle( TITLEBAR ) && !m_dragging ) {
			m_dragging = true;
			m_resizing = false;

			m_drag_offset = sf::Vector2f(
				static_cast<float>( x ) - GetAllocation().left,
				static_cast<float>( y ) - GetAllocation().top
			);
		}
	}
	else {
		float handle_size( Context::Get().GetEngine().GetProperty<float>( "HandleSize", shared_from_this() ) );

		area.left = GetAllocation().left + GetAllocation().width - handle_size;
		area.top = GetAllocation().top + GetAllocation().height - handle_size;
		area.width = handle_size;
		area.height = handle_size;

		if( area.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			m_dragging = false;
			m_resizing = true;

			m_drag_offset = sf::Vector2f(
				handle_size - static_cast<float>( x ) + GetAllocation().left + GetAllocation().width - handle_size,
				handle_size - static_cast<float>( y ) + GetAllocation().top + GetAllocation().height - handle_size
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
	else if( m_resizing && (GetStyle() & RESIZE) == RESIZE ) {
		SetAllocation(
			sf::FloatRect(
				GetAllocation().left,
				GetAllocation().top,
				std::max( GetRequisition().x, static_cast<float>( x ) + m_drag_offset.x - GetAllocation().left ),
				std::max( GetRequisition().y, static_cast<float>( y ) + m_drag_offset.y - GetAllocation().top )
			)
		);
	}
}

void Window::HandleAdd( const Widget::Ptr& child ) {
	Bin::HandleAdd( child );

	if( GetChild() ) {
		// Reset allocation so the window will be as large as required.
		SetAllocation( sf::FloatRect( GetAllocation().left, GetAllocation().top, 1.f, 1.f ) );
		RequestResize();
	}
}

}
