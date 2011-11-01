#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Button::Button() :
	Bin(),
	m_label( Label::Create( L"" ) ),
	m_padding( 5.f )
{
	//OnMouseEnter.Connect( &Button::HandleMouseEnter, this );
	//OnMouseLeave.Connect( &Button::HandleMouseLeave, this );
	//OnMouseButtonClick.Connect( &Button::HandleMouseButtonClick, this );
	//OnMouseButtonPress.Connect( &Button::HandleMouseButtonPress, this );
	//OnMouseButtonRelease.Connect( &Button::HandleMouseButtonRelease, this );
	//OnSizeAllocate.Connect( &Button::HandleSizeAllocate, this );
}

Button::Ptr Button::Create( const sf::String& label ) {
	Button::Ptr  ptr( new Button );

	ptr->SetLabel( label );
	ptr->Add( ptr->m_label );

	return ptr;
}

sf::Drawable* Button::InvalidateImpl() {
	m_label->Invalidate();

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

bool Button::HandleMouseMoveEvent( int /*x*/, int /*y*/ ) {
	if( IsMouseInWidget() ) {
		if( GetState() == Normal ) {
			SetState( Prelight );
		}
	}
	else {
		if( GetState() == Prelight ) {
			SetState( Normal );
		}
	}
	
	// The event is processed, but we don't want to stop processing it further,
	// i.e. in other hooks registered.
	return false;
}

bool Button::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( !IsMouseInWidget() ) {
		SetState( Normal );
		return false;
	}

	if( button == sf::Mouse::Left ) {
		if( press ) {
			SetState( Active );
		}
		else {
			SetState( Prelight );
		}
	}

	return true;
}

bool Button::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) {
	sf::FloatRect  label_allocation(
		GetBorderWidth() + m_padding,
		GetBorderWidth() + m_padding,
		GetAllocation().Width - 2 * GetBorderWidth() - 2 * m_padding,
		GetAllocation().Height - 2 * GetBorderWidth() - 2 * m_padding
	);

	m_label->AllocateSize( label_allocation );

	return true;
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

const std::string& Button::GetName() const {
	static const std::string name( "Button" );
	return name;
}

}
