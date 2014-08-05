#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Spinner.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <cmath>
#include <algorithm>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateSpinnerDrawable( std::shared_ptr<const Spinner> spinner ) const {
	auto color = GetProperty<sf::Color>( "Color", spinner );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", spinner );
	auto steps = GetProperty<unsigned int>( "Steps", spinner );
	auto inner_radius = GetProperty<float>( "InnerRadius", spinner );
	auto rod_thickness = GetProperty<float>( "RodThickness", spinner );
	auto stopped_alpha = GetProperty<unsigned int>( "StoppedAlpha", spinner );
	auto radius = std::min( spinner->GetAllocation().width, spinner->GetAllocation().height ) / 2.f;

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Make sure steps is sane.
	steps = std::max( steps, 3u );

	// SFML does this too, for compatibility reasons, so lay off the flame :P
	static const auto two_pi = 3.141592654f * 2.f;

	sf::Vector2f center_offset( spinner->GetAllocation().width / 2.f, spinner->GetAllocation().height / 2.f );

	// We just have to produce the spinner in stopped state.
	// The class itself will take care of the started state.
	auto blend = ( 255.f - static_cast<float>( stopped_alpha ) ) / 255.f;

	sf::Color stop_color(
		static_cast<sf::Uint8>( static_cast<float>( color.r ) * ( 1.f - blend ) + static_cast<float>( background_color.r ) * blend ),
		static_cast<sf::Uint8>( static_cast<float>( color.g ) * ( 1.f - blend ) + static_cast<float>( background_color.g ) * blend ),
		static_cast<sf::Uint8>( static_cast<float>( color.b ) * ( 1.f - blend ) + static_cast<float>( background_color.b ) * blend )
	);

	auto started = spinner->Started();
	auto current_stage = spinner->GetStage();

	for( unsigned int index = 0; index < steps; index++ ) {
		// Time for some hardcore trigonometry...
		sf::Vector2f inner_point(
			std::cos( two_pi * static_cast<float>( index ) / -static_cast<float>( steps ) ) * inner_radius,
			std::sin( two_pi * static_cast<float>( index ) / -static_cast<float>( steps ) ) * inner_radius
		);

		sf::Vector2f outer_point(
			std::cos( two_pi * static_cast<float>( index ) / -static_cast<float>( steps ) ) * radius,
			std::sin( two_pi * static_cast<float>( index ) / -static_cast<float>( steps ) ) * radius
		);

		unsigned int rod_stage = ( current_stage + index ) % steps;

		auto rod_alpha = static_cast<float>( rod_stage ) / ( static_cast<float>( steps ) - 1.f );

		sf::Color rod_color(
			static_cast<sf::Uint8>( static_cast<float>( color.r ) * ( 1.f - rod_alpha ) + static_cast<float>( background_color.r ) * rod_alpha ),
			static_cast<sf::Uint8>( static_cast<float>( color.g ) * ( 1.f - rod_alpha ) + static_cast<float>( background_color.g ) * rod_alpha ),
			static_cast<sf::Uint8>( static_cast<float>( color.b ) * ( 1.f - rod_alpha ) + static_cast<float>( background_color.b ) * rod_alpha )
		);

		queue->Add(
			Renderer::Get().CreateLine(
				inner_point + center_offset,
				outer_point + center_offset,
				started ? rod_color : stop_color,
				rod_thickness
			)
		);
	}

	return queue;
}

}
}
