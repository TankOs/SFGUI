#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Notebook.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateNotebookDrawable( SharedPtr<const Notebook> /*notebook*/ ) const {
	return NULL;
}

}
}
