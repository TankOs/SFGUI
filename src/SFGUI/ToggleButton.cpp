#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

// Signals.
Signal::SignalID ToggleButton::OnToggle = 0;

ToggleButton::ToggleButton() :
	m_active( false )
{
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

std::unique_ptr<RenderQueue> ToggleButton::InvalidateImpl() const {
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

	if( active ) {
		SetState( State::SELECTED );
	}
	else {
		SetState( State::NORMAL );
	}

	Invalidate();
	GetSignals().Emit( OnToggle );
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
