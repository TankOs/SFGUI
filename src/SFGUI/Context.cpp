#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace {

sfg::Context* active_context = nullptr;
std::shared_ptr<sfg::eng::BREW> default_engine;

}

namespace sfg {

Context& Context::Get() {
	if( active_context ) {
		return *active_context;
	}

	static Context context;

	return context;
}

bool Context::Activate( Context& context ) {
	if( active_context ) {
		return false;
	}

	active_context = &context;
	return true;
}

bool Context::Deactivate() {
	if( !active_context ) {
		return false;
	}

	active_context = 0;
	return true;
}

Context::Context() :
	m_engine( nullptr )
{
}

Engine& Context::GetDefaultEngine() {
	if( !default_engine ) {
		default_engine = std::make_shared<eng::BREW>();
	}

	return *default_engine;
}

/// @cond

void Context::DestroyDefaultEngine() {
	default_engine.reset();
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
