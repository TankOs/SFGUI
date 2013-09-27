#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Spinner.hpp>

#include <SFML/Graphics/Transform.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateSpinnerDrawable( SharedPtr<const Spinner> spinner ) const {
    RenderQueue* queue( new RenderQueue );

    const sf::Image *image = GetResourceManager().GetImage( GetProperty<std::string>( "Image", spinner ) );
    if( image == NULL )
		return queue;

   	SharedPtr< Primitive::Texture > texture( m_texture_manager.GetTexture( image ) );
	if( texture.get() == NULL )
		return queue;

	// Only quadratic spinners.
	float size = std::min( spinner->GetAllocation().width, spinner->GetAllocation().height );
	sf::Vector2f dim = sf::Vector2f( size, size );
	sf::Vector2f offset = ( sf::Vector2f( spinner->GetAllocation().width, spinner->GetAllocation().height ) - dim ) / 2.f;

	unsigned int steps( GetProperty<unsigned int>( "Steps", spinner ) );

	// Use SFML here instead of some nasty trigonometry.
	sf::Transform mat;
	mat.rotate( static_cast<float>( spinner->GetStage() ) / static_cast<float>( steps ) * 360.f, dim / 2.f );

	Primitive::Vertex vertex0, vertex1, vertex2, vertex3;

	vertex0.position = offset + mat.transformPoint( sf::Vector2f( 0, 0 ) );
	vertex1.position = offset + mat.transformPoint( sf::Vector2f( 0, dim.y ) );
	vertex2.position = offset + mat.transformPoint( sf::Vector2f( dim.x, 0) );
	vertex3.position = offset + mat.transformPoint( dim );

	vertex0.texture_coordinate = texture->offset + sf::Vector2f( 0, 0 );
	vertex1.texture_coordinate = texture->offset + sf::Vector2f( 0, static_cast<float>( texture->size.y ) );
	vertex2.texture_coordinate = texture->offset + sf::Vector2f( static_cast<float>( texture->size.x ), 0 );
	vertex3.texture_coordinate = texture->offset + sf::Vector2f( texture->size );

	Primitive::Ptr primitive( new Primitive( 6 ) );
	primitive->AddTexture( texture );

	primitive->AddVertex( vertex0 );
	primitive->AddVertex( vertex1 );
	primitive->AddVertex( vertex2 );
	primitive->AddVertex( vertex2 );
	primitive->AddVertex( vertex1 );
	primitive->AddVertex( vertex3 );

	Renderer::Get().AddPrimitive( primitive );
	queue->Add( primitive );

    return queue;
}

}
}
