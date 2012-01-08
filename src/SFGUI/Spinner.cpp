#include <SFGUI/Spinner.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/VertexArray.hpp>

namespace sfg {

Spinner::Spinner() :
	m_elapsed_time( 0.f ),
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
	float steps( Context::Get().GetEngine().GetProperty<float>( "Steps", shared_from_this() ) );

	m_elapsed_time += seconds;

	if( m_started && ( m_elapsed_time > ( ( duration / steps ) / 1000.f ) ) ) {
		m_elapsed_time = 0.f;

		// We built it... so we can make some useful assumptions.
		const std::vector<ProjectO::PrimitivePtr>& primitives( m_renderqueue->GetPrimitives() );

		// Epic variable name
		int alpha_delta = static_cast<int>( 255.f / ( steps - 1.f ) );

		std::size_t primitives_size = primitives.size();

		for( std::size_t index = 0; index < primitives_size; ++index ) {
			// We exploit integer wraparound here ;)
			primitives[index]->vertices[0].color.a = static_cast<sf::Uint8>( primitives[index]->vertices[0].color.a - alpha_delta );
			primitives[index]->vertices[1].color.a = static_cast<sf::Uint8>( primitives[index]->vertices[1].color.a - alpha_delta );
			primitives[index]->vertices[2].color.a = static_cast<sf::Uint8>( primitives[index]->vertices[2].color.a - alpha_delta );
			primitives[index]->vertices[3].color.a = static_cast<sf::Uint8>( primitives[index]->vertices[3].color.a - alpha_delta );
			primitives[index]->vertices[4].color.a = static_cast<sf::Uint8>( primitives[index]->vertices[4].color.a - alpha_delta );
			primitives[index]->vertices[5].color.a = static_cast<sf::Uint8>( primitives[index]->vertices[5].color.a - alpha_delta );
			primitives[index]->synced = false;
		}
	}
}

}
