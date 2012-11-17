#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Engines/Bob/Spritebox.hpp>

#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scrollbar.hpp>

#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateScrollbarDrawable( SharedPtr<const Scrollbar> scrollbar ) const {
	RenderQueue* queue( new RenderQueue );

	Scrollbar::Orientation orientation = scrollbar->GetOrientation();

	sf::FloatRect slider_rect(scrollbar->GetSliderRect());

	sf::Vector2f stepper_dimension;
	sf::Vector2f trough_dimension;

	sf::Vector2f increase_position;
	sf::Vector2f decrease_position;
	sf::Vector2f trough_position;

	unsigned int arrow_decrease_rotation;
	unsigned int arrow_increase_rotation;

	if( orientation == Scrollbar::HORIZONTAL ) {
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
	const sf::Image *trough_image( GetResourceManager().GetImage(GetProperty<std::string>( "TroughImage", scrollbar ) ) );
	if(trough_image != NULL){
		bob::Spritebox spritebox;
		spritebox.SetDimension( sf::Vector2i( static_cast<sf::Vector2i>( trough_dimension ) ) );
		spritebox.SetPosition( trough_position );

		SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( trough_image ) );
		spritebox.SetTexture( texture_handle );

		Primitive::Ptr primitive = spritebox.ConstructPrimitive();

		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	// Steppers
	const sf::Image *stepper_image;

	// Decrease
	if(scrollbar->IsDecreaseStepperPressed()){
		stepper_image = GetResourceManager().GetImage( GetProperty<std::string>( "StepperPressedImage", scrollbar ) );
	} else {
		stepper_image = GetResourceManager().GetImage( GetProperty<std::string>( "StepperImage", scrollbar ) );
	}

	if(stepper_image != NULL){
		bob::Spritebox spritebox;
		spritebox.SetDimension( static_cast<sf::Vector2i>( stepper_dimension ) );
		spritebox.SetPosition( decrease_position );

		SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( stepper_image ) );
		spritebox.SetTexture( texture_handle );

		Primitive::Ptr primitive = spritebox.ConstructPrimitive();

		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	// Increase
	if(scrollbar->IsIncreaseStepperPressed()){
		stepper_image = GetResourceManager().GetImage( GetProperty<std::string>( "StepperPressedImage", scrollbar ) );
	} else {
		stepper_image = GetResourceManager().GetImage( GetProperty<std::string>( "StepperImage", scrollbar ) );
	}

	if(stepper_image != NULL){
		bob::Spritebox spritebox;
		spritebox.SetDimension( static_cast<sf::Vector2i>( stepper_dimension ) );
		spritebox.SetPosition( increase_position );

		SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( stepper_image ) );
		spritebox.SetTexture( texture_handle );

		Primitive::Ptr primitive = spritebox.ConstructPrimitive();

		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	// Arrows
	const sf::Image *arrow_image( GetResourceManager().GetImage( GetProperty<std::string>( "ArrowImage", scrollbar ) ) );
	if(arrow_image != NULL){
		sf::Vector2f arrow_size;
		if( orientation == Scrollbar::HORIZONTAL ) {
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
	const sf::Image *slider_image(GetResourceManager().GetImage(GetProperty<std::string>("SliderImage", scrollbar)));
	if(stepper_image != NULL){
		bob::Spritebox spritebox;
		spritebox.SetDimension( sf::Vector2i( static_cast<int>(slider_rect.width),  static_cast<int>(slider_rect.height) ) );
		spritebox.SetPosition(  sf::Vector2f( slider_rect.left, slider_rect.top ) );

		SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( slider_image ) );
		spritebox.SetTexture( texture_handle );

		Primitive::Ptr primitive = spritebox.ConstructPrimitive();

		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	return queue;
}

Primitive::Ptr Bob::CreateArrow( const sf::FloatRect& rect, unsigned int rotation, const sf::Image *image ) const {
	SharedPtr<Primitive::Texture> texture_handle( m_texture_manager.GetTexture( image ) );
	sf::Vector2f offset = texture_handle->offset;

	Primitive::Ptr primitive( new Primitive );

	Primitive::Vertex v0, v1, v2, v3;

	v0.position = sf::Vector2f( rect.left, rect.top );
	v1.position = sf::Vector2f( rect.left, rect.top +  rect.height );
	v2.position = sf::Vector2f( rect.left  + rect.width, rect.top + rect.height );
	v3.position = sf::Vector2f( rect.left  + rect.width, rect.top );

	v0.color = sf::Color::White;
	v1.color = sf::Color::White;
	v2.color = sf::Color::White;
	v3.color = sf::Color::White;

	sf::Vector2f texture_coordinates[4] = {
		sf::Vector2f( 0.f, 0.f ),
		sf::Vector2f( 0.f, static_cast<float>( image->getSize().y ) ),
		sf::Vector2f( static_cast<float>( image->getSize().x ), static_cast<float>( image->getSize().y ) ),
		sf::Vector2f( static_cast<float>( image->getSize().x ), 0.f )
	};

	v0.texture_coordinate = offset + texture_coordinates[ rotation % 4 ];
	v1.texture_coordinate = offset + texture_coordinates[ ( rotation + 1 ) % 4 ];
	v2.texture_coordinate = offset + texture_coordinates[ ( rotation + 2 ) % 4 ];
	v3.texture_coordinate = offset + texture_coordinates[ ( rotation + 3 ) % 4 ];

	primitive->AddVertex( v0 );
	primitive->AddVertex( v1 );
	primitive->AddVertex( v3 );
	primitive->AddVertex( v3 );
	primitive->AddVertex( v1 );
	primitive->AddVertex( v2 );

	primitive->AddTexture( texture_handle );

	Renderer::Get().AddPrimitive( primitive );

	return primitive;
}

}
}
