#include <SFGUI/GLCanvas.hpp>
#include <SFGUI/Renderer.hpp>
#include <cmath>

namespace sfg {

GLCanvas::GLCanvas() :
	Widget(),
	m_custom_draw_callback( new Signal ),
	m_last_update( 0.f ),
	m_desired_refresh_rate( 0.f )
{
	m_custom_viewport = Renderer::Get().CreateViewport();
	SetViewport( m_custom_viewport );

	m_custom_draw_callback->Connect( &GLCanvas::ResetUpdateTime, this );
}

GLCanvas::Ptr GLCanvas::Create() {
	Ptr gl_canvas_ptr( new GLCanvas() );
	return gl_canvas_ptr;
}

RenderQueue* GLCanvas::InvalidateImpl() const {
	RenderQueue* queue( new RenderQueue );

	queue->Add(
		Renderer::Get().CreateGLCanvas(
			m_custom_draw_callback
		)
	);

	// Override whatever viewport our parents want us to use.
	// Yes... we are very naughty aren't we... in more than one way...
	const_cast<GLCanvas*>( this )->SetViewport( m_custom_viewport );

	return queue;
}

sf::Vector2f GLCanvas::CalculateRequisition() {
	return sf::Vector2f( 0.f, 0.f );
}

void GLCanvas::HandleSizeChange() {
	sf::FloatRect allocation = GetAllocation();

	m_custom_viewport->SetSize(
		sf::Vector2f(
			std::floor( allocation.width + .5f ),
			std::floor( allocation.height + .5f )
		)
	);

	Invalidate();
}

void GLCanvas::HandleAbsolutePositionChange() {
	sf::Vector2f position = Widget::GetAbsolutePosition();

	m_custom_viewport->SetDestinationOrigin(
		sf::Vector2f(
			std::floor( position.x + .5f ),
			std::floor( position.y + .5f )
		)
	);

	Invalidate();
}

const std::string& GLCanvas::GetName() const {
	static const std::string name( "GLCanvas" );
	return name;
}

Signal& GLCanvas::GetCustomDrawCallbackSignal() {
	return *m_custom_draw_callback;
}

void GLCanvas::HandleUpdate( float seconds ) {
	m_last_update += seconds;

	if( ( m_desired_refresh_rate > 0.f ) && ( m_last_update > ( 1.f / m_desired_refresh_rate ) ) ) {
		sfg::Renderer::Get().Redraw();
	}
}

void GLCanvas::SetDesiredRefreshRate( float rate ) {
	if( rate < 0.f ) {
		return;
	}

	m_desired_refresh_rate = rate;
}

void GLCanvas::ResetUpdateTime() {
	m_last_update = 0.f;
}

void GLCanvas::Redraw() {
	sfg::Renderer::Get().Redraw();
}

}
