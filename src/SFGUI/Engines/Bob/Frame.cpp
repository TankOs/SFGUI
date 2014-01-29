#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Frame.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateFrameDrawable( std::shared_ptr<const Frame> /*frame*/ ) const {
	return nullptr;
}

}
}
