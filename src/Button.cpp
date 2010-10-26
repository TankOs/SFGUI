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
	SetState( Prelight );
}

void Button::HandleMouseLeave( sfg::Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	SetState( Normal );
}

}
