#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ProgressBar.hpp>
#include <SFGUI/Engines/Bob/Spritebox.hpp>

#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateProgressBarDrawable( SharedPtr<const ProgressBar> progress_bar ) const {
	RenderQueue* queue( new RenderQueue );

	//Background
	bob::Spritebox background_box;
	background_box.SetDimension( sf::Vector2i( static_cast<int>( progress_bar->GetAllocation().width ), static_cast<int>( progress_bar->GetAllocation().height ) ) );

	const sf::Image *image( GetResourceManager().GetImage( GetProperty<std::string>( "BackgroundImage", progress_bar ) ) );
	if( image == NULL )
		return queue;

	SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( image ) );
	background_box.SetTexture( texture_handle );

	Primitive::Ptr primitive = background_box.ConstructPrimitive();

	Renderer::Get().AddPrimitive( primitive );
	queue->Add( primitive );

	//Bar
	if( progress_bar->GetFraction() > 0.f ) {
		float padding = GetProperty<float>( "BarPadding", progress_bar );

		sf::Vector2f bar_dimension(
			std::max( 0.f, progress_bar->GetAllocation().width  - 2.f * padding ),
			std::max( 0.f, progress_bar->GetAllocation().height - 2.f * padding )
		);
		sf::Vector2f bar_position( padding, padding );

		if( progress_bar->GetOrientation() == ProgressBar::HORIZONTAL ) {
			bar_dimension.x *= progress_bar->GetFraction();
		}
		else {
			bar_dimension.y *= progress_bar->GetFraction();
			bar_position.y = std::floor( progress_bar->GetAllocation().height + 0.5f) - std::floor( bar_dimension.y + 0.5f ) - padding;
		}

		bob::Spritebox bar_box;

		bar_box.SetDimension( static_cast<sf::Vector2i>( bar_dimension ) );
		bar_box.SetPosition( bar_position );

		const sf::Image *bar_image( GetResourceManager().GetImage( GetProperty<std::string>( "BarImage", progress_bar ) ) );
		if( image == NULL )
			return queue;

		texture_handle = m_texture_manager.GetTexture( bar_image );
		bar_box.SetTexture( texture_handle );

		primitive = bar_box.ConstructPrimitive();

		Renderer::Get().AddPrimitive( primitive );
		queue->Add( primitive );
	}

	return queue;
}

}
}
