#include <SFGUI/Box.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Box::Box( Orientation orientation, float spacing ) :
	Container(),
	m_spacing( spacing ),
	m_orientation( orientation )
{
}

Box::Ptr Box::Create( Orientation orientation, float spacing ) {
	Box::Ptr ptr( new Box( orientation, spacing ) );
	return ptr;
}

const std::string& Box::GetName() const {
	static const std::string name( "Box" );
	return name;
}

void Box::Pack( const Widget::Ptr& widget, bool expand, bool fill ) {
	if( IsChild( widget ) ) {
		return;
	}

	// It's important to create the ChildInfo object first, so that the
	// HandleAdd() method recognized the widget as a correctly packed one.
	m_children.push_back( ChildInfo( widget, expand, fill ) );
	Add( widget );
}

void Box::HandleAdd( const Widget::Ptr& child ) {
	ChildrenCont::const_iterator iter( std::find( m_children.begin(), m_children.end(), child ) );

	// If there's no ChildInfo present for the widget, the user added the widget
	// manually, which is not allowed for this class.
	if( iter == m_children.end() ) {

#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Child must be added via Pack() for sfg::Box widgets." << std::endl;
#endif

		Remove( child );
	}

	RequestResize();
	AllocateChildren();
}

void Box::HandleRemove( const Widget::Ptr& child ) {
	ChildrenCont::iterator iter( std::find( m_children.begin(), m_children.end(), child ) );

	if( iter != m_children.end() ) {
		m_children.erase( iter );
	}

	AllocateChildren();
}

sf::Vector2f Box::CalculateRequisition() {
	sf::Vector2f requisition( 0.f, 0.f );
	unsigned int num_visible( 0 );
	ChildrenCont::const_iterator iter( m_children.begin() );
	ChildrenCont::const_iterator iterend( m_children.end() );

	for( ; iter != iterend; ++iter ) {
		if( !IsChildInteresting( iter->widget ) ) {
			continue;
		}

		++num_visible;

		sf::Vector2f child_requisition( iter->widget->GetRequisition() );

		if( m_orientation == HORIZONTAL ) {
			requisition.x += child_requisition.x;
			requisition.y = std::max( requisition.y, child_requisition.y );
		}
		else {
			requisition.x = std::max( requisition.x, child_requisition.x );
			requisition.y += child_requisition.y;
		}
	}

	if( num_visible > 1 ) {
		if( m_orientation == HORIZONTAL ) {
			requisition.x += static_cast<float>( num_visible - 1 ) * GetSpacing();
		}
		else {
			requisition.y += static_cast<float>( num_visible - 1 ) * GetSpacing();
		}
	}

	requisition.x += 2 * GetMargin();
	requisition.y += 2 * GetMargin();

	return requisition;
}

void Box::HandleAllocationChange( const sf::FloatRect& /*old_allocation*/ ) {
	AllocateChildren();
}

Box::ChildInfo::ChildInfo( const Widget::Ptr& widget_, bool expand_, bool fill_ ) :
	widget( widget_ ),
	expand( expand_ ),
	fill( fill_ )
{
}

bool Box::ChildInfo::operator==( const ChildInfo& rhs ) const {
	return widget == rhs.widget;
}

void Box::SetSpacing( float spacing ) {
	m_spacing = spacing;
	RequestResize();
	Invalidate();
}

float Box::GetSpacing() const {
	return m_spacing;
}

void Box::AllocateChildren() const {
	ChildrenCont::const_iterator iter( m_children.begin() );
	ChildrenCont::const_iterator iterend( m_children.end() );
	unsigned int num_expand( 0 );
	unsigned int num_visible( 0 );

	// Count number of visible and expanded children.
	for( ; iter != iterend; ++iter ) {
		if( !IsChildInteresting( iter->widget ) ) {
			continue;
		}

		++num_visible;

		if( iter->expand ) {
			++num_expand;
		}
	}

	// Calculate extra width pre expanded widget.
	float extra( 0.f );

	if( num_expand > 0 ) {
		if( m_orientation == HORIZONTAL ) {
			extra = std::max( 0.f, GetAllocation().Width - GetRequisition().x ) / static_cast<float>( num_expand );
		}
		else {
			extra = std::max( 0.f, GetAllocation().Height - GetRequisition().y ) / static_cast<float>( num_expand );
		}
	}

	// Allocate children.
	sf::Vector2f allocation( 0.f, 0.f );
	sf::Vector2f position( GetMargin(), GetMargin() );

	for( iter = m_children.begin(); iter != iterend; ++iter ) {
		if( !IsChildInteresting( iter->widget ) ) {
			continue;
		}

		if( m_orientation == HORIZONTAL ) {
			allocation.x = iter->widget->GetRequisition().x + (iter->expand ? extra : 0.f);
			allocation.y = GetAllocation().Height - 2 * GetMargin();

			iter->widget->SetAllocation( sf::FloatRect( position.x, position.y, allocation.x - (iter->expand && !iter->fill ? extra : 0.f), allocation.y ) );
			position.x += allocation.x + GetSpacing();
		}
		else {
			allocation.x = GetAllocation().Width - 2 * GetMargin();
			allocation.y = iter->widget->GetRequisition().y + (iter->expand ? extra : 0.f);

			iter->widget->SetAllocation( sf::FloatRect( position.x, position.y, allocation.x, allocation.y - (iter->expand && !iter->fill ? extra : 0.f) ) );
			position.y += allocation.y + GetSpacing();
		}

		--num_visible;
	}
}

bool Box::IsChildInteresting( const sfg::Widget::PtrConst& child ) const {
	return
		child->IsVisible() &&
		(child->GetRequisition().x > 0.f || child->GetAllocation().Width > 0.0f) &&
		(child->GetRequisition().y > 0.f || child->GetAllocation().Height > 0.0f)
	;
}

void Box::HandleRequisitionChange() {
	AllocateChildren();
}

}
