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
		widget->SetGroup( SharedPtr<RadioButtonGroup>( new RadioButtonGroup ) );
	}

	return widget;
}

const SharedPtr<RadioButton::RadioButtonGroup>& RadioButton::GetGroup() const {
	return m_group;
}

void RadioButton::SetGroup( const SharedPtr<RadioButton::RadioButtonGroup>& group ) {
	WeakPtr<Widget> weak_this( shared_from_this() );

	if( m_group ) {
		m_group->members.erase( weak_this );
	}

	m_group = group;

	if( m_group ) {
		m_group->members.insert( weak_this );
	}
}

void RadioButton::SetActive( bool active ) {
	if( active && m_group ) {
		for( std::set< WeakPtr<Widget> >::const_iterator iter = m_group->members.begin(); iter != m_group->members.end(); ++iter ) {
			SharedPtr<Widget> widget( iter->lock() );

			if( widget ) {
				SharedPtr<RadioButton> radio_button( StaticPointerCast<RadioButton>( widget ) );

				radio_button->SetActive( false );
			}
			else {
				m_group->members.erase( iter );
			}
		}
	}

	ToggleButton::SetActive( active );
}

const std::string& RadioButton::GetName() const {
	static const std::string name( "RadioButton" );
	return name;
}

}
