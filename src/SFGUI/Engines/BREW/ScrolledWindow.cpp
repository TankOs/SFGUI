#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Color.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateScrolledWindowDrawable( std::shared_ptr<const ScrolledWindow> scrolled_window ) const {
	auto border_color_light = GetProperty<sf::Color>( "BorderColor", scrolled_window );
	auto border_color_dark = GetProperty<sf::Color>( "BorderColor", scrolled_window );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", scrolled_window );
	auto border_width = GetProperty<float>( "BorderWidth", scrolled_window );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	auto rect = scrolled_window->GetContentAllocation();

	rect.left -= ( border_width + scrolled_window->GetAllocation().left );
	rect.top -= ( border_width + scrolled_window->GetAllocation().top );
	rect.width += 2.f * border_width;
	rect.height += 2.f * border_width;

	queue->Add(
		CreateBorder(
			sf::FloatRect(
				0.f,
				0.f,
				rect.width,
				rect.height
			),
			border_width,
			border_color_dark,
			border_color_light
		)
	);

	return queue;
}

}
}
