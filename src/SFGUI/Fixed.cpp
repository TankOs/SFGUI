#include <SFGUI/Fixed.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Fixed::Fixed() :
	Container()
{
}

Fixed::Ptr Fixed::Create() {
	Fixed::Ptr ptr( new Fixed );

	return ptr;
}

sf::Vector2f Fixed::CalculateRequisition() {
	ChildrenPositionMap::const_iterator child_iter( m_children_position_map.begin() );
	ChildrenPositionMap::const_iterator child_iter_end( m_children_position_map.end() );

	sf::Vector2f requisition( 0.f, 0.f );

	// Find size needed to accomodate all widgets without overlapping any other widgets
	for( ; child_iter != child_iter_end; ++ child_iter ) {
		requisition.x = std::max( requisition.x, child_iter->second.x + child_iter->first->GetRequisition().x );
		requisition.y = std::max( requisition.y, child_iter->second.y + child_iter->first->GetRequisition().y );
	}

	return requisition;
}

const std::string& Fixed::GetName() const {
	static const std::string name( "Fixed" );
	return name;
}

void Fixed::Put( const Widget::Ptr& widget, const sf::Vector2f& position ) {
	m_children_position_map[ widget ] = position;

	Container::Add( widget );
}

void Fixed::Move( const Widget::Ptr& widget, const sf::Vector2f& position ) {
	if( m_children_position_map.find( widget ) != m_children_position_map.end() ) {
		m_children_position_map[ widget ] = position;

		widget->SetAllocation( sf::FloatRect( position, widget->GetRequisition() ) );

		RequestResize();
	}
}

void Fixed::HandleAdd( const Widget::Ptr& child ) {
	Container::HandleAdd( child );

	// If there's no position info present for the widget, the user added the widget
	// manually, which is not allowed for this class.
	if( m_children_position_map.find( child ) == m_children_position_map.end() ) {

#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Child must be added via Put() for sfg::Fixed widgets.\n";
#endif

		Remove( child );
	} else {
		child->SetAllocation( sf::FloatRect( m_children_position_map[ child ], child->GetRequisition() ) );

		RequestResize();
	}
}

void Fixed::HandleRemove( const Widget::Ptr& child ) {
	m_children_position_map.erase( child );

	RequestResize();
}

}
