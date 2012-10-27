#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/ScrolledWindow.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateScrolledWindowDrawable( SharedPtr<const ScrolledWindow> /*scrolled_window*/ ) const {
	return NULL;
}

}
}
