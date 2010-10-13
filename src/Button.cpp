#include <SFGUI/Button.hpp>
#include <SFGUI/RenderEngine.hpp>
#include <iostream> // XXX

namespace sfg {

Button::Button() :
	Widget(),
	m_caption( L"" )
{
}

Button::Ptr Button::Create( const sf::String& caption ) {
	Button::Ptr  ptr( new Button );

	ptr->SetCaption( caption );

	return ptr;
}

sf::Drawable* Button::InvalidateImpl() {
	return GetRenderEngine()->CreateButtonDrawable( boost::shared_dynamic_cast<Button>( shared_from_this() ) );
}

void Button::SetCaption( const sf::String& caption ) {
	m_caption = caption;
	Invalidate();

	if( !GetRenderEngine() ) {
		RequestSize( sf::FloatRect( GetRequisition().Left, GetRequisition().Top, 10.f, 10.f ) );
	}
	else {
		RequestSize( sf::FloatRect( GetRequisition().Left, GetRequisition().Top, 5.f /*GetRenderEngine()->GetTextMetrics( m_caption ).x*/, GetRenderEngine()->GetProperty( "Button.caption-font-size", 10.f ) ) );
	}
}

}
