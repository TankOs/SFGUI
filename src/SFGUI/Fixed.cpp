#include <SFGUI/Fixed.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Fixed::Ptr Fixed::Create() {
	return std::make_shared<Fixed>();
}

sf::Vector2f Fixed::CalculateRequisition() {
	sf::Vector2f requisition( 0.f, 0.f );

	// Find size needed to accomodate all widgets without overlapping any other widgets
	for( const auto& child_position : m_children_position_map ) {
		requisition.x = std::max( requisition.x, child_position.second.x + child_position.first->GetRequisition().x );
		requisition.y = std::max( requisition.y, child_position.second.y + child_position.first->GetRequisition().y );
	}

	return requisition;
}

const std::string& Fixed::GetName() const {
	static const std::string name( "Fixed" );
	return name;
}

void Fixed::Put( Widget::Ptr widget, const sf::Vector2f& position ) {
	m_children_position_map[ widget ] = position;

	Container::Add( widget );
}

void Fixed::Move( Widget::Ptr widget, const sf::Vector2f& position ) {
	if( m_children_position_map.find( widget ) != m_children_position_map.end() ) {
		m_children_position_map[ widget ] = position;

		widget->SetAllocation( sf::FloatRect( position, widget->GetRequisition() ) );

		RequestResize();
	}
}

void Fixed::HandleAdd( Widget::Ptr child ) {
	Container::HandleAdd( child );

	// If there's no position info present for the widget, the user added the widget
	// manually, which is not allowed for this class.
	if( m_children_position_map.find( child ) == m_children_position_map.end() ) {

#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Child must be added via Put() for sfg::Fixed widgets.\n";
#endif

		Remove( child );
	} else {
		child->SetAllocation( sf::FloatRect( m_children_position_map[ child ], child->GetRequisition() ) );

		RequestResize();
	}
}

void Fixed::HandleRemove( Widget::Ptr child ) {
	m_children_position_map.erase( child );

	RequestResize();
}

}
