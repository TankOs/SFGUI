#include <SFGUI/Spinner.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Spinner::Spinner() :
	m_elapsed_time( 0.f ),
	m_stage( 1 ),
	m_renderqueue( 0 ),
	m_started( false )
{
}

Spinner::~Spinner() {
}

Spinner::Ptr Spinner::Create() {
	Spinner::Ptr ptr( new Spinner );

	return ptr;
}

void Spinner::Start() {
	m_started = true;
	m_elapsed_time = 0.f;
	m_stage = 1;

	Invalidate();
}

void Spinner::Stop() {
	m_started = false;

	Invalidate();
}

bool Spinner::Started() const {
	return m_started;
}

RenderQueue* Spinner::InvalidateImpl() const {
	m_renderqueue = Context::Get().GetEngine().CreateSpinnerDrawable( DynamicPointerCast<const Spinner>( shared_from_this() ) );
	return m_renderqueue;
}

sf::Vector2f Spinner::CalculateRequisition() {
	// Make it obvious where the spinner is if the user forgot to set requisition.
	return sf::Vector2f( 4.f, 4.f );
}

const std::string& Spinner::GetName() const {
	static const std::string name( "Spinner" );
	return name;
}

void Spinner::HandleUpdate( float seconds ) {
	float duration( Context::Get().GetEngine().GetProperty<float>( "CycleDuration", shared_from_this() ) );
	unsigned int steps( Context::Get().GetEngine().GetProperty<unsigned int>( "Steps", shared_from_this() ) );

	m_elapsed_time += seconds;

	if( m_started && ( m_elapsed_time > ( ( duration / static_cast<float>( steps ) ) / 1000.f ) ) ) {
		m_elapsed_time = 0.f;

		m_stage = ( m_stage + 1 ) % steps;

		Invalidate();
	}
}

unsigned int Spinner::GetStage() const {
	return m_stage;
}

}
