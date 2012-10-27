#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scale.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateScaleDrawable( SharedPtr<const Scale> /*scale*/ ) const {
	return NULL;
}

}
}
