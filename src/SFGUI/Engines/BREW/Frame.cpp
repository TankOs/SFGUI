#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Frame.hpp>

#include <SFML/Graphics/Text.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateFrameDrawable( SharedPtr<const Frame> frame ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", frame ) );
	sf::Color color( GetProperty<sf::Color>( "Color", frame ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", frame ) );
	float border_width( GetProperty<float>( "BorderWidth", frame ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", frame ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", frame ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
	float label_padding( GetProperty<float>( "LabelPadding", frame ) );

	float line_height = GetLineHeight( font, font_size );

	RenderQueue* queue( new RenderQueue );

	// Right
	queue->Add(
		Context::Get().GetRenderer().CreateLine(
			sf::Vector2f( frame->GetAllocation().Width - border_width, line_height / 2.f ),
			sf::Vector2f( frame->GetAllocation().Width - border_width, frame->GetAllocation().Height - border_width ),
			border_color,
			border_width
		)
	);

	// Bottom
	queue->Add(
		Context::Get().GetRenderer().CreateLine(
			sf::Vector2f( frame->GetAllocation().Width, frame->GetAllocation().Height - border_width ),
			sf::Vector2f( 0.f, frame->GetAllocation().Height - border_width ),
			border_color,
			border_width
		)
	);

	// Left
	queue->Add(
		Context::Get().GetRenderer().CreateLine(
			sf::Vector2f( 0.f, frame->GetAllocation().Height - border_width ),
			sf::Vector2f( 0.f, line_height / 2.f ),
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

		sf::Text text( frame->GetLabel(), font, font_size );
		text.SetPosition( label_start_x + label_padding, .0f );
		text.SetColor( color );
		queue->Add( Context::Get().GetRenderer().CreateText( text, background_color ) );
	}

	// Top Left
	queue->Add(
		Context::Get().GetRenderer().CreateLine(
			sf::Vector2f( 0.f, line_height / 2.f ),
			sf::Vector2f( label_start_x, line_height / 2.f ),
			border_color,
			border_width
		)
	);

	// Top Right
	queue->Add(
		Context::Get().GetRenderer().CreateLine(
			sf::Vector2f( label_end_x, line_height / 2.f ),
			sf::Vector2f( frame->GetAllocation().Width - border_width, line_height / 2.f ),
			border_color,
			border_width
		)
	);

	return queue;
}

}
}
