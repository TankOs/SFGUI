#include <SFGUI/Engines/Bob.hpp>

#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scrollbar.hpp>

#include <cmath>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateScrollbarDrawable( std::shared_ptr<const Scrollbar> scrollbar ) const {
	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	Scrollbar::Orientation orientation = scrollbar->GetOrientation();

	sf::FloatRect slider_rect(scrollbar->GetSliderRect());

	sf::Vector2f stepper_dimension;
	sf::Vector2f trough_dimension;

	sf::Vector2f increase_position;
	sf::Vector2f decrease_position;
	sf::Vector2f trough_position;

	unsigned int arrow_decrease_rotation;
	unsigned int arrow_increase_rotation;

	if( orientation == Scrollbar::Orientation::HORIZONTAL ) {
		stepper_dimension = sf::Vector2f( scrollbar->GetAllocation().height, scrollbar->GetAllocation().height );
		trough_dimension  = sf::Vector2f( scrollbar->GetAllocation().width - 2 * stepper_dimension.x, scrollbar->GetAllocation().height );

		decrease_position = sf::Vector2f( 0, 0 );
		increase_position = sf::Vector2f( scrollbar->GetAllocation().width - stepper_dimension.x, 0 );
		trough_position   = sf::Vector2f( stepper_dimension.x, 0 );

		arrow_decrease_rotation = 3;
		arrow_increase_rotation = 1;
	}
	else {
		stepper_dimension = sf::Vector2f( scrollbar->GetAllocation().width, scrollbar->GetAllocation().width );
		trough_dimension  = sf::Vector2f( scrollbar->GetAllocation().width, scrollbar->GetAllocation().height - 2 * stepper_dimension.y );

		decrease_position = sf::Vector2f( 0, 0 );
		increase_position = sf::Vector2f( 0, scrollbar->GetAllocation().height - stepper_dimension.y );
		trough_position   = sf::Vector2f( 0, stepper_dimension.y );

		arrow_decrease_rotation = 0;
		arrow_increase_rotation = 2;
	}

	// Trough
	queue->Add( CreateSpritebox( sf::FloatRect( trough_position, trough_dimension ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "TroughImage", scrollbar ) ),
								 GetProperty<UintRect>( "TroughSubRect", scrollbar ) ) );

	// Decrease-stepper
	std::string stepper_image_property("StepperImage");
	if(scrollbar->IsDecreaseStepperPressed()){
		stepper_image_property = "StepperPressedImage";
	}

	queue->Add( CreateSpritebox( sf::FloatRect( decrease_position, stepper_dimension ),
								 GetResourceManager().GetImage( GetProperty<std::string>( stepper_image_property, scrollbar ) ),
								 GetProperty<UintRect>( "StepperSubRect", scrollbar ) ) );

	// Increase-stepper
	stepper_image_property = "StepperImage";
	if(scrollbar->IsDecreaseStepperPressed()){
		stepper_image_property = "StepperPressedImage";
	}

	queue->Add( CreateSpritebox( sf::FloatRect( increase_position, stepper_dimension ),
								 GetResourceManager().GetImage( GetProperty<std::string>( stepper_image_property, scrollbar ) ),
								 GetProperty<UintRect>( "StepperSubRect", scrollbar ) ) );

	// Arrows
	std::shared_ptr<const sf::Image> arrow_image( GetResourceManager().GetImage( GetProperty<std::string>( "ArrowImage", scrollbar ) ) );
	if( arrow_image ){
		sf::Vector2f arrow_size;
		if( orientation == Scrollbar::Orientation::HORIZONTAL ) {
			arrow_size = sf::Vector2f( static_cast<float>( arrow_image->getSize().y ), static_cast<float>( arrow_image->getSize().x ) );
		}
		else {
			arrow_size = sf::Vector2f( static_cast<float>( arrow_image->getSize().x ), static_cast<float>( arrow_image->getSize().y ) );
		}

		// Decrease-arrow
		queue->Add( CreateArrow( sf::FloatRect( std::floor( decrease_position.x + ( stepper_dimension.x - arrow_size.x ) / 2.f + 0.5f ),
												std::floor( decrease_position.y + ( stepper_dimension.y - arrow_size.y ) / 2.f + 0.5f ),
												std::floor( arrow_size.x + 0.5f),
												std::floor( arrow_size.y + 0.5f) ),
								  arrow_decrease_rotation, arrow_image) );

		// Increase-arrow
		queue->Add( CreateArrow( sf::FloatRect( std::floor( increase_position.x + ( stepper_dimension.x - arrow_size.x ) / 2.f + 0.5f ),
												std::floor( increase_position.y + ( stepper_dimension.y - arrow_size.y ) / 2.f + 0.5f ),
												std::floor( arrow_size.x + 0.5f),
												std::floor( arrow_size.y + 0.5f) ),
								  arrow_increase_rotation, arrow_image) );
	}

	// Slider
	queue->Add( CreateSpritebox( slider_rect,
								 GetResourceManager().GetImage( GetProperty<std::string>( "SliderImage", scrollbar ) ),
								 GetProperty<UintRect>( "SliderSubRect", scrollbar ) ) );

	return queue;
}

}
}
