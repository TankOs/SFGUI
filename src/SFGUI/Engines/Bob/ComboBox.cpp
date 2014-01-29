#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ComboBox.hpp>

#include <SFML/Graphics/Text.hpp>
namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateComboBoxDrawable( std::shared_ptr<const ComboBox> /*combo_box*/ ) const {
	return nullptr;
}

}
}
