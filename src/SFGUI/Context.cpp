#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

Context* Context::m_active_context( 0 );
std::shared_ptr<eng::BREW> Context::m_default_engine;

Context& Context::Get() {
	if( m_active_context ) {
		return *m_active_context;
	}

	static Context context;

	return context;
}

bool Context::Activate( Context& context ) {
	if( m_active_context ) {
		return false;
	}

	m_active_context = &context;
	return true;
}

bool Context::Deactivate() {
	if( !m_active_context ) {
		return false;
	}

	m_active_context = 0;
	return true;
}

Context::Context() :
	m_engine( nullptr )
{
}

Engine& Context::GetDefaultEngine() {
	if( !m_default_engine ) {
		m_default_engine = std::make_shared<eng::BREW>();
	}

	return *m_default_engine;
}

/// @cond

void Context::DestroyDefaultEngine() {
	m_default_engine.reset();
}

/// @endcond

Engine& Context::GetEngine() const {
	return (m_engine ? *m_engine : GetDefaultEngine());
}

void Context::SetEngine( Engine& engine ) {
	m_engine = &engine;
}

void Context::SetActiveWidget( std::shared_ptr<Widget> widget ) {
	m_active_widget = widget;
}

std::shared_ptr<Widget> Context::GetActiveWidget() const {
	return m_active_widget.lock();
}

}
