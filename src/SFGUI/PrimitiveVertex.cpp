#include <SFGUI/PrimitiveVertex.hpp>

namespace sfg {

PrimitiveVertex::PrimitiveVertex() :
	color( sf::Color::White )
{
}

PrimitiveVertex::PrimitiveVertex( const PrimitiveVertex& other ) :
	position( other.position ),
	color( other.color ),
	texture_coordinate( other.texture_coordinate )
{
}

bool PrimitiveVertex::operator==( const PrimitiveVertex& other ) const {
	if( position != other.position ) {
		return false;
	}

	if( texture_coordinate != other.texture_coordinate ) {
		return false;
	}

	if( color != other.color ) {
		return false;
	}

	return true;
}

}
