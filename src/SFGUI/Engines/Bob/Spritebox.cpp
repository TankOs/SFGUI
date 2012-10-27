#include <SFGUI/Engines/Bob/Spritebox.hpp>
#include <SFGUI/Primitive.hpp>

#include <cmath>

namespace sfg {
namespace eng {
namespace bob {

Spritebox::Spritebox( sf::Vector2i dimension ) :
    m_subRect( sf::Vector2i( 0, 0 ), dimension ),
	m_dimension( dimension ),
	m_texture()
{
}

Spritebox::Spritebox( SharedPtr< Primitive::Texture > texture, sf::Vector2i dimension ) :
	m_subRect( sf::Vector2i( 0, 0 ), dimension ),
	m_dimension( dimension ),
	m_texture()
{
	SetTexture(texture);
}

void Spritebox::SetTexture( SharedPtr< Primitive::Texture > texture, bool adjustSubRect )
{
	//if no texture was set before adjust subRect to texture size
	if( !m_texture ){
		adjustSubRect = true;
	}

	m_texture = texture;
	if( adjustSubRect ){
        m_subRect = sf::IntRect( 0, 0, m_texture->size.x, m_texture->size.y );
	}
}

SharedPtr< Primitive::Texture > Spritebox::GetTexture() const
{
    return m_texture;
}

void Spritebox::SetSubRect( const sf::IntRect &subRect )
{
    m_subRect = subRect;
}

const sf::IntRect &Spritebox::GetSubRect() const
{
    return m_subRect;
}

void Spritebox::SetDimension( const sf::Vector2i &vec )
{
    m_dimension = vec;
}

const sf::Vector2i &Spritebox::GetDimension() const
{
    return m_dimension;
}

Primitive::Ptr Spritebox::ConstructPrimitive() const
{
	Primitive::Ptr primitive( new Primitive );

    if( m_dimension.x < 1 || m_dimension.y < 1 || m_texture == 0 )
        return primitive;

    //round for pixel perfect rendering
    sf::Vector2f step( std::floor( static_cast<float>( m_subRect.width ) /3.f + 0.5f),
                       std::floor( static_cast<float>( m_subRect.height )/3.f + 0.5f));

	if (m_dimension.x < m_subRect.width ){
		step.x = static_cast<float>( m_dimension.x )/3.f;
	}
	if( m_dimension.y < m_subRect.height ){
		step.y = static_cast<float>( m_dimension.y )/3.f;
	}

    sf::Vector2f coords[4];
    coords[0] = sf::Vector2f( 0, 0 );
    coords[1] = step;
    coords[2] = static_cast<sf::Vector2f>( m_dimension ) - step;
    coords[3] = static_cast<sf::Vector2f>( m_dimension );

    sf::Vector2f texStartCoord(static_cast<float>( m_subRect.left ) / static_cast<float>( m_texture->size.x ),
							   static_cast<float>( m_subRect.top )  / static_cast<float>( m_texture->size.y ) );

	texStartCoord += m_texture->offset;

	sf::Vector2f texStep( static_cast<float>( m_subRect.width )  / 3.f,
						  static_cast<float>( m_subRect.height ) / 3.f );

    primitive->AddTexture( m_texture );

	Primitive::Vertex vertex0, vertex1, vertex2, vertex3;

    for( unsigned int x = 0; x < 3; ++x ){
        for( unsigned int y = 0; y < 3; ++y ){
			vertex0.position = sf::Vector2f( coords[x].x,   coords[y].y );
			vertex1.position = sf::Vector2f( coords[x].x,   coords[y+1].y );
			vertex2.position = sf::Vector2f( coords[x+1].x, coords[y].y );
			vertex3.position = sf::Vector2f( coords[x+1].x, coords[y+1].y );

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

    return primitive;
}


sf::FloatRect Spritebox::GetRect() const
{
	return sf::FloatRect( sf::Vector2f( 0, 0 ), static_cast< sf::Vector2f >( GetDimension() ) );
}

}
}
}
