#include <SFGUI/SFGUI.hpp>

namespace sfg {

SFGUI::SFGUI() {
	Renderer::Get();
}

SFGUI::~SFGUI() {
	Renderer::Destroy();
}

}
