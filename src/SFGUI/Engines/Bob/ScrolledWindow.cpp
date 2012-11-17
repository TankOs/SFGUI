#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Engines/Bob/Spritebox.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ScrolledWindow.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateScrolledWindowDrawable( SharedPtr<const ScrolledWindow> scrolled_window ) const {
	float border_width( GetProperty<float>( "BorderWidth", scrolled_window ) );

	RenderQueue* queue( new RenderQueue );

	const sf::Image *window_image( GetResourceManager().GetImage(GetProperty<std::string>( "Image", scrolled_window ) ) );
	if(window_image != NULL){
		bob::Spritebox spritebox;
		spritebox.SetDimension( sf::Vector2i( static_cast<int>( scrolled_window->GetContentAllocation().width  + 2 * border_width ),
											  static_cast<int>( scrolled_window->GetContentAllocation().height + 2 * border_width ) ) );

		SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( window_image ) );
		spritebox.SetTexture( texture_handle );

		Primitive::Ptr primitive = spritebox.ConstructPrimitive();

		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	return queue;
}

}
}
