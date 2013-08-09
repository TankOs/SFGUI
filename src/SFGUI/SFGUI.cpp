#include <SFGUI/SFGUI.hpp>

namespace sfg {

bool SFGUI::m_alive = false;

SFGUI::SFGUI() {
	Renderer::Create();

	m_alive = true;
}

SFGUI::~SFGUI() {
	Renderer::Destroy();
	Context::DestroyDefaultEngine();
}

void SFGUI::Display( sf::Window& target ) {
	Renderer::Get().Display( target );
}

void SFGUI::Display( sf::RenderWindow& target ) {
	Renderer::Get().Display( target );
}

void SFGUI::Display( sf::RenderTexture& target ) {
	Renderer::Get().Display( target );
}

Renderer& SFGUI::GetRenderer() {
	return Renderer::Get();
}

bool SFGUI::IsAlive() {
	return m_alive;
}

}
