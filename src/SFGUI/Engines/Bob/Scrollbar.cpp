#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scrollbar.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateScrollbarDrawable( SharedPtr<const Scrollbar> /*scrollbar*/ ) const {
	return NULL;
}

}
}
