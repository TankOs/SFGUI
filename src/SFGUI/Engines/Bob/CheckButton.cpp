#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/CheckButton.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateCheckButtonDrawable( SharedPtr<const CheckButton> /*check*/ ) const {
	return NULL;
}

}
}
