#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Image.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateImageDrawable( SharedPtr<const Image> /*image*/ ) const {
	return NULL;
}

}
}
