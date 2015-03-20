#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/ScrolledWindow.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateScrolledWindowDrawable( std::shared_ptr<const ScrolledWindow> scrolled_window ) const {
	float border_width( GetProperty<float>( "BorderWidth", scrolled_window ) );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	queue->Add( CreateSpritebox( sf::FloatRect( 0.f, 0.f,
												scrolled_window->GetContentAllocation().width  + 2 * border_width,
												scrolled_window->GetContentAllocation().height + 2 * border_width ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "Image", scrolled_window ) ),
								 GetProperty<UintRect>( "SubRect", scrolled_window ) ) );

	return queue;
}

}
}
