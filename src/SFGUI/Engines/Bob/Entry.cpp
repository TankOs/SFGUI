#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Entry.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateEntryDrawable( SharedPtr<const Entry> /*entry*/ ) const {
	return NULL;
}

}
}
