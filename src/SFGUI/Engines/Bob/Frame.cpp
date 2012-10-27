#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Frame.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateFrameDrawable( SharedPtr<const Frame> /*frame*/ ) const {
	return NULL;
}

}
}
