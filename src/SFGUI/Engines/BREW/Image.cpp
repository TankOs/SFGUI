#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Image.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateImageDrawable( SharedPtr<const Image> image ) const {
	RenderQueue* queue( new RenderQueue );

	queue->Add(
		Context::Get().GetProjectO().CreateImage(
			sf::FloatRect(
				( image->GetAllocation().Width - image->GetRequisition().x ) * image->GetAlignment().x,
				( image->GetAllocation().Height - image->GetRequisition().y ) * image->GetAlignment().y,
				static_cast<float>( image->GetImage().GetWidth() ),
				static_cast<float>( image->GetImage().GetHeight() )
			),
			image->GetImage()
		)
	);

	return queue;
}

}
}
