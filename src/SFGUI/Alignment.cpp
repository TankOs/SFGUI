#include <SFGUI/Alignment.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Alignment::Alignment() :
	Bin(),
	Misc(),
	m_scale( sf::Vector2f( 1.f, 1.f ) )
{
	SetAlignment( sf::Vector2f( 0.f, 0.f ) );
}

Alignment::Ptr Alignment::Create() {
	Alignment::Ptr ptr( new Alignment );

	return ptr;
}

void Alignment::SetScale( const sf::Vector2f& scale ) {
	m_scale = scale;
	UpdateChild();
}

const sf::Vector2f& Alignment::GetScale() const {
	return m_scale;
}

sf::Vector2f Alignment::CalculateRequisition() {
	Widget::Ptr child = GetChild();

	if( !child ) {
		return sf::Vector2f( 0.f, 0.f );
	}

	return child->GetRequisition();
}

const std::string& Alignment::GetName() const {
	static const std::string name( "Alignment" );
	return name;
}

void Alignment::HandleSizeChange() {
	UpdateChild();
}

void Alignment::HandleAlignmentChange( const sf::Vector2f& /*old_alignment*/ ) {
	UpdateChild();
}

void Alignment::UpdateChild() {
	Widget::Ptr child = GetChild();

	if( !child ) {
		return;
	}

	sf::FloatRect allocation( GetAllocation() );

	sf::Vector2f spare_space( allocation.width, allocation.height );
	spare_space -= child->GetRequisition();
	spare_space.x *= 1.f - GetScale().x;
	spare_space.y *= 1.f - GetScale().y;

	if( ( spare_space.x < 0 ) || ( spare_space.y < 0 ) ) {
#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Alignment got a smaller allocation than it requested.\n";
		return;
#endif
	}

	allocation.left = spare_space.x * GetAlignment().x;
	allocation.top = spare_space.y * GetAlignment().y;
	allocation.width -= spare_space.x;
	allocation.height -= spare_space.y;

	child->SetAllocation( allocation );
}

}
