#include <SFGUI/ProgressBar.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ProgressBar::ProgressBar( Orientation orientation ) :
	m_fraction( 0.f ),
	m_orientation( orientation )
{
}

ProgressBar::Ptr ProgressBar::Create( Orientation orientation ) {
	return Ptr( new ProgressBar( orientation ) );
}

void ProgressBar::SetFraction( float fraction ) {
	m_fraction = std::max( 0.f, std::min( 1.f, fraction ) );
	Invalidate();
}

float ProgressBar::GetFraction() const {
	return m_fraction;
}

const std::string& ProgressBar::GetName() const {
	static const std::string name( "ProgressBar" );
	return name;
}

sf::Vector2f ProgressBar::CalculateRequisition() {
	if( m_orientation == Orientation::HORIZONTAL ) {
		return sf::Vector2f( 20.f, 10.f );
	}

	return sf::Vector2f( 10.f, 20.f );
}

std::unique_ptr<RenderQueue> ProgressBar::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateProgressBarDrawable( std::dynamic_pointer_cast<const ProgressBar>( shared_from_this() ) );
}

void ProgressBar::SetOrientation( Orientation orientation ) {
	m_orientation = orientation;
	RequestResize();
}

ProgressBar::Orientation ProgressBar::GetOrientation() const {
	return static_cast<Orientation>( m_orientation );
}

}
