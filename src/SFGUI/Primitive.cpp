#include <SFGUI/Primitive.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Primitive::Primitive() :
	position( sf::Vector2f( 0.f, 0.f ) ),
	layer( 0.f ),
	level( 0 ),
	synced( false ),
	visible( true )
{
	viewport = Context::Get().GetRenderer().GetDefaultViewport();
}

void Primitive::Add( const Primitive& primitive ) {
	std::size_t vertice_count = primitive.vertices.size();

	for( std::size_t index = 0; index < vertice_count; ++index ) {
		vertices.push_back( primitive.vertices[index] );
	}
}

}
