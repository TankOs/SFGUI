#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Separator.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateSeparatorDrawable( std::shared_ptr<const Separator> /*separator*/ ) const {
	return nullptr;
}

}
}
