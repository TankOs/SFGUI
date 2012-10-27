#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ToggleButton.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateToggleButtonDrawable( SharedPtr<const ToggleButton> /*button*/ ) const {
    return NULL;
}

}
}
