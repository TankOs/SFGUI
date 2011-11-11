#include <SFGUI/CheckButton.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

CheckButton::CheckButton() :
	ToggleButton()
{
}

CheckButton::~CheckButton() {
}

CheckButton::Ptr CheckButton::Create( const sf::String& label ) {
	Ptr widget( new CheckButton );

	widget->SetLabel( label );

	return widget;
}

const std::string& CheckButton::GetName() const {
	static const std::string name( "CheckButton" );
	return name;
}

sf::Drawable* CheckButton::InvalidateImpl() const {
	if( GetChild() ) {
		GetChild()->Invalidate();
	}

	return Context::Get().GetEngine().CreateCheckButtonDrawable( std::dynamic_pointer_cast<const CheckButton>( shared_from_this() ) );
}

sf::Vector2f CheckButton::GetRequisitionImpl() const {
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	float box_size( Context::Get().GetEngine().GetProperty<float>( "BoxSize", shared_from_this() ) );
	sf::Vector2f requisition( box_size, box_size );

	if( GetChild() ) {
		requisition.x = std::max(
			requisition.x,
			GetChild()->GetRequisition().x + spacing + box_size
		);
		requisition.y = std::max( requisition.y, GetChild()->GetRequisition().y );
	}

	return requisition;
}

void CheckButton::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) {
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	float box_size( Context::Get().GetEngine().GetProperty<float>( "BoxSize", shared_from_this() ) );

	if( GetChild() ) {
		GetChild()->AllocateSize(
			sf::FloatRect(
				box_size + spacing,
				GetAllocation().Height / 2.f - GetChild()->GetAllocation().Height / 2.f,
				GetChild()->GetRequisition().x,
				GetChild()->GetRequisition().y
			)
		);
	}
}


}
