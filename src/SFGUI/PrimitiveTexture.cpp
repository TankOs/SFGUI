#include <SFGUI/PrimitiveTexture.hpp>
#include <SFGUI/Renderer.hpp>

#include <SFML/Graphics/Image.hpp>

namespace sfg {

PrimitiveTexture::~PrimitiveTexture() {
	if( sfg::Renderer::Exists() ) {
		sfg::Renderer::Get().UnloadImage( offset );
	}
}

void PrimitiveTexture::Update( const sf::Image& data ) {
	if( data.getSize() != size ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "Tried to update texture with mismatching image size.\n";
#endif
		return;
	}

	sfg::Renderer::Get().UpdateImage( offset, data );
}

}
