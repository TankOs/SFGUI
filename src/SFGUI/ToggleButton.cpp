#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

// Signals.
Signal::SignalID ToggleButton::OnToggle = 0;

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

RenderQueue* ToggleButton::InvalidateImpl() const {
	if( GetChild() ) {
		GetChild()->Invalidate();
	}

	return Context::Get().GetEngine().CreateToggleButtonDrawable( DynamicPointerCast<const ToggleButton>( shared_from_this() ) );
}

void ToggleButton::SetActive( bool active ) {
	if( active == m_active ) {
		return;
	}

	m_active = active;

	if( active ) {
		SetState( SELECTED );
	}
	else {
		SetState( NORMAL );
	}

	GetSignals().Emit( OnToggle );
	Invalidate();
}

bool ToggleButton::IsActive() const {
	return m_active;
}

void ToggleButton::HandleMouseClick( sf::Mouse::Button button, int x, int y ) {
	if( button == sf::Mouse::Left ) {
		SetActive( !IsActive() );
	}

	Button::HandleMouseClick( button, x, y );
}

}
