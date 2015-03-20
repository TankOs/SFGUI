#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/PrimitiveVertex.hpp>

#include <cmath>

#include <SFML/Graphics/Transform.hpp>

namespace sfg {
namespace eng {

Bob::Bob() :
	Engine()
{
	ResetProperties();
}

void Bob::ResetProperties() {
	// Disable automatic widget refreshing.
	SetAutoRefresh( false );

	ClearProperties();

	// Set defaults.
	SetProperty( "*", "Color", sf::Color::Black );
	SetProperty( "*", "FontSize", 14 );
	SetProperty( "*", "FontName", "" ); // Uses SFGUI's default font when empty.
	SetProperty( "*", "BackgroundColor", sf::Color::White );
	SetProperty( "*", "Padding", 5.f );
	SetProperty( "*", "SubRect", UintRect( 0, 0, 0, 0 ) );

	// Button-specific.
	SetProperty( "Button", "Spacing", 5.f );

	// CheckButton-specific.
	SetProperty( "CheckButton", "Spacing", 5.f );
	SetProperty( "CheckButton", "BoxSize", 15.f );
	SetProperty( "CheckButton", "SubRect", UintRect( 0, 0, 0, 0 ) );
	SetProperty( "CheckButton", "CheckSubRect", UintRect( 0, 0, 0, 0 ) );

	// ComboBox-specific.
	SetProperty( "ComboBox", "ItemPadding", 4.f );
	SetProperty( "ComboBox", "ItemPaddingHorizontally", 4.f );

	// Entry-specific.
	SetProperty( "Entry", "Thickness", 1.f );

	// ProgressBar-specific.
	SetProperty( "ProgressBar", "BarPadding", 1.f);

	// Scale-specific.
	SetProperty( "Scale", "SliderLength", 15.f );
	SetProperty( "Scale", "SliderSubRect", UintRect( 0, 0, 0, 0 ) );
	SetProperty( "Scale", "TroughWidth", 5.f );
	SetProperty( "Scale", "TroughSubRect", UintRect( 0, 0, 0, 0 ) );

	// Scrollbar-specific.
	SetProperty( "Scrollbar", "StepperSpeed", 10.f );
	SetProperty( "Scrollbar", "StepperRepeatDelay", 300 );
	SetProperty( "Scrollbar", "StepperSubRect", UintRect( 0, 0, 0, 0 ) );
	SetProperty( "Scrollbar", "SliderMinimumLength", 15.f );
	SetProperty( "Scrollbar", "SliderSubRect", UintRect( 0, 0, 0, 0 ) );
	SetProperty( "Scrollbar", "TroughSubRect", UintRect( 0, 0, 0, 0 ) );

	// ScrolledWindow-specific.
	SetProperty( "ScrolledWindow", "ScrollbarWidth", 20.f );
	SetProperty( "ScrolledWindow", "ScrollbarSpacing", 5.f );
	SetProperty( "ScrolledWindow", "BorderWidth", 0.f );

	// Spinner-specific.
	SetProperty( "Spinner", "CycleDuration", 1000.f );
	SetProperty( "Spinner", "Steps", 60 );

	// ToggleButton-specific.
	SetProperty( "ToggleButton", "Spacing", 5.f );

	// Window-specific.
	SetProperty( "Window", "Gap", 10.f );
	SetProperty( "Window", "TitlePadding", 5.f );
	SetProperty( "Window", "WindowSubRect", UintRect( 0, 0, 0, 0 ) );
	SetProperty( "Window", "HandleSize", 10.f );

	// Don't forget to reactivate automatic widget refreshing.
	SetAutoRefresh( true );
}

Primitive::Ptr Bob::CreateSpritebox( const sf::FloatRect& rect, std::shared_ptr<const sf::Image> image, UintRect sub_rect, unsigned int horizontal, unsigned int vertical, int rotation_turns ) const
{
	Primitive::Ptr primitive( new Primitive( 6 * horizontal * vertical ) );

	if( !image )
		return primitive;

	PrimitiveTexture::Ptr texture( m_texture_manager.GetTexture( image ) );
	if( !texture )
		return primitive;

	// Get rotation_turns into the range [0;3].
	rotation_turns %= 4;

	sf::FloatRect float_sub_rect = static_cast<sf::FloatRect>( sub_rect );
	if( sub_rect == UintRect( 0, 0, 0, 0 ) ){
		float_sub_rect = sf::FloatRect( sf::Vector2f( 0, 0 ), static_cast<sf::Vector2f>( texture->size ) );
	}

	// Use SFML to rotate
	sf::Transform transform, inverse_transform;
	transform.rotate( static_cast<float>( rotation_turns * 90 ) );
	inverse_transform.rotate( static_cast<float>( -rotation_turns * 90 ) );

	sf::FloatRect transformed_rect( transform.transformRect( rect ) );

	sf::Vector2f dimension = sf::Vector2f( transformed_rect.width, transformed_rect.height );
	sf::Vector2f position  = sf::Vector2f( transformed_rect.left,  transformed_rect.top );

    if( dimension.x < 1 || dimension.y < 1 || texture == 0 )
        return primitive;

	// Calculate the coordinates of the spritebox depending on the number of divisions

    // Round for pixel perfect rendering
    sf::Vector2f border_step( std::floor( float_sub_rect.width   / static_cast<float>( horizontal ) + 0.5f),
                              std::floor( float_sub_rect.height  / static_cast<float>( vertical )   + 0.5f) );

	if (dimension.x < float_sub_rect.width ){
		border_step.x = dimension.x / static_cast<float>( horizontal ) ;
	}
	if( dimension.y < float_sub_rect.height ){
		border_step.y = dimension.y / static_cast<float>( vertical ) ;
	}

    std::vector<float> x_coords( horizontal + 1) , y_coords( vertical + 1 );
    x_coords[0] = 0.f;
    y_coords[0] = 0.f;

    // Make sure we don't cause a overflow for only one division in either direction
    if( horizontal > 1 ){
		x_coords[horizontal - 1] = dimension.x  - border_step.x;
    }
    if( vertical > 1){
		y_coords[vertical - 1] = dimension.y - border_step.y;
    }

    x_coords[1] = border_step.x;
    y_coords[1] = border_step.y;

	x_coords[horizontal] = dimension.x;
	y_coords[vertical]   = dimension.y;

	// Space out the remaining coordinates equally
	if( horizontal > 3 ){
		float x_step( ( dimension.x - 2 * border_step.x ) / static_cast<float>( horizontal - 2 ) );
		for( int i = 2; i <= static_cast<int>( horizontal ) / 2; ++i ){
			x_coords[horizontal - i] = std::floor( dimension.x - border_step.x - static_cast<float>( i - 1 ) * x_step + 0.5f );
			x_coords[i]              = std::floor( border_step.x + static_cast<float>( i - 1 ) * x_step + 0.5f );
		}
	}

	if( vertical > 3 ){
		float y_step( ( dimension.y - 2 * border_step.y ) / static_cast<float>( vertical - 2 ) );
		for( int i = 2; i <= static_cast<int>( vertical ) / 2; ++i ){
			y_coords[vertical - i] = std::floor( dimension.y - border_step.y - static_cast<float>( i - 1 ) * y_step + 0.5f );
			y_coords[i]            = std::floor( border_step.y + static_cast<float>( i - 1 ) * y_step + 0.5f );
		}
	}


	// Calculate texture coordinates
	sf::Vector2f texStartCoord( float_sub_rect.left ,float_sub_rect.top );

	texStartCoord += texture->offset;

	sf::Vector2f texStep( float_sub_rect.width / static_cast<float>( horizontal ), float_sub_rect.height / static_cast<float>( vertical ) );

    primitive->AddTexture( texture );

	PrimitiveVertex vertex0, vertex1, vertex2, vertex3;

    for( unsigned int x = 0; x < horizontal; ++x ){
        for( unsigned int y = 0; y < vertical; ++y ){
			vertex0.position = inverse_transform.transformPoint( sf::Vector2f( x_coords[x],   y_coords[y]   ) + position );
			vertex1.position = inverse_transform.transformPoint( sf::Vector2f( x_coords[x],   y_coords[y+1] ) + position );
			vertex2.position = inverse_transform.transformPoint( sf::Vector2f( x_coords[x+1], y_coords[y]   ) + position );
			vertex3.position = inverse_transform.transformPoint( sf::Vector2f( x_coords[x+1], y_coords[y+1] ) + position );

			vertex0.texture_coordinate = texStartCoord + sf::Vector2f( std::round( static_cast<float>( x )   * texStep.x ), std::round( static_cast<float>( y )   * texStep.y ) );
			vertex1.texture_coordinate = texStartCoord + sf::Vector2f( std::round( static_cast<float>( x )   * texStep.x ), std::round( static_cast<float>( y+1 ) * texStep.y ) );
			vertex2.texture_coordinate = texStartCoord + sf::Vector2f( std::round( static_cast<float>( x+1 ) * texStep.x ), std::round( static_cast<float>( y )   * texStep.y ) );
			vertex3.texture_coordinate = texStartCoord + sf::Vector2f( std::round( static_cast<float>( x+1 ) * texStep.x ), std::round( static_cast<float>( y+1 ) * texStep.y ) );

			primitive->AddVertex( vertex0 );
			primitive->AddVertex( vertex1 );
			primitive->AddVertex( vertex2 );
			primitive->AddVertex( vertex2 );
			primitive->AddVertex( vertex1 );
			primitive->AddVertex( vertex3 );
        }
    }

	Renderer::Get().AddPrimitive( primitive );

    return primitive;
}

}
}
