#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Spinner.hpp>

#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSpinnerDrawable( SharedPtr<const Spinner> spinner ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", spinner ) );
	float steps( GetProperty<float>( "Steps", spinner ) );
	float inner_radius( GetProperty<float>( "InnerRadius", spinner ) );
	float rod_thickness( GetProperty<float>( "RodThickness", spinner ) );
	unsigned int stopped_alpha( GetProperty<unsigned int>( "StoppedAlpha", spinner ) );
	float radius = std::min( spinner->GetAllocation().Width, spinner->GetAllocation().Height ) / 2.f;

	// Make sure steps is sane.
	steps = std::max( steps, 3.f );

	// SFML does this too, for compatibility reasons, so lay off the flame :P
	static const float two_pi = 3.141592654f * 2.f;

	RenderQueue* queue( new RenderQueue );

	sf::Vector2f center_offset( spinner->GetAllocation().Width / 2.f, spinner->GetAllocation().Height / 2.f );

	for( float index = 0.f; index < steps; index += 1.f ) {
		sf::Color rod_color( color );

		if( spinner->Started() ) {
			rod_color.a = static_cast<sf::Uint8>( 255.f * index / ( steps - 1.f ) );
		}
		else {
			rod_color.a = static_cast<sf::Uint8>( stopped_alpha );
		}

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
			Context::Get().GetProjectO().CreateLine(
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
