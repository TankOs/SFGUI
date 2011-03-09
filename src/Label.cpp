#include <SFGUI/Label.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderEngine.hpp>

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

sf::Drawable* Label::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetRenderEngine().CreateLabelDrawable( boost::shared_dynamic_cast<Label>( shared_from_this() ), target );
}

sf::Vector2f Label::GetRequisitionImpl() const {
	std::string default_( "arial.ttf" );
	const std::string& font_name( Context::Get().GetRenderEngine().GetWidgetProperty<std::string>( shared_from_this(), "Label.Font", "" ) );
	const sf::Font& font( Context::Get().GetRenderEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetRenderEngine().GetWidgetProperty<unsigned int>( shared_from_this(), "Label.FontSize", 10 ) );

	return Context::Get().GetRenderEngine().GetTextMetrics( m_text, font, font_size );
}

void Label::SetAlignment( const sf::Vector2f& alignment ) {
	m_alignment.x = std::max( 0.f, std::min( 1.f, alignment.x ) );
	m_alignment.y = std::max( 0.f, std::min( 1.f, alignment.y ) );
}

const sf::Vector2f& Label::GetAlignment() const {
	return m_alignment;
}

}
