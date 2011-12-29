#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Spinner.hpp>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSpinnerDrawable( SharedPtr<const Spinner> spinner ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", spinner ) );
	float steps( GetProperty<float>( "Steps", spinner ) );
	unsigned int stopped_alpha( GetProperty<unsigned int>( "StoppedAlpha", spinner ) );
	float radius = std::min( spinner->GetAllocation().Width, spinner->GetAllocation().Height ) / 2.f;

	// Make sure steps is sane.
	steps = std::max( steps, 3.f );

	// SFML does this too, for compatibility reasons, so lay off the flame :P
	static const float two_pi = 3.141592654f * 2.f;

	RenderQueue* queue( new RenderQueue );

	sf::VertexArray* array = new sf::VertexArray( sf::Triangles, static_cast<unsigned int>( steps ) * 3 );

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
		sf::Vector2f point1(
			static_cast<float>( cos( two_pi * index / steps ) ) * radius,
			static_cast<float>( sin( two_pi * index / steps ) ) * radius
		);

		sf::Vector2f point2(
			static_cast<float>( cos( two_pi * ( index + .5f ) / steps ) ) * radius,
			static_cast<float>( sin( two_pi * ( index + .5f ) / steps ) ) * radius
		);

		(*array)[static_cast<unsigned int>( index ) * 3 + 0].Position = sf::Vector2f( 0.f, 0.f ) + center_offset;
		(*array)[static_cast<unsigned int>( index ) * 3 + 1].Position = point1 + center_offset;
		(*array)[static_cast<unsigned int>( index ) * 3 + 2].Position = point2 + center_offset;

		(*array)[static_cast<unsigned int>( index ) * 3 + 0].Color = rod_color;
		(*array)[static_cast<unsigned int>( index ) * 3 + 1].Color = rod_color;
		(*array)[static_cast<unsigned int>( index ) * 3 + 2].Color = rod_color;
	}

	queue->Add( array );

	return queue;
}

}
}
