#include <SFGUI/Label.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Font.hpp>

namespace sfg {

Label::Label( const sf::String& text ) :
	m_text( text ),
	m_alignment( .5f, .5f )
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
	RequestSize();
	Invalidate();
}

const sf::String& Label::GetText() const {
	return m_text;
}

RenderQueue* Label::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateLabelDrawable( std::dynamic_pointer_cast<const Label>( shared_from_this() ) );
}

sf::Vector2f Label::GetRequisitionImpl() const {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	sf::Vector2f metrics = Context::Get().GetEngine().GetTextMetrics( m_text, font, font_size );
	metrics.y = Context::Get().GetEngine().GetLineHeight( font, font_size );

	return metrics;
}

void Label::SetAlignment( const sf::Vector2f& alignment ) {
	m_alignment.x = std::max( 0.f, std::min( 1.f, alignment.x ) );
	m_alignment.y = std::max( 0.f, std::min( 1.f, alignment.y ) );
}

const sf::Vector2f& Label::GetAlignment() const {
	return m_alignment;
}

const std::string& Label::GetName() const {
	static const std::string name( "Label" );
	return name;
}

}
