#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Spinner.hpp>

#include <SFML/Graphics/ConvexShape.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateSpinnerDrawable( SharedPtr<const Spinner> spinner ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", spinner ) );
	float steps( GetProperty<float>( "Steps", spinner ) );
	float fade( GetProperty<float>( "Fade", spinner ) );
	unsigned int stopped_alpha( GetProperty<unsigned int>( "StoppedAlpha", spinner ) );
	float radius = std::min( spinner->GetAllocation().Width, spinner->GetAllocation().Height ) / 2.f;

	// Make sure steps is sane.
	steps = std::max( steps, 3.f );

	// SFML does this too, for compatibility reasons, so lay off the flame :P
	static const float two_pi = 3.141592654f * 2.f;

	// SFML doesn't allow to change vertex colors anymore, so create one vertex
	// array per polygon.........
	RenderQueue* queue( new RenderQueue );

	for( float index = 0.f; index < steps; index += 1.f ) {
		sf::Color rod_color( color );

		if( spinner->Started() ) {
			rod_color.a = static_cast<sf::Uint8>( 255.f * pow( index / ( steps - 1.f ), fade ) );
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

		sf::ConvexShape* shape = new sf::ConvexShape( 3 );
		shape->SetPoint( 0, sf::Vector2f( 0.f, 0.f ) );
		shape->SetPoint( 1, point1 );
		shape->SetPoint( 2, point2 );

		shape->SetPosition( spinner->GetAllocation().Width / 2.f, spinner->GetAllocation().Height / 2.f );
		shape->SetOutlineColor( sf::Color::Transparent );
		shape->SetFillColor( rod_color );

		queue->Add( shape );
	}

	return queue;
}

}
}
