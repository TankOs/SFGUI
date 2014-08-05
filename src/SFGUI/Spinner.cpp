#include <SFGUI/Spinner.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/System/Vector2.hpp>

namespace sfg {

Spinner::Spinner() :
	m_elapsed_time( 0.f ),
	m_stage( 1 ),
	m_started( false )
{
}

Spinner::Ptr Spinner::Create() {
	return Ptr( new Spinner );
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

std::unique_ptr<RenderQueue> Spinner::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateSpinnerDrawable( std::dynamic_pointer_cast<const Spinner>( shared_from_this() ) );
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
