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

RenderQueue* CheckButton::InvalidateImpl() const {
	if( GetChild() ) {
		GetChild()->Invalidate();
	}

	return Context::Get().GetEngine().CreateCheckButtonDrawable( std::dynamic_pointer_cast<const CheckButton>( shared_from_this() ) );
}

sf::Vector2f CheckButton::GetRequisitionImpl() const {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	float box_size( Context::Get().GetEngine().GetProperty<float>( "BoxSize", shared_from_this() ) );
	sf::Vector2f requisition( box_size, box_size );

	if( GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = Context::Get().GetEngine().GetTextMetrics( GetLabel(), font, font_size );
		requisition.x = std::max( requisition.x, metrics.x );
		requisition.y = std::max( requisition.y, Context::Get().GetEngine().GetLineHeight( font, font_size ) );

		requisition.x += spacing + 2.f * padding;
		requisition.y += 2.f * padding;
	}

	return requisition;
}

void CheckButton::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) const {
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
