#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderEngine.hpp>
#include <iostream> // XXX

namespace sfg {

Button::Button() :
	Widget(),
	m_caption( L"" )
{
	OnStateChange.Connect( &Button::HandleStateChange, this );
	OnMouseEnter.Connect( &Button::HandleMouseEnter, this );
	OnMouseLeave.Connect( &Button::HandleMouseLeave, this );
	OnMouseButtonClick.Connect( &Button::HandleMouseButtonClick, this );
	OnMouseButtonPress.Connect( &Button::HandleMouseButtonPress, this );
	OnMouseButtonRelease.Connect( &Button::HandleMouseButtonRelease, this );
}

Button::Ptr Button::Create( const sf::String& caption ) {
	Button::Ptr  ptr( new Button );

	ptr->SetCaption( caption );

	return ptr;
}

sf::Drawable* Button::InvalidateImpl() {
	return Context::Get().GetRenderEngine().CreateButtonDrawable( boost::shared_dynamic_cast<Button>( shared_from_this() ) );
}

void Button::SetCaption( const sf::String& caption ) {
	m_caption = caption;

	// Request new size.
	sf::Vector2f  metrics( Context::Get().GetRenderEngine().GetMetrics( m_caption ) );
	RequestSize( metrics );

	Invalidate();
}

const sf::String& Button::GetCaption() const {
	return m_caption;
}

void Button::HandleStateChange( sfg::Widget::Ptr /*widget*/, State /*oldstate*/ ) {
	Invalidate();
}

void Button::HandleMouseEnter( sfg::Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Prelight );
	}
}

void Button::HandleMouseLeave( sfg::Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Normal );
	}
}

void Button::HandleMouseButtonClick( sfg::Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button button ) {
	if( button == sf::Mouse::Left ) {
		OnClick.Sig( shared_from_this() );
	}
}

void Button::HandleMouseButtonPress( sfg::Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button button ) {
	if( button == sf::Mouse::Left ) {
		SetState( Active );
	}
}

void Button::HandleMouseButtonRelease( sfg::Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button /*button*/ ) {
	SetState( IsMouseInWidget() ? Prelight : Normal );
}

}
