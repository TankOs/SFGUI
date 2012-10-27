#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Separator.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateSeparatorDrawable( SharedPtr<const Separator> /*separator*/ ) const {
	return NULL;
}

}
}
