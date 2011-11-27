#include <SFGUI/Separator.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Separator::Separator( Orientation orientation ) :
	m_orientation( orientation )
{
}

Separator::Ptr Separator::Create( Orientation orientation ) {
	Separator::Ptr ptr( new Separator( orientation ) );
	return ptr;
}

Separator::Orientation Separator::GetOrientation() const {
	return m_orientation;
}


RenderQueue* Separator::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateSeparatorDrawable( DynamicPointerCast<const Separator>( shared_from_this() ) );
}

sf::Vector2f Separator::CalculateRequisition() {
	return sf::Vector2f( 2.f, 2.f );
}

const std::string& Separator::GetName() const {
	static const std::string name( "Separator" );
	return name;
}

}
