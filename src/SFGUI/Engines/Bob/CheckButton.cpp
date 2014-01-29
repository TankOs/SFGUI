#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/CheckButton.hpp>

#include <SFML/Graphics/Text.hpp>
#include <cmath>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateCheckButtonDrawable( std::shared_ptr<const CheckButton> check ) const {
	unsigned int font_size(GetProperty<unsigned int>("FontSize", check));
	const sf::Font& font(*GetResourceManager().GetFont(GetProperty<std::string>("FontName", check)));
	float box_size( GetProperty<float>( "BoxSize", check ) );
	float spacing( GetProperty<float>( "Spacing", check ) );
	const sf::Color font_color( GetProperty<sf::Color>( "Color", check ) );

	float vertical_offset = std::floor( ( check->GetAllocation().height - box_size ) / 2.f + 0.5f );

	std::unique_ptr<RenderQueue>  queue( new RenderQueue );

	// Box
	queue->Add( CreateSpritebox( sf::FloatRect( 0.f, vertical_offset, box_size, box_size ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "Image", check ) ),
								 GetProperty<UintRect>( "SubRect", check ) ) );
	// Check


	if( check->IsActive() ) {
		auto check_image = GetResourceManager().GetImage( GetProperty<std::string>( "CheckImage", check ) );
		auto check_texture = m_texture_manager.GetTexture( check_image );
		sf::Vector2f check_image_size( check_image->getSize() );

		queue->Add( Renderer::Get().CreateSprite(
						sf::FloatRect( std::floor( ( box_size - check_image_size.x ) / 2.f + 0.5f ),
									   std::floor( ( box_size - check_image_size.y ) / 2.f + 0.5f ) +  vertical_offset,
									   check_image_size.x,
									   check_image_size.y ),
						check_texture ) );
	}

	// Label
	if( check->GetLabel().getSize() > 0 ) {
		sf::Text text( check->GetLabel(), font, font_size );

		sf::Vector2f metrics = GetTextMetrics( check->GetLabel(), font, font_size );
		metrics.y = GetFontLineHeight( font, font_size );
		text.setPosition( box_size + spacing,
						  check->GetAllocation().height / 2.f - metrics.y / 2.f );

		text.setColor( font_color );

		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
