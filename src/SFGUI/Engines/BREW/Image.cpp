#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Image.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateImageDrawable( SharedPtr<const Image> image ) const {
	RenderQueue* queue( new RenderQueue );

	sf::Sprite* sprite = new sf::Sprite( image->GetTexture() );
	sprite->SetPosition(
		( image->GetAllocation().Width - image->GetRequisition().x ) * image->GetAlignment().x,
		( image->GetAllocation().Height - image->GetRequisition().y ) * image->GetAlignment().y
	);

	queue->Add( sprite );
	return queue;
}

}
}
