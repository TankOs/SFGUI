#include <SFGUI/Box.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderEngine.hpp>
#include <iostream>

namespace sfg {

Box::Box( Orientation orientation, float padding ) :
	Container(),
	m_orientation( orientation ),
	m_padding( padding )
{
	OnAdd.Connect( &Box::HandleAdd, this );
	OnRemove.Connect( &Box::HandleRemove, this );
	OnSizeAllocate.Connect( &Box::HandleSizeAllocate, this );
}

Box::Ptr Box::Create( Orientation orientation, float padding ) {
	Box::Ptr  ptr( new Box( orientation, padding ) );
	return ptr;
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

void Box::HandleAdd( Widget::Ptr /*widget*/, Widget::Ptr child ) {
	ChildrenCont::const_iterator  iter( std::find( m_children.begin(), m_children.end(), child ) );

	// If there's no ChildInfo present for the widget, the user added the widget
	// manually, which is not allowed for this class.
	// TODO: Add ChildInfo with default settings.
	if( iter == m_children.end() ) {
		std::cerr << "SFGUI warning: Child must be added via Pack() for sfg::Box widgets." << std::endl;
		Remove( child );
		return;
	}

	QueueResize( child );
}

void Box::HandleRemove( Widget::Ptr /*widget*/, Widget::Ptr child ) {
	ChildrenCont::iterator  iter( std::find( m_children.begin(), m_children.end(), child ) );

	if( iter != m_children.end() ) {
		m_children.erase( iter );
	}
}

void Box::QueueResize( Widget::Ptr widget ) {
	if( widget != shared_from_this() && !IsChild( widget ) ) {
		return;
	}

	//RequestSize( AllocateChildrenSizes() );
	ChildrenCont::iterator  iter( m_children.begin() );
	ChildrenCont::iterator  iterend( m_children.end() );
	sf::Vector2f  requisition( 0.f, 0.f );
	unsigned int  num_visible( 0 );

	for( ; iter != iterend; ++iter ) {
		if( iter->widget->IsVisible() ) {
			++num_visible;

			if( m_orientation == Horizontal ) {
				requisition.x += iter->widget->GetRequisition().x + GetPadding();
				requisition.y = std::max( requisition.y, iter->widget->GetRequisition().y );
			}
			else {
				requisition.x = std::max( requisition.x, iter->widget->GetRequisition().x );
				requisition.y += iter->widget->GetRequisition().y + GetPadding();
			}
		}
	}

	if( num_visible > 0 ) {
		requisition.x += GetBorderWidth() * 2;
		requisition.y += GetBorderWidth() * 2;
	}
	
	RequestSize( requisition );
}

sf::Vector2f Box::AllocateChildrenSizes() {
	ChildrenCont::iterator  iter( m_children.begin() );
	ChildrenCont::iterator  iterend( m_children.end() );
	sf::Vector2f  allocation( 0.f, 0.f );
	sf::Vector2f  position( GetAllocation().Left + GetBorderWidth(), GetAllocation().Top + GetBorderWidth() );
	unsigned int  num_expand( 0 );
	unsigned int  num_visible( 0 );
	float  extra( 0.f );
	sf::Vector2f  requisition(
		m_orientation == Horizontal ? 2 * GetBorderWidth() : GetAllocation().Width,
		m_orientation == Vertical ? 2 * GetBorderWidth() : GetAllocation().Height
	);

	for( ; iter != iterend; ++iter ) {
		if( iter->expand ) {
			++num_expand;
		}

		if( iter->widget->IsVisible() ) {
			++num_visible;
		}

		if( m_orientation == Horizontal ) {
			requisition.x += iter->widget->GetRequisition().x;
			requisition.y = std::max( requisition.y, iter->widget->GetRequisition().y + 2 * GetBorderWidth() );
		}
		else {
			requisition.x = std::max( requisition.x, iter->widget->GetRequisition().x + 2 * GetBorderWidth() );
			requisition.y += iter->widget->GetRequisition().y;
		}
	}

	// Add paddings.
	if( num_visible > 1 ) {
		if( m_orientation == Horizontal ) {
			requisition.x += static_cast<float>( num_visible - 1 ) * GetPadding();
		}
		else {
			requisition.y += static_cast<float>( num_visible - 1 ) * GetPadding();
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
		if( m_orientation == Horizontal ) {
			allocation.x = iter->widget->GetRequisition().x + (iter->expand ? extra : 0.f);
			allocation.y = requisition.y - 2 * GetBorderWidth();

			iter->widget->AllocateSize( sf::FloatRect( position.x, position.y, allocation.x - (iter->expand && !iter->fill ? extra : 0.f), allocation.y ) );
			position.x += allocation.x + (num_visible > 1 ? GetPadding() : 0.f);
		}
		else {
			allocation.x = requisition.x - 2 * GetBorderWidth();
			allocation.y = iter->widget->GetRequisition().y + (iter->expand ? extra : 0.f);

			iter->widget->AllocateSize( sf::FloatRect( position.x, position.y, allocation.x, allocation.y - (iter->expand && !iter->fill ? extra : 0.f) ) );
			position.y += allocation.y + (num_visible > 1 ? GetPadding() : 0.f);
		}

		--num_visible;
	}

	return requisition;
}

void Box::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& /*oldallocation*/ ) {
	AllocateChildrenSizes();
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

void Box::SetPadding( float padding ) {
	m_padding = padding;
	QueueResize( shared_from_this() );
	Invalidate();
}

float Box::GetPadding() const {
	return m_padding;
}

}
