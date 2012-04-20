#include <SFGUI/Label.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Font.hpp>

namespace sfg {

Label::Label( const sf::String& text ) :
	Misc(),
	m_text( text )
{
	SetAlignment( sf::Vector2f( .5f, .5f ) );
	Invalidate();
}

Label::~Label() {
}

Label::Ptr Label::Create( const sf::String& text ) {
	Ptr label( new Label( text ) );
	label->RequestResize();
	return label;
}

void Label::SetText( const sf::String& text ) {
	m_text = text;
	RequestResize();
	Invalidate();
}

const sf::String& Label::GetText() const {
	return m_text;
}

RenderQueue* Label::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateLabelDrawable( DynamicPointerCast<const Label>( shared_from_this() ) );
}

sf::Vector2f Label::CalculateRequisition() {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	sf::Vector2f metrics = Context::Get().GetEngine().GetTextMetrics( m_text, font, font_size );
	metrics.y = Context::Get().GetEngine().GetFontLineHeight( font, font_size );

	return metrics;
}

const std::string& Label::GetName() const {
	static const std::string name( "Label" );
	return name;
}

}
