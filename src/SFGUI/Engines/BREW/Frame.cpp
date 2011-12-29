#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Frame.hpp>

#include <SFML/Graphics/Text.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateFrameDrawable( SharedPtr<const Frame> frame ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", frame ) );
	sf::Color color( GetProperty<sf::Color>( "Color", frame ) );
	float border_width( GetProperty<float>( "BorderWidth", frame ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", frame ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", frame ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
	float label_padding( GetProperty<float>( "LabelPadding", frame ) );

	float line_height = GetLineHeight( font, font_size );

	RenderQueue* queue( new RenderQueue );

	// Bottom
	queue->Add(
		CreateLine(
			BOTTOM,
			sf::Vector2f( .5f, std::floor( frame->GetAllocation().Height - border_width + .5f ) - .5f ),
			sf::Vector2f( frame->GetAllocation().Width - .5f, std::floor( frame->GetAllocation().Height - border_width + .5f ) - .5f ),
			border_color,
			border_width
		)
	);

	// Left
	queue->Add(
		CreateLine(
			LEFT,
			sf::Vector2f( .5f, std::floor( line_height / 2.f + .5f ) + .5f ),
			sf::Vector2f( .5f, std::floor( frame->GetAllocation().Height - border_width + .5f ) + .5f ),
			border_color,
			border_width
		)
	);

	// Right
	queue->Add(
		CreateLine(
			LEFT,
			sf::Vector2f( std::floor( frame->GetAllocation().Width - border_width + .5f ) - .5f, std::floor( line_height / 2.f + .5f ) + .5f ),
			sf::Vector2f( std::floor( frame->GetAllocation().Width - border_width + .5f ) - .5f, std::floor( frame->GetAllocation().Height - 2.f * border_width + .5f ) + .5f ),
			border_color,
			border_width
		)
	);

	float label_start_x = line_height;
	float label_end_x = line_height;

	float alignment = frame->GetAlignment().x;

	if( frame->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( frame->GetLabel(), font, font_size );
		metrics.x += ( 2 * label_padding );

		label_start_x += ( alignment * ( frame->GetAllocation().Width - 2 * line_height - metrics.x ) );
		label_end_x += ( metrics.x + alignment * ( frame->GetAllocation().Width - 2 * line_height - metrics.x ) );

		sf::Text* text( new sf::Text( frame->GetLabel(), font, font_size ) );
		text->SetPosition( std::floor( label_start_x + label_padding + .5f ), .0f );
		text->SetColor( color );
		queue->Add( text );
	}

	// Top Left
	queue->Add(
		CreateLine(
			TOP,
			sf::Vector2f( .5f, std::floor( line_height / 2.f + .5f ) + .5f ),
			sf::Vector2f( std::floor( label_start_x + .5f ) - .5f, std::floor( line_height / 2.f + .5f ) + .5f ),
			border_color,
			border_width
		)
	);

	// Top Right
	queue->Add(
		CreateLine(
			TOP,
			sf::Vector2f( std::floor( label_end_x + .5f ) + .5f, std::floor( line_height / 2.f + .5f ) + .5f ),
			sf::Vector2f( std::floor( frame->GetAllocation().Width + .5f ) - .5f, std::floor( line_height / 2.f + .5f ) + .5f ),
			border_color,
			border_width
		)
	);

	return queue;
}

}
}
