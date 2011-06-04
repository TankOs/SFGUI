#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <iostream> // XXX

namespace sfg {

Button::Button() :
	Bin(),
	m_label( Label::Create( L"" ) ),
	m_padding( 5.f )
{
	OnStateChange.Connect( &Button::HandleStateChange, this );
	OnMouseEnter.Connect( &Button::HandleMouseEnter, this );
	OnMouseLeave.Connect( &Button::HandleMouseLeave, this );
	OnMouseButtonClick.Connect( &Button::HandleMouseButtonClick, this );
	OnMouseButtonPress.Connect( &Button::HandleMouseButtonPress, this );
	OnMouseButtonRelease.Connect( &Button::HandleMouseButtonRelease, this );
	OnSizeAllocate.Connect( &Button::HandleSizeAllocate, this );
}

Button::Ptr Button::Create( const sf::String& label ) {
	Button::Ptr  ptr( new Button );

	ptr->SetLabel( label );
	ptr->Add( ptr->m_label );

	return ptr;
}

sf::Drawable* Button::InvalidateImpl() {
	m_label->SetProperty<sf::Color>( "Label.TextColor", Context::Get().GetEngine().GetProperty<sf::Color>( "Button.Normal.TextColor", shared_from_this() ) );

	return Context::Get().GetEngine().CreateButtonDrawable( boost::shared_dynamic_cast<Button>( shared_from_this() ) );

}

void Button::SetLabel( const sf::String& label ) {
	m_label->SetText( label );
	RequestSize();
	Invalidate();
}

const sf::String& Button::GetLabel() const {
	return m_label->GetText();
}

void Button::HandleStateChange( Widget::Ptr /*widget*/, State /*oldstate*/ ) {
	Invalidate();
}

void Button::HandleMouseEnter( Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Prelight );
	}
}

void Button::HandleMouseLeave( Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Normal );
	}
}

bool Button::HandleMouseButtonClick( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button button ) {
	if( button == sf::Mouse::Left ) {
		OnClick.Sig( shared_from_this() );
	}

	return true;
}

bool Button::HandleMouseButtonPress( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button button ) {
	if( button == sf::Mouse::Left ) {
		SetState( Active );
		return true;
	}

	return false;
}

bool Button::HandleMouseButtonRelease( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button /*button*/ ) {
	SetState( IsMouseInWidget() ? Prelight : Normal );
	return false;
}

void Button::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& /*old_allocation*/ ) {
	sf::FloatRect  label_allocation(
		GetAllocation().Left + GetBorderWidth() + m_padding,
		GetAllocation().Top + GetBorderWidth() + m_padding,
		GetAllocation().Width - 2 * GetBorderWidth() - 2 * m_padding,
		GetAllocation().Height - 2 * GetBorderWidth() - 2 * m_padding
	);

	m_label->AllocateSize( label_allocation );
}

sf::Vector2f Button::GetRequisitionImpl() const {
	sf::Vector2f  requisition( m_label->GetRequisition() );

	requisition.x += 2 * GetBorderWidth() + 2 * m_padding;
	requisition.y += 2 * GetBorderWidth() + 2 * m_padding;

	return requisition;
}

void Button::SetPadding( float padding ) {
	m_padding = padding;
	RequestSize();
	Invalidate();
}

float Button::GetPadding() const {
	return m_padding;
}

}
