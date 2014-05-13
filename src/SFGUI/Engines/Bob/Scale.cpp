#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scale.hpp>

#include <cmath>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateScaleDrawable( std::shared_ptr<const Scale> scale ) const {
	float trough_thickness( GetProperty<float>( "TroughWidth", scale ) );
	sf::Vector2f trough_position;
	sf::Vector2f trough_dimension;
	sf::FloatRect slider_rect( scale->GetSliderRect() );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	Scale::Orientation orientation = scale->GetOrientation();
	if( orientation == Scale::Orientation::HORIZONTAL ) {
		trough_position = sf::Vector2f( std::floor( slider_rect.width / 2.f + 0.5f ), std::floor( ( scale->GetAllocation().height - trough_thickness ) / 2.f + 0.5f ) );
		trough_dimension = sf::Vector2f( std::ceil( scale->GetAllocation().width - slider_rect.width + 0.5f ), trough_thickness );
	}
	else {
		trough_position = sf::Vector2f( std::floor( ( scale->GetAllocation().width - trough_thickness ) / 2.f + 0.5f ), std::floor( slider_rect.height / 2.f + 0.5f ) );
		trough_dimension = sf::Vector2f( trough_thickness, std::ceil( scale->GetAllocation().height - slider_rect.height + 0.5f ) );
	}

	// Trough
	queue->Add( CreateSpritebox( sf::FloatRect( trough_position, trough_dimension ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "TroughImage", scale ) ),
								 GetProperty<UintRect>( "TroughSubRect", scale ) ) );

	// Slider
	queue->Add( CreateSpritebox( sf::FloatRect( std::round( slider_rect.left ),  std::round( slider_rect.top ),
												std::round( slider_rect.width ), std::round( slider_rect.height ) ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "SliderImage", scale ) ),
								 GetProperty<UintRect>( "SliderSubRect", scale ) ) );

	return queue;
}

}
}
