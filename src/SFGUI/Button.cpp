#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Button::Button() :
	Bin(),
	m_label( Label::Create( L"" ) )
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
	m_label->Invalidate();

	return Context::Get().GetEngine().CreateButtonDrawable( std::dynamic_pointer_cast<const Button>( shared_from_this() ) );

}

void Button::SetLabel( const sf::String& label ) {
	if( !GetChild() ) {
		Add( m_label );
	}

	m_label->SetText( label );
	RequestSize();
	Invalidate();
}

const sf::String& Button::GetLabel() const {
	return m_label->GetText();
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

void Button::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) const {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );

	sf::FloatRect  label_allocation(
		GetBorderWidth() + padding,
		GetBorderWidth() + padding,
		GetAllocation().Width - 2 * GetBorderWidth() - 2 * padding,
		GetAllocation().Height - 2 * GetBorderWidth() - 2 * padding
	);

	m_label->AllocateSize( label_allocation );
}

void Button::HandleStateChange( State old_state ) {
	m_label->RequestSize();

	Widget::HandleStateChange( old_state );
}

sf::Vector2f Button::GetRequisitionImpl() const {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	sf::Vector2f requisition( m_label->GetRequisition() );

	requisition.x += 2 * GetBorderWidth() + 2 * padding;
	requisition.y += 2 * GetBorderWidth() + 2 * padding;

	return requisition;
}

const std::string& Button::GetName() const {
	static const std::string name( "Button" );
	return name;
}

void Button::HandleMouseClick( int /*x*/, int /*y*/ ) {
	OnClick();
}

}
