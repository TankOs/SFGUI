#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Spinner.hpp>

#include <SFML/Graphics/Transform.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateSpinnerDrawable( std::shared_ptr<const Spinner> spinner ) const {
    std::unique_ptr<RenderQueue> queue( new RenderQueue );

    std::shared_ptr<const sf::Image> image = GetResourceManager().GetImage( GetProperty<std::string>( "Image", spinner ) );
    if( !image )
		return queue;

   	std::shared_ptr< Primitive::Texture > texture( m_texture_manager.GetTexture( image ) );
	if( !texture )
		return queue;

    auto subRect = GetProperty<sf::FloatRect>( "SubRect", spinner );
    if( subRect == sf::FloatRect() ) {
		subRect = sf::FloatRect( 0.f, 0.f, static_cast<float>( texture->size.x ), static_cast<float>( texture->size.y ) );
    }

	// Only quadratic spinners.
	float size = std::min( spinner->GetAllocation().width, spinner->GetAllocation().height );
	sf::Vector2f dim = sf::Vector2f( size, size );
	sf::Vector2f offset = ( sf::Vector2f( spinner->GetAllocation().width, spinner->GetAllocation().height ) - dim ) / 2.f;

	unsigned int steps( GetProperty<unsigned int>( "Steps", spinner ) );

	// Use SFML here instead of some nasty trigonometry.
	sf::Transform mat;
	mat.translate( offset );
	mat.rotate( ( static_cast<float>( spinner->GetStage() ) / static_cast<float>( steps ) ) * 360.f, dim / 2.f );

	Primitive::Vertex vertex0, vertex1, vertex2, vertex3;

	vertex0.position = mat.transformPoint( sf::Vector2f( 0, 0 ) );
	vertex1.position = mat.transformPoint( sf::Vector2f( 0, dim.y ) );
	vertex2.position = mat.transformPoint( sf::Vector2f( dim.x, 0) );
	vertex3.position = mat.transformPoint( dim );

	vertex0.texture_coordinate = texture->offset + sf::Vector2f( subRect.left, subRect.top );
	vertex1.texture_coordinate = texture->offset + sf::Vector2f( subRect.left, subRect.top + subRect.height );
	vertex2.texture_coordinate = texture->offset + sf::Vector2f( subRect.left + subRect.width, subRect.top );
	vertex3.texture_coordinate = texture->offset + sf::Vector2f( subRect.left + subRect.width, subRect.top + subRect.height );

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
