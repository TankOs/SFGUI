#include <SFGUI/Scissor.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/OpenGL.hpp>

Scissor::Scissor( bool enable, const sf::FloatRect& viewport ) :
	m_viewport( viewport ),
	m_enable( enable ),
	m_prev_view( 0 )
{
}

void Scissor::Render( sf::RenderTarget& target, sf::Renderer& /*renderer*/ ) const {
	if( m_enable ) {
		glScissor(
			static_cast<unsigned int>( m_viewport.Left ),
			target.GetHeight() - static_cast<unsigned int>( m_viewport.Top ),
			static_cast<unsigned int>( m_viewport.Width ),
			static_cast<unsigned int>( m_viewport.Height )
		);
		//glEnable( GL_SCISSOR_TEST );
	}
	else {
		//glDisable( GL_SCISSOR_TEST );
	}
}
