#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/SpinButton.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateSpinButtonDrawable( SharedPtr<const SpinButton> /*spinbutton*/ ) const {
    return NULL;
}

}
}
