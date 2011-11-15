#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Button::Button() :
	Bin()
{
}

Button::~Button() {
}

Button::Ptr Button::Create( const sf::String& label ) {
	Button::Ptr  ptr( new Button );

	ptr->SetLabel( label );

	return ptr;
}

RenderQueue* Button::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateButtonDrawable( std::dynamic_pointer_cast<const Button>( shared_from_this() ) );

}

void Button::SetLabel( const sf::String& label ) {
	m_label = label;
	RequestSize();
	Invalidate();
}

const sf::String& Button::GetLabel() const {
	return m_label;
}

void Button::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( GetState() == Normal ) {
		SetState( Prelight );
	}
}

void Button::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( GetState() == Prelight ) {
		SetState( Normal );
	}
}

void Button::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( !IsMouseInWidget() ) {
		SetState( Normal );
		return;
	}

	if( button == sf::Mouse::Left ) {
		if( press ) {
			SetState( Active );
		}
		else {
			SetState( Prelight );
		}
	}
}

sf::Vector2f Button::GetRequisitionImpl() const {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	sf::Vector2f metrics = Context::Get().GetEngine().GetTextMetrics( m_label, font, font_size );
	metrics.y = Context::Get().GetEngine().GetLineHeight( font, font_size );

	sf::Vector2f requisition(
		metrics.x + 2 * GetBorderWidth() + 2 * padding,
		metrics.y + 2 * GetBorderWidth() + 2 * padding
	);

	return requisition;
}

const std::string& Button::GetName() const {
	static const std::string name( "Button" );
	return name;
}

void Button::HandleMouseClick( sf::Mouse::Button button, int /*x*/, int /*y*/ ) {
	if( button != sf::Mouse::Left ) {
		return;
	}

	OnClick();
}

}
