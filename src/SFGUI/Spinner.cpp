#include <SFGUI/Spinner.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Spinner::Spinner() :
	m_renderqueue( 0 ),
	m_started( false )
{
}

Spinner::~Spinner() {
}

Spinner::Ptr Spinner::Create() {
	Spinner::Ptr ptr( new Spinner() );

	return ptr;
}

void Spinner::Start() {
	m_started = true;
	m_clock.Reset();

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

void Spinner::HandleExpose( CullingTarget& /*target*/ ) const {
	float duration( Context::Get().GetEngine().GetProperty<float>( "CycleDuration", shared_from_this() ) );
	float steps( Context::Get().GetEngine().GetProperty<float>( "Steps", shared_from_this() ) );

	if( m_started && ( static_cast<float>( m_clock.GetElapsedTime() ) > ( duration / steps ) ) ) {
		m_clock.Reset();

		m_renderqueue->GetDrawables()[0].first->Rotate( 360.f / steps );
		m_renderqueue->Compile();
	}
}

}
