#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ComboBox.hpp>

#include <SFML/Graphics/Text.hpp>
namespace sfg {
namespace eng {

RenderQueue* Bob::CreateComboBoxDrawable( SharedPtr<const ComboBox> /*combo_box*/ ) const {
	return NULL;
}

}
}
