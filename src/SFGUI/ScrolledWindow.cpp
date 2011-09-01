#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ScrolledWindow::ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) :
	Bin(),
	m_horizontal_scrollbar(),
	m_vertical_scrollbar()
{
	m_horizontal_scrollbar = Scrollbar::Create( horizontal_adjustment, Scrollbar::Horizontal );
	m_vertical_scrollbar = Scrollbar::Create( vertical_adjustment, Scrollbar::Vertical );

	m_policies.horizontal_policy = Always;
	m_policies.vertical_policy = Always;

	m_placement = TopLeft;
}

ScrolledWindow::Ptr ScrolledWindow::Create() {
	ScrolledWindow::Ptr  ptr( new ScrolledWindow( Adjustment::Create(), Adjustment::Create() ) );

	return ptr;
}

ScrolledWindow::Ptr ScrolledWindow::Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) {
	ScrolledWindow::Ptr  ptr( new ScrolledWindow( horizontal_adjustment, vertical_adjustment ) );

	return ptr;
}

Adjustment::Ptr ScrolledWindow::GetHorizontalAdjustment() const {
	return m_horizontal_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetHorizontalAdjustment( Adjustment::Ptr adjustment ) {
	m_horizontal_scrollbar->SetAdjustment( adjustment );
}

Adjustment::Ptr ScrolledWindow::GetVerticalAdjustment() const {
	return m_vertical_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetVerticalAdjustment( Adjustment::Ptr adjustment ) {
	m_vertical_scrollbar->SetAdjustment( adjustment );
}

const ScrolledWindow::ScrollbarPolicyPair& ScrolledWindow::GetScrollbarPolicies() const {
	return m_policies;
}

void ScrolledWindow::SetScrollbarPolicies( const ScrolledWindow::ScrollbarPolicyPair& policies ) {
	m_policies = policies;
}

void ScrolledWindow::SetPlacement( Placement placement ) {
	m_placement = placement;
}

sf::Vector2f ScrolledWindow::GetRequisitionImpl() const {
	sf::Vector2f requisition( 0.f, 0.f );

	return requisition;
}

}
