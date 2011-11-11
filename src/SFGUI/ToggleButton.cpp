#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ToggleButton::ToggleButton() :
	Button(),
	m_active( false )
{
}

ToggleButton::~ToggleButton() {
}

ToggleButton::Ptr ToggleButton::Create( const sf::String& label ) {
	Ptr button( new ToggleButton );

	button->SetLabel( label );

	return button;
}

const std::string& ToggleButton::GetName() const {
	static const std::string name( "ToggleButton" );
	return name;
}

sf::Drawable* ToggleButton::InvalidateImpl() const {
	if( GetChild() ) {
		GetChild()->Invalidate();
	}

	return Context::Get().GetEngine().CreateToggleButtonDrawable( std::dynamic_pointer_cast<const ToggleButton>( shared_from_this() ) );
}

void ToggleButton::SetActive( bool active ) {
	if( active == m_active ) {
		return;
	}

	m_active = active;

	OnToggle();
	Invalidate();
}

bool ToggleButton::IsActive() const {
	return m_active;
}

void ToggleButton::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( !IsMouseInWidget() ) {
		SetState( Normal );
		return;
	}

	if( button != sf::Mouse::Left ) {
		return;
	}

	if( press ) {
		SetState( Active );
	}
	else {
		SetState( Prelight );
		SetActive( !IsActive() );
	}
}

}
