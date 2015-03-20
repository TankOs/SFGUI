#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Image.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateImageDrawable( std::shared_ptr<const Image> image ) const {
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", image ) );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );
	auto texture = Renderer::Get().LoadTexture( image->GetImage() );

	queue->Add(
		Renderer::Get().CreateSprite(
			sf::FloatRect(
				( image->GetAllocation().width - image->GetRequisition().x ) * image->GetAlignment().x,
				( image->GetAllocation().height - image->GetRequisition().y ) * image->GetAlignment().y,
				static_cast<float>( image->GetImage().getSize().x ),
				static_cast<float>( image->GetImage().getSize().y )
			),
			texture
		)
	);

	return queue;
}

}
}
