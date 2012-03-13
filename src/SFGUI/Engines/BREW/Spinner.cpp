#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Spinner.hpp>

#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSpinnerDrawable( SharedPtr<const Spinner> spinner ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", spinner ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", spinner ) );
	float steps( GetProperty<float>( "Steps", spinner ) );
	float inner_radius( GetProperty<float>( "InnerRadius", spinner ) );
	float rod_thickness( GetProperty<float>( "RodThickness", spinner ) );
	unsigned int stopped_alpha( GetProperty<unsigned int>( "StoppedAlpha", spinner ) );
	float radius = std::min( spinner->GetAllocation().width, spinner->GetAllocation().height ) / 2.f;

	// Make sure steps is sane.
	steps = std::max( steps, 3.f );

	// SFML does this too, for compatibility reasons, so lay off the flame :P
	static const float two_pi = 3.141592654f * 2.f;

	RenderQueue* queue( new RenderQueue );

	sf::Vector2f center_offset( spinner->GetAllocation().width / 2.f, spinner->GetAllocation().height / 2.f );

	// We just have to produce the spinner in stopped state.
	// The class itself will take care of the started state.
	float blend = ( 255.f - static_cast<float>( stopped_alpha ) ) / 255.f;

	sf::Color rod_color(
		static_cast<sf::Uint8>( static_cast<float>( color.r ) * ( 1.f - blend ) + static_cast<float>( background_color.r ) * blend ),
		static_cast<sf::Uint8>( static_cast<float>( color.g ) * ( 1.f - blend ) + static_cast<float>( background_color.g ) * blend ),
		static_cast<sf::Uint8>( static_cast<float>( color.b ) * ( 1.f - blend ) + static_cast<float>( background_color.b ) * blend )
	);

	for( float index = 0.f; index < steps; index += 1.f ) {
		// Time for some hardcore trigonometry...
		sf::Vector2f inner_point(
			static_cast<float>( cos( two_pi * index / steps ) ) * inner_radius,
			static_cast<float>( sin( two_pi * index / steps ) ) * inner_radius
		);

		sf::Vector2f outer_point(
			static_cast<float>( cos( two_pi * index / steps ) ) * radius,
			static_cast<float>( sin( two_pi * index / steps ) ) * radius
		);

		queue->Add(
			Renderer::Get().CreateLine(
				inner_point + center_offset,
				outer_point + center_offset,
				rod_color,
				rod_thickness
			)
		);
	}

	return queue;
}

}
}
