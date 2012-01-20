#include <SFGUI/Spinner.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/VertexArray.hpp>
#include <cassert>

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
	float steps( Context::Get().GetEngine().GetProperty<float>( "Steps", shared_from_this() ) );
	sf::Color color( Context::Get().GetEngine().GetProperty<sf::Color>( "Color", shared_from_this() ) );
	sf::Color background_color( Context::Get().GetEngine().GetProperty<sf::Color>( "BackgroundColor", shared_from_this() ) );

	m_elapsed_time += seconds;

	if( m_started && ( m_elapsed_time > ( ( duration / steps ) / 1000.f ) ) ) {
		m_elapsed_time = 0.f;

		// We built it... so we can make some useful assumptions.
		const std::vector<Primitive::Ptr>& primitives( m_renderqueue->GetPrimitives() );

		// Epic variable name
		int alpha_delta = static_cast<int>( static_cast<float>( background_color.a - color.a ) / ( steps - 1.f ) );
		int red_delta = static_cast<int>( static_cast<float>( background_color.r - color.r ) / ( steps - 1.f ) );
		int green_delta = static_cast<int>( static_cast<float>( background_color.g - color.g ) / ( steps - 1.f ) );
		int blue_delta = static_cast<int>( static_cast<float>( background_color.b - color.b ) / ( steps - 1.f ) );

		std::size_t primitives_size = primitives.size();

		// Make sure we don't bite ourselves in the tail.
		assert( primitives_size == static_cast<std::size_t>( steps ) );

		unsigned int current_stage = m_stage++;

		m_stage %= primitives_size;

		for( std::size_t index = primitives_size; index > 0; --index ) {
			Primitive* primitive = primitives[index - 1].get();

			std::vector<Primitive::Vertex>& vertices( primitive->GetVertices() );

			sf::Color new_color(
				static_cast<sf::Uint8>( color.r + red_delta * current_stage ),
				static_cast<sf::Uint8>( color.g + green_delta * current_stage ),
				static_cast<sf::Uint8>( color.b + blue_delta * current_stage ),
				static_cast<sf::Uint8>( color.a + alpha_delta * current_stage )
			);

			vertices[0].color = new_color;
			vertices[1].color = new_color;
			vertices[2].color = new_color;
			vertices[3].color = new_color;
			vertices[4].color = new_color;
			vertices[5].color = new_color;

			primitive->SetSynced( false );

			current_stage = ( current_stage + 1 ) % primitives_size;
		}

		Renderer::Get().InvalidateVBO();
	}
}

}
