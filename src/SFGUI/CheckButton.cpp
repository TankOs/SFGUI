#include <SFGUI/CheckButton.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {

CheckButton::Ptr CheckButton::Create( const sf::String& label ) {
	auto widget = Ptr( new CheckButton );
	widget->SetLabel( label );
	return widget;
}

const std::string& CheckButton::GetName() const {
	static const std::string name( "CheckButton" );
	return name;
}

std::unique_ptr<RenderQueue> CheckButton::InvalidateImpl() const {
	if( GetChild() ) {
		GetChild()->Invalidate();
	}

	return Context::Get().GetEngine().CreateCheckButtonDrawable( std::dynamic_pointer_cast<const CheckButton>( shared_from_this() ) );
}

sf::Vector2f CheckButton::CalculateRequisition() {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	float box_size( Context::Get().GetEngine().GetProperty<float>( "BoxSize", shared_from_this() ) );
	sf::Vector2f requisition( box_size, box_size );

	if( GetLabel().getSize() > 0 ) {
		auto metrics = Context::Get().GetEngine().GetTextStringMetrics( GetLabel(), font, font_size );
		requisition.x += metrics.x + spacing;
		requisition.y = std::max( requisition.y, Context::Get().GetEngine().GetFontLineHeight( font, font_size ) );
	}

	return requisition;
}

void CheckButton::HandleSizeChange() {
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	float box_size( Context::Get().GetEngine().GetProperty<float>( "BoxSize", shared_from_this() ) );

	if( GetChild() ) {
		GetChild()->SetAllocation(
			sf::FloatRect(
				box_size + spacing,
				GetAllocation().height / 2.f - GetChild()->GetAllocation().height / 2.f,
				GetChild()->GetRequisition().x,
				GetChild()->GetRequisition().y
			)
		);
	}
}


}
