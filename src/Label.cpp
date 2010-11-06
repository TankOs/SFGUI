#include <SFGUI/Label.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderEngine.hpp>

namespace sfg {

Label::Label( const sf::String& text ) :
	m_text( text )
{
	Invalidate();
}

Label::~Label() {
}

Label::Ptr Label::Create( const sf::String& text ) {
	Ptr  label( new Label( text ) );
	return label;
}

void Label::SetText( const sf::String& text ) {
	m_text = text;
	Invalidate();
}

const sf::String& Label::GetText() const {
	return m_text;
}

sf::Drawable* Label::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetRenderEngine().CreateLabelDrawable( boost::shared_dynamic_cast<Label>( shared_from_this() ), target );
}

sf::Vector2f Label::GetRequisition() const {
	return sf::Vector2f( 50, 50 );
}

}
