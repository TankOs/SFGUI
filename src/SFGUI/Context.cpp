#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

Context* Context::m_active_context( 0 );

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
	m_engine( NULL )
{
}

Context::~Context() {
}

Engine& Context::GetDefaultEngine() {
	static eng::BREW brew;
	return brew;
}

Engine& Context::GetEngine() const {
	return (m_engine ? *m_engine : GetDefaultEngine());
}

void Context::SetEngine( Engine& engine ) {
	m_engine = &engine;
}

void Context::SetActiveWidget( SharedPtr<Widget> widget ) {
	m_active_widget = widget;
}

SharedPtr<Widget> Context::GetActiveWidget() const {
	return m_active_widget.lock();
}

}
