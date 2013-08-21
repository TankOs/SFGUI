#include <SFGUI/RadioButton.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

RadioButton::RadioButton() :
	CheckButton(),
	m_group()
{
}

RadioButton::~RadioButton() {
}

RadioButton::Ptr RadioButton::Create( const sf::String& label, const SharedPtr<RadioButtonGroup>& group ) {
	Ptr widget( new RadioButton );

	widget->SetLabel( label );

	if( group ) {
		widget->SetGroup( group );
	}
	else {
		widget->SetGroup( RadioButtonGroup::Create() );
	}

	return widget;
}

const SharedPtr<RadioButton::RadioButtonGroup>& RadioButton::GetGroup() const {
	return m_group;
}

void RadioButton::SetGroup( const SharedPtr<RadioButton::RadioButtonGroup>& group ) {
	WeakPtr<RadioButton> weak_this( StaticPointerCast<RadioButton>( shared_from_this() ) );

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
		for( std::set< WeakPtr<RadioButton> >::const_iterator iter = m_group->GetMembers().begin(); iter != m_group->GetMembers().end(); ++iter ) {
			SharedPtr<RadioButton> radio_button( iter->lock() );

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

RadioButton::RadioButtonGroup::RadioButtonGroup() {
}

RadioButton::RadioButtonGroup::Ptr RadioButton::RadioButtonGroup::Create() {
	return Ptr( new RadioButtonGroup );
}

std::set< WeakPtr<RadioButton> >& RadioButton::RadioButtonGroup::GetMembers() {
	return m_members;
}

}
