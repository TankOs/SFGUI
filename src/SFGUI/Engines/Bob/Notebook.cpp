#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Notebook.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateNotebookDrawable( std::shared_ptr<const Notebook> /*notebook*/ ) const {
	return nullptr;
}

}
}
