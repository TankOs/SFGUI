#include <SFGUI/Image.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Primitive.hpp>
#include <SFGUI/PrimitiveTexture.hpp>

#include <SFML/Graphics/Image.hpp>

namespace sfg {

Image::Image( const sf::Image& image )
{
	SetAlignment( sf::Vector2f( .5f, .5f ) );
	SetImage( image );
}

Image::Ptr Image::Create( const sf::Image& image ) {
	return Ptr( new Image( image ) );
}

void Image::SetImage( const sf::Image& image ) {
	if( !image.getSize().x || !image.getSize().y ) {
		return;
	}

	if( m_image.getSize() == image.getSize() ) {
		m_image = image;

		sfg::Renderer::Get().UpdateImage( m_texture_offset, image );
	}
	else {
		m_image = image;

		RequestResize();
		Invalidate();
	}
}

const sf::Image& Image::GetImage() const {
	return m_image;
}

std::unique_ptr<RenderQueue> Image::InvalidateImpl() const {
	std::unique_ptr<RenderQueue> queue = Context::Get().GetEngine().CreateImageDrawable( std::dynamic_pointer_cast<const Image>( shared_from_this() ) );

	m_texture_offset = queue->GetPrimitives()[0]->GetTextures()[0]->offset;

	return queue;
}

sf::Vector2f Image::CalculateRequisition() {
	return sf::Vector2f( static_cast<float>( m_image.getSize().x ), static_cast<float>( m_image.getSize().y ) );
}

const std::string& Image::GetName() const {
	static const std::string name( "Image" );
	return name;
}

}
