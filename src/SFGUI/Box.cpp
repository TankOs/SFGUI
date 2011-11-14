#include <SFGUI/Box.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Box::Box( Orientation orientation, float spacing ) :
	Container(),
	m_orientation( orientation ),
	m_spacing( spacing )
{
}

Box::Ptr Box::Create( Orientation orientation, float spacing ) {
	Box::Ptr  ptr( new Box( orientation, spacing ) );
	return ptr;
}

const std::string& Box::GetName() const {
	static const std::string name( "Box" );
	return name;
}

void Box::Pack( Widget::Ptr widget, bool expand, bool fill ) {
	if( IsChild( widget ) ) {
		return;
	}

	// It's important to create the ChildInfo object first, so that the
	// HandleAdd() method recognized the widget as a correctly packed one.
	m_children.push_back( ChildInfo( widget, expand, fill ) );
	Add( widget );
}

void Box::HandleAdd( Widget::Ptr child ) {
	ChildrenCont::const_iterator  iter( std::find( m_children.begin(), m_children.end(), child ) );

	// If there's no ChildInfo present for the widget, the user added the widget
	// manually, which is not allowed for this class.
	if( iter == m_children.end() ) {

#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Child must be added via Pack() for sfg::Box widgets." << std::endl;
#endif

		Remove( child );
	}

	RequestSize();
	AllocateChildren();
}

void Box::HandleRemove( Widget::Ptr child ) {
	ChildrenCont::iterator  iter( std::find( m_children.begin(), m_children.end(), child ) );

	if( iter != m_children.end() ) {
		m_children.erase( iter );
	}

	AllocateChildren();
}

sf::Vector2f Box::GetRequisitionImpl() const {
	sf::Vector2f  requisition( 0, 0 );
	ChildrenCont::const_iterator  iter( m_children.begin() );
	ChildrenCont::const_iterator  iterend( m_children.end() );
	unsigned int  num_visible( 0 );

	for( ; iter != iterend; ++iter ) {
		if( iter->widget->IsVisible() ) {
			++num_visible;
		}

		sf::Vector2f  child_requisition( iter->widget->GetRequisition() );

		if( m_orientation == Horizontal ) {
			requisition.x += child_requisition.x + GetSpacing();
			requisition.y = std::max( requisition.y, child_requisition.y );
		}
		else {
			requisition.x = std::max( requisition.x, child_requisition.x );
			requisition.y += child_requisition.y + GetSpacing();
		}
	}

	if( num_visible > 0 ) {
		requisition.x += GetBorderWidth() * 2;
		requisition.y += GetBorderWidth() * 2;

		if( m_orientation == Horizontal ) {
			requisition.x -= GetSpacing();
		}
		else {
			requisition.y -= GetSpacing();
		}
	}

	AllocateChildren();

	return requisition;
}

void Box::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) const {
	AllocateChildren();
}

Box::ChildInfo::ChildInfo( Widget::Ptr widget_, bool expand_, bool fill_ ) :
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
	RequestSize();
	Invalidate();
}

float Box::GetSpacing() const {
	return m_spacing;
}

void Box::AllocateChildren() const {
	ChildrenCont::const_iterator  iter( m_children.begin() );
	ChildrenCont::const_iterator  iterend( m_children.end() );
	sf::Vector2f  allocation( 0.f, 0.f );
	sf::Vector2f  position( GetBorderWidth(), GetBorderWidth() );
	unsigned int  num_expand( 0 );
	unsigned int  num_visible( 0 );
	float  extra( 0.f );
	sf::Vector2f  requisition(
		m_orientation == Horizontal ? 2 * GetBorderWidth() : GetAllocation().Width,
		m_orientation == Vertical ? 2 * GetBorderWidth() : GetAllocation().Height
	);

	for( ; iter != iterend; ++iter ) {
		if( !iter->widget->IsVisible() ) {
			continue;
		}

		++num_visible;

		if( iter->expand ) {
			++num_expand;
		}

		sf::Vector2f  child_requisition( iter->widget->GetRequisition() );

		if( m_orientation == Horizontal ) {
			requisition.x += child_requisition.x;
			requisition.y = std::max( requisition.y, child_requisition.y + 2 * GetBorderWidth() );
		}
		else {
			requisition.x = std::max( requisition.x, child_requisition.x + 2 * GetBorderWidth() );
			requisition.y += child_requisition.y;
		}
	}

	// Add spacings.
	if( num_visible > 1 ) {
		if( m_orientation == Horizontal ) {
			requisition.x += static_cast<float>( num_visible - 1 ) * GetSpacing();
		}
		else {
			requisition.y += static_cast<float>( num_visible - 1 ) * GetSpacing();
		}
	}

	if( num_expand > 0 ) {
		if( m_orientation == Horizontal ) {
			extra = std::max( 0.f, GetAllocation().Width - requisition.x ) / static_cast<float>( num_expand );
		}
		else {
			extra = std::max( 0.f, GetAllocation().Height - requisition.y ) / static_cast<float>( num_expand );
		}
	}

	for( iter = m_children.begin(); iter != iterend; ++iter ) {
		if( !iter->widget->IsVisible() ) {
			continue;
		}

		if( m_orientation == Horizontal ) {
			allocation.x = iter->widget->GetRequisition().x + (iter->expand ? extra : 0.f);
			allocation.y = requisition.y - 2 * GetBorderWidth();

			iter->widget->AllocateSize( sf::FloatRect( position.x, position.y, allocation.x - (iter->expand && !iter->fill ? extra : 0.f), allocation.y ) );
			position.x += allocation.x + (num_visible > 1 ? GetSpacing() : 0.f);
		}
		else {
			allocation.x = requisition.x - 2 * GetBorderWidth();
			allocation.y = iter->widget->GetRequisition().y + (iter->expand ? extra : 0.f);

			iter->widget->AllocateSize( sf::FloatRect( position.x, position.y, allocation.x, allocation.y - (iter->expand && !iter->fill ? extra : 0.f) ) );
			position.y += allocation.y + (num_visible > 1 ? GetSpacing() : 0.f);
		}

		--num_visible;
	}
}

}
