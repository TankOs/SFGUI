#include <SFGUI/Widget.hpp>
#include <SFGUI/RenderEngine.hpp>

namespace sfg {
Widget::Widget() :
	m_sensitive( true ),
	m_visible( true ),
	m_state( Normal ),
	m_rect( 0, 0, 0, 0 ),
	m_renderengine( 0 )
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

void Widget::GrabFocus() {
	GrabFocus( shared_from_this() );
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
	m_rect = rect;
	OnConfigure.Sig( shared_from_this() );
	Invalidate();
}

const sf::FloatRect& Widget::GetRect() const {
	return m_rect;
}

void Widget::Expose( sf::RenderTarget& target ) const {
	if( !m_drawable ) {
		return;
	}

	target.Draw( *m_drawable );
}

const RenderEngine* Widget::GetRenderEngine() const {
	return m_renderengine;
}

void Widget::SetRenderEngine( const RenderEngine& engine ) {
	m_renderengine = &engine;
	Invalidate();
}

void Widget::Invalidate() {
	if( !m_renderengine ) {
		return;
	}

	m_drawable.reset( InvalidateImpl() );

	if( m_drawable ) {
		m_drawable->SetPosition( GetRect().Left, GetRect().Top );
	}
}

sf::Drawable* Widget::InvalidateImpl() {
	return 0;
}

}
