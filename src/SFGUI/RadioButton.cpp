#include <SFGUI/RadioButton.hpp>
#include <SFGUI/RadioButtonGroup.hpp>

namespace sfg {

RadioButton::Ptr RadioButton::Create( const sf::String& label, RadioButtonGroup::Ptr group ) {
	auto widget = Ptr( new RadioButton );

	widget->SetLabel( label );

	if( group ) {
		widget->SetGroup( group );
	}
	else {
		widget->SetGroup( RadioButtonGroup::Create() );
	}

	return widget;
}

RadioButtonGroup::Ptr RadioButton::GetGroup() const {
	return m_group;
}

void RadioButton::SetGroup( RadioButtonGroup::Ptr group ) {
	auto weak_this = std::weak_ptr<RadioButton>( std::static_pointer_cast<RadioButton>( shared_from_this() ) );

	if( m_group ) {
		m_group->GetMembers().erase( weak_this );
	}

	m_group = group;

	if( m_group ) {
		m_group->GetMembers().insert( weak_this );
	}
}

void RadioButton::SetActive( bool active ) {
	if( active && m_group ) {
		for( auto iter = m_group->GetMembers().begin(); iter != m_group->GetMembers().end(); ++iter ) {
			std::shared_ptr<RadioButton> radio_button( iter->lock() );

			if( radio_button ) {
				radio_button->SetActive( false );
			}
			else {
				m_group->GetMembers().erase( iter );
			}
		}
	}

	ToggleButton::SetActive( active );
}

void RadioButton::HandleMouseClick( sf::Mouse::Button button, int x, int y ) {
	if( !IsActive() && ( button == sf::Mouse::Left ) ) {
		SetActive( true );
	}

	Button::HandleMouseClick( button, x, y );
}

const std::string& RadioButton::GetName() const {
	static const std::string name( "RadioButton" );
	return name;
}

}
