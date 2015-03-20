#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/SpinButton.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateSpinButtonDrawable( std::shared_ptr<const SpinButton> /*spinbutton*/ ) const {
    return nullptr;
}

}
}
