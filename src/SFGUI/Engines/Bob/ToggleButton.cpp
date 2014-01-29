#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ToggleButton.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateToggleButtonDrawable( std::shared_ptr<const ToggleButton> /*button*/ ) const {
    return nullptr;
}

}
}
