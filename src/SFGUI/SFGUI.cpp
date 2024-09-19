#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

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

void SFGUI::AddCharacterSet( std::uint32_t low_bound, std::uint32_t high_bound ) {
	Renderer::Get().AddCharacterSet( low_bound, high_bound );
	Context::Get().GetEngine().AddCharacterSet( low_bound, high_bound );
}

}
