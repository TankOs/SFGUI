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

void SFGUI::Display( sf::RenderTarget& target ) {
	Renderer::Get().Display( target );
}

void SFGUI::TuneAlphaThreshold( float alpha_threshold ) {
	Renderer::Get().TuneAlphaThreshold( alpha_threshold );
}

void SFGUI::TuneCull( bool enable ) {
	Renderer::Get().TuneCull( enable );
}

void SFGUI::TuneUseFBO( bool enable ) {
	Renderer::Get().TuneUseFBO( enable );
}

bool SFGUI::IsAlive() {
	return m_alive;
}

}
