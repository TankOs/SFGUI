#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Engines/Bob/Spritebox.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ScrolledWindow.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateScrolledWindowDrawable( SharedPtr<const ScrolledWindow> scrolled_window ) const {
	float border_width( GetProperty<float>( "BorderWidth", scrolled_window ) );

	RenderQueue* queue( new RenderQueue );

	queue->Add( CreateSpritebox( sf::FloatRect( 0.f, 0.f,
												scrolled_window->GetContentAllocation().width  + 2 * border_width,
												scrolled_window->GetContentAllocation().height + 2 * border_width ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "Image", scrolled_window ) ),
								 UintRect( 0, 0, 0, 0 ) ) );

	return queue;
}

}
}
