#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Engines/Bob/Spritebox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scale.hpp>

#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateScaleDrawable( SharedPtr<const Scale> scale ) const {
	float trough_thickness( GetProperty<float>( "TroughWidth", scale ) );
	sf::Vector2f trough_position;
	sf::Vector2f trough_dimension;
	sf::FloatRect slider_rect(scale->GetSliderRect());

	RenderQueue* queue( new RenderQueue );

	Scale::Orientation orientation = scale->GetOrientation();
	if( orientation == Scale::HORIZONTAL ) {
		trough_position = sf::Vector2f( std::floor( slider_rect.width / 2.f + 0.5f ), std::floor( ( scale->GetAllocation().height - trough_thickness ) / 2.f + 0.5f ) );
		trough_dimension = sf::Vector2f( std::ceil( scale->GetAllocation().width - slider_rect.width + 0.5f ), trough_thickness );
	}
	else {
		trough_position = sf::Vector2f( std::floor( ( scale->GetAllocation().width - trough_thickness ) / 2.f + 0.5f ), std::floor( slider_rect.height / 2.f + 0.5f ) );
		trough_dimension = sf::Vector2f( trough_thickness, std::ceil( scale->GetAllocation().height - slider_rect.height + 0.5f ) );
	}

	// Trough
	const sf::Image *trough_image( GetResourceManager().GetImage( GetProperty<std::string>( "TroughImage", scale ) ) );
	if( trough_image != NULL ){
		bob::Spritebox spritebox;
		spritebox.SetDimension( static_cast<sf::Vector2i>( trough_dimension ) );
		spritebox.SetPosition( trough_position );
		spritebox.SetTexture( m_texture_manager.GetTexture( trough_image ) );

		Primitive::Ptr primitive = spritebox.ConstructPrimitive();
		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	// Slider
	const sf::Image *slider_image( GetResourceManager().GetImage( GetProperty<std::string>( "SliderImage", scale ) ) );
	if(slider_image != NULL){
		bob::Spritebox spritebox;
		spritebox.SetDimension( sf::Vector2i( static_cast<int>( slider_rect.width ), static_cast<int>( slider_rect.height ) ) );
		spritebox.SetPosition( sf::Vector2f( std::floor( slider_rect.left + 0.5f ), std::floor( slider_rect.top + 0.5f ) ) );
		spritebox.SetTexture( m_texture_manager.GetTexture( slider_image ) );

		Primitive::Ptr primitive = spritebox.ConstructPrimitive();
		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	return queue;
}

}
}
