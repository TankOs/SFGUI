#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Context.hpp>

namespace {

bool alive = false;

}

namespace sfg {

SFGUI::SFGUI() {
	Renderer::Create();

	alive = true;
}

SFGUI::~SFGUI() {
	Renderer::Destroy();
	Context::DestroyDefaultEngine();

	alive = false;
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
	return alive;
}

}
