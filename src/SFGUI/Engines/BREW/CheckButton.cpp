#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/CheckButton.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateCheckButtonDrawable( SharedPtr<const CheckButton> check ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", check ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", check ) );
	sf::Color color( GetProperty<sf::Color>( "Color", check ) );
	sf::Color check_color( GetProperty<sf::Color>( "CheckColor", check ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", check ) );
	float border_width( GetProperty<float>( "BorderWidth", check ) );
	float box_size( GetProperty<float>( "BoxSize", check ) );
	float spacing( GetProperty<float>( "Spacing", check ) );
	float check_size( std::min( box_size, GetProperty<float>( "CheckSize", check ) ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", check ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", check ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
	RenderQueue* queue( new RenderQueue );

	// Check Pane.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, check->GetAllocation().height / 2.f - box_size / 2.f ),
			sf::Vector2f( box_size, box_size ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

	if( check->IsActive() ) {
		float diff( box_size - check_size );

		queue->Add(
			Renderer::Get().CreateRect(
				sf::FloatRect(
					box_size / 2 - check_size / 2,
					check->GetAllocation().height / 2.f - box_size / 2.f + diff / 2.f,
					check_size - 1.f,
					check_size - 1.f
				),
				check_color
			)
		);
	}

	// Label.
	if( check->GetLabel().getSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( check->GetLabel(), font, font_size );
		metrics.y = GetFontLineHeight( font, font_size );

		sf::Text text( check->GetLabel(), font, font_size );
		text.setPosition(
			box_size + spacing,
			check->GetAllocation().height / 2.f - metrics.y / 2.f
		);
		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
