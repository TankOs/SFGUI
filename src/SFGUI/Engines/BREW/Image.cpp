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
				( image->GetAllocation().Width - image->GetRequisition().x ) * image->GetAlignment().x,
				( image->GetAllocation().Height - image->GetRequisition().y ) * image->GetAlignment().y,
				static_cast<float>( image->GetImage().GetWidth() ),
				static_cast<float>( image->GetImage().GetHeight() )
			),
			image->GetImage(),
			background_color
		)
	);

	return queue;
}

}
}
