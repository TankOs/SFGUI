#include <SFGUI/RendererViewport.hpp>

namespace sfg {

RendererViewport::RendererViewport() :
	source_origin( sf::Vector2f( 0.f, 0.f ) ),
	destination_origin( sf::Vector2f( 0.f, 0.f ) ),
	size( sf::Vector2f( 0.f, 0.f ) )
{
	id = last_id++;
}

void RendererViewport::Apply() {

}

}
