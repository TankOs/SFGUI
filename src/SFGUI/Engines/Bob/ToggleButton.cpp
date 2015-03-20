#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/ToggleButton.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateToggleButtonDrawable( std::shared_ptr<const ToggleButton> button ) const {
	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	std::string image_name( "Image" );
	if( ( button->GetState() == Button::State::ACTIVE ) || button->IsActive() ) {
		image_name = "ActiveImage";
	}

	queue->Add( CreateSpritebox( sf::FloatRect( 0.f, 0.f, button->GetAllocation().width, button->GetAllocation().height ),
								 GetResourceManager().GetImage( GetProperty<std::string>( image_name, button ) ),
								 GetProperty<UintRect>( "SubRect", button ) ) );

	//Label
	if( button->GetLabel().getSize() > 0 ) {
		const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
		unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
		sf::Color color( GetProperty<sf::Color>( "Color", button ) );
		float spacing( GetProperty<float>( "Spacing", button ) );

		const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
		sf::Vector2f metrics = GetTextStringMetrics( button->GetLabel(), font, font_size );
		metrics.y = GetFontLineHeight( font, font_size );

		sf::Text text( button->GetLabel(), font, font_size );
		Widget::PtrConst child( button->GetChild() );

		if( !child ) {
			text.setPosition(
				button->GetAllocation().width / 2.f - metrics.x / 2.f,
				button->GetAllocation().height / 2.f - metrics.y / 2.f
			);
		}
		else {
			float width( button->GetAllocation().width - spacing - child->GetAllocation().width );

			text.setPosition(
				child->GetAllocation().width + spacing + (width / 2.f - metrics.x / 2.f),
				button->GetAllocation().height / 2.f - metrics.y / 2.f
			);
		}

		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
