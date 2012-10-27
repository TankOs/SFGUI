#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Spinner.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateSpinnerDrawable( SharedPtr<const Spinner> /*spinner*/ ) const {
    return NULL;
}

}
}
