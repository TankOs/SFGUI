#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Renderer.hpp>

#include <cmath>

namespace sfg {
namespace eng {

Bob::Bob() :
	Engine()
{
	ResetProperties();
}

void Bob::ResetProperties() {
	ClearProperties();

	// Set defaults.
	SetProperty( "*", "Color", sf::Color::Black );
	SetProperty( "*", "FontSize", 14 );
	SetProperty( "*", "FontName", "" ); // Uses SFGUI's default font when empty.
	SetProperty( "*", "BackgroundColor", sf::Color::White );
	SetProperty( "*", "Padding", 5.f );
	SetProperty( "*", "SubRect", UintRect( 0, 0, 0, 0 ) );

	// Button-specific
	SetProperty( "Button", "Spacing", 5.f );

	// CheckButton-specific.
	SetProperty( "CheckButton", "Spacing", 5.f );
	SetProperty( "CheckButton", "BoxSize", 15.f );
	SetProperty( "CheckButton", "SubRect", UintRect( 0, 0, 0, 0 ) );
	SetProperty( "CheckButton", "CheckSubRect", UintRect( 0, 0, 0, 0 ) );

	// Entry-specific
	SetProperty( "Entry", "Thickness", 1.f );

	// ProgressBar-specific.
	SetProperty( "ProgressBar", "BarPadding", 1.f);

	// Scale-specific
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

	// ScrolledWindow-specific
	SetProperty( "ScrolledWindow", "ScrollbarWidth", 20.f );
	SetProperty( "ScrolledWindow", "ScrollbarSpacing", 5.f );
	SetProperty( "ScrolledWindow", "BorderWidth", 0.f );

	// Window-specific
	SetProperty( "Window", "Gap", 10.f );
	SetProperty( "Window", "TitlePadding", 5.f );
	SetProperty( "Window", "WindowSubRect", UintRect( 0, 0, 0, 0 ) );
	SetProperty( "Window", "HandleSize", 10.f );
}

Primitive::Ptr Bob::CreateSpritebox( const sf::FloatRect& rect, const sf::Image *image, UintRect sub_rect ) const
{
	Primitive::Ptr primitive( new Primitive );

	if( image == NULL )
		return primitive;

	SharedPtr< Primitive::Texture > texture( m_texture_manager.GetTexture( image ) );
	if( texture.get() == NULL )
		return primitive;

	sf::FloatRect float_sub_rect = static_cast<sf::FloatRect>( sub_rect );
	if( sub_rect == UintRect( 0, 0, 0, 0 ) ){
		float_sub_rect = sf::FloatRect( sf::Vector2f( 0, 0 ), static_cast<sf::Vector2f>( texture->size ) );
	}

	sf::Vector2f dimension = sf::Vector2f( rect.width, rect.height );
	sf::Vector2f position  = sf::Vector2f( rect.left, rect.top );

    if( dimension.x < 1 || dimension.y < 1 || texture == 0 )
        return primitive;

    //round for pixel perfect rendering
    sf::Vector2f step( std::floor( float_sub_rect.width  / 3.f + 0.5f),
                       std::floor( float_sub_rect.height  / 3.f + 0.5f));

	if (dimension.x < float_sub_rect.width ){
		step.x = dimension.x / 3.f;
	}
	if( dimension.y < float_sub_rect.height ){
		step.y = dimension.y / 3.f;
	}

    sf::Vector2f coords[4];
    coords[0] = sf::Vector2f( 0, 0 );
    coords[1] = step;
    coords[2] = dimension - step;
    coords[3] = dimension;

    sf::Vector2f texStartCoord( float_sub_rect.left ,float_sub_rect.top );

	texStartCoord += texture->offset;

	sf::Vector2f texStep( float_sub_rect.width  / 3.f,
						  float_sub_rect.height / 3.f );

    primitive->AddTexture( texture );

	Primitive::Vertex vertex0, vertex1, vertex2, vertex3;

    for( unsigned int x = 0; x < 3; ++x ){
        for( unsigned int y = 0; y < 3; ++y ){
			vertex0.position = sf::Vector2f( coords[x].x,   coords[y].y )   + position;
			vertex1.position = sf::Vector2f( coords[x].x,   coords[y+1].y ) + position;
			vertex2.position = sf::Vector2f( coords[x+1].x, coords[y].y )   + position;
			vertex3.position = sf::Vector2f( coords[x+1].x, coords[y+1].y ) + position;

			vertex0.texture_coordinate = sf::Vector2f( texStartCoord.x +  static_cast<float>( x )   * texStep.x, texStartCoord.y + static_cast<float>( y )   * texStep.y );
			vertex1.texture_coordinate = sf::Vector2f( texStartCoord.x +  static_cast<float>( x )   * texStep.x, texStartCoord.y + static_cast<float>( y+1 ) * texStep.y );
			vertex2.texture_coordinate = sf::Vector2f( texStartCoord.x +  static_cast<float>( x+1 ) * texStep.x, texStartCoord.y + static_cast<float>( y )   * texStep.y );
			vertex3.texture_coordinate = sf::Vector2f( texStartCoord.x +  static_cast<float>( x+1 ) * texStep.x, texStartCoord.y + static_cast<float>( y+1 ) * texStep.y );

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
