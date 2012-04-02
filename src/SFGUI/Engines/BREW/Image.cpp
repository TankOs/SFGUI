#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Image.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateImageDrawable( SharedPtr<const Image> image ) const {
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", image ) );

	RenderQueue* queue( new RenderQueue );

	queue->Add(
		Renderer::Get().CreateImage(
			sf::FloatRect(
				( image->GetAllocation().width - image->GetRequisition().x ) * image->GetAlignment().x,
				( image->GetAllocation().height - image->GetRequisition().y ) * image->GetAlignment().y,
				static_cast<float>( image->GetImage().getSize().x ),
				static_cast<float>( image->GetImage().getSize().y )
			),
			image->GetImage()
		)
	);

	return queue;
}

}
}
