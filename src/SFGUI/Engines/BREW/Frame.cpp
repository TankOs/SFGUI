#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Frame.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateFrameDrawable( std::shared_ptr<const Frame> frame ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", frame );
	auto color = GetProperty<sf::Color>( "Color", frame );
	auto border_width = GetProperty<float>( "BorderWidth", frame );
	const auto& font_name = GetProperty<std::string>( "FontName", frame );
	auto font_size = GetProperty<unsigned int>( "FontSize", frame );
	const auto& font = GetResourceManager().GetFont( font_name );
	auto label_padding = GetProperty<float>( "LabelPadding", frame );
	auto line_height = GetFontLineHeight( *font, font_size );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Right
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( frame->GetAllocation().width - border_width / 2.f, line_height / 2.f + border_width / 2.f ),
			sf::Vector2f( frame->GetAllocation().width - border_width / 2.f, frame->GetAllocation().height - border_width ),
			border_color,
			border_width
		)
	);

	// Bottom
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( frame->GetAllocation().width - border_width / 2.f, frame->GetAllocation().height - border_width ),
			sf::Vector2f( border_width / 2.f, frame->GetAllocation().height - border_width ),
			border_color,
			border_width
		)
	);

	// Left
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( border_width / 2.f, frame->GetAllocation().height - border_width ),
			sf::Vector2f( border_width / 2.f, line_height / 2.f + border_width / 2.f ),
			border_color,
			border_width
		)
	);

	auto label_start_x = line_height;
	auto label_end_x = line_height;

	auto alignment = frame->GetAlignment().x;

	if( frame->GetLabel().getSize() > 0 ) {
		auto metrics = GetTextStringMetrics( frame->GetLabel(), *font, font_size );
		metrics.x += ( 2 * label_padding );

		label_start_x += ( alignment * ( frame->GetAllocation().width - 2 * line_height - metrics.x ) );
		label_end_x += ( metrics.x + alignment * ( frame->GetAllocation().width - 2 * line_height - metrics.x ) );

		sf::Text text( frame->GetLabel(), *font, font_size );
		text.setPosition( label_start_x + label_padding, border_width / 2.f );
		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	// Top Left
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( border_width / 2.f, line_height / 2.f + border_width / 2.f ),
			sf::Vector2f( label_start_x - .5f * border_width, line_height / 2.f + border_width / 2.f ),
			border_color,
			border_width
		)
	);

	// Top Right
	queue->Add(
		Renderer::Get().CreateLine(
			sf::Vector2f( label_end_x + .5f * border_width, line_height / 2.f + border_width / 2.f ),
			sf::Vector2f( frame->GetAllocation().width - border_width / 2.f, line_height / 2.f + border_width / 2.f ),
			border_color,
			border_width
		)
	);

	return queue;
}

}
}
