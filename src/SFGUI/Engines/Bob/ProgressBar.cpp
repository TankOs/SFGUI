#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ProgressBar.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateProgressBarDrawable( SharedPtr<const ProgressBar> /*progress_bar*/ ) const {
	return NULL;
}

}
}
