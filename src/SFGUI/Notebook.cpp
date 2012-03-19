#include <SFGUI/Notebook.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Notebook::Notebook() :
	Container(),
	m_tab_requisition( 0.f, 0.f ),
	m_child_requisition( 0.f, 0.f ),
	m_current_page( 0 ),
	m_prelight_tab( -1 ),
	m_tab_position( TOP )
{
	SetCurrentPage( 0 );
}

Notebook::Ptr Notebook::Create() {
	Notebook::Ptr ptr( new Notebook );

	return ptr;
}

RenderQueue* Notebook::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateNotebookDrawable( DynamicPointerCast<const Notebook>( shared_from_this() ) );
}

sf::Vector2f Notebook::CalculateRequisition() {
	if( !GetPageCount() ) {
		return sf::Vector2f( 0.f, 0.f );
	}

	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	sf::Vector2f tab_requisition( 0.f, 0.f );
	sf::Vector2f child_requisition( 0.f, 0.f );

	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		if( ( GetTabPosition() == TOP ) || ( GetTabPosition() == BOTTOM ) ) {
			// Tabs are positioned horizontally.
			tab_requisition.x += ( m_children[index].tab_label->GetRequisition().x + 2.f * padding + border_width );
			tab_requisition.y = std::max( tab_requisition.y, m_children[index].tab_label->GetRequisition().y );
		}
		else {
			// Tabs are positioned vertically.
			tab_requisition.x = std::max( tab_requisition.x, m_children[index].tab_label->GetRequisition().x );
			tab_requisition.y += ( m_children[index].tab_label->GetRequisition().y + 2.f * padding + border_width );
		}

		child_requisition.x = std::max( child_requisition.x, m_children[index].child->GetRequisition().x );
		child_requisition.y = std::max( child_requisition.y, m_children[index].child->GetRequisition().y );
	}

	sf::Vector2f requisition( 0.f, 0.f );

	child_requisition.x += 2.f * ( padding + border_width );
	child_requisition.y += 2.f * ( padding + border_width );

	if( ( GetTabPosition() == TOP ) || ( GetTabPosition() == BOTTOM ) ) {
		// Tabs are positioned horizontally.
		tab_requisition.x += border_width;
		tab_requisition.y += ( border_width + 2.f * padding );

		requisition.x = std::max( tab_requisition.x, child_requisition.x );
		requisition.y = tab_requisition.y + child_requisition.y;
	}
	else {
		// Tabs are positioned vertically.
		tab_requisition.x += ( border_width + 2.f * padding );
		tab_requisition.y += border_width;

		requisition.x = tab_requisition.x + child_requisition.x;
		requisition.y = std::max( tab_requisition.y, child_requisition.y );
	}

	m_child_requisition = child_requisition;
	m_tab_requisition = tab_requisition;

	return requisition;
}

const std::string& Notebook::GetName() const {
	static const std::string name( "Notebook" );
	return name;
}

Notebook::IndexType Notebook::AppendPage( const Widget::Ptr& child, const Widget::Ptr& tab_label ) {
	return InsertPage( child, tab_label, -1 );
}

Notebook::IndexType Notebook::PrependPage( const Widget::Ptr& child, const Widget::Ptr& tab_label ) {
	return InsertPage( child, tab_label, 0 );
}

Notebook::IndexType Notebook::InsertPage( const Widget::Ptr& child, const Widget::Ptr& tab_label, IndexType position ) {
	if( ( position >= GetPageCount() ) || ( position < 0 ) ) {
		m_children.push_back( ChildLabelPair( child, tab_label ) );

		Add( child );
		Add( tab_label );

		child->Show( false );

		SetCurrentPage( GetCurrentPage() );

		return GetPageCount() - 1;
	}

	m_children.insert( m_children.begin() + position, ChildLabelPair( child, tab_label ) );

	Add( child );
	Add( tab_label );

	child->Show( false );

	// If the current page was after or at the position of the insertion,
	// make sure it stays active.
	if( GetCurrentPage() >= position ) {
		NextPage();
	}

	return position;
}

void Notebook::RemovePage( IndexType page_number ) {
	if( ( page_number < 0 ) || ( page_number >= GetPageCount() ) ) {
		m_children.pop_back();
	}

	m_children.erase( m_children.begin() + page_number );
}

Notebook::IndexType Notebook::GetPageOf( const Widget::Ptr& widget ) const {
	ChildrenList::const_iterator iter( std::find( m_children.begin(), m_children.end(), ChildLabelPair( widget, Widget::Ptr() ) ) );

	if( iter == m_children.end() ) {
		return -1;
	}

	return static_cast<IndexType>( iter - m_children.begin() );
}

void Notebook::NextPage() {
	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[m_current_page].child->Show( false );
	}

	m_current_page = std::min( m_current_page + 1, GetPageCount() - 1 );

	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[m_current_page].child->Show( true );
	}

	Invalidate();
}

void Notebook::PreviousPage() {
	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[m_current_page].child->Show( false );
	}

	m_current_page = std::max( m_current_page - 1, 0 );

	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[m_current_page].child->Show( true );
	}

	Invalidate();
}

void Notebook::ReorderChild( const Widget::Ptr& child, IndexType position ) {
	Widget::Ptr tab_label = GetTabLabel( child );

	if( !tab_label ) {
		return;
	}

	IndexType old_page_number = GetPageOf( child );

	RemovePage( old_page_number );
	InsertPage( child, tab_label, position - ( old_page_number > position ? 1 : 0 ) );
}

Notebook::TabPosition Notebook::GetTabPosition() const {
	return static_cast<TabPosition>( m_tab_position );
}

void Notebook::SetTabPosition( TabPosition tab_position ) {
	m_tab_position = static_cast<unsigned char>( tab_position );
}

Notebook::IndexType Notebook::GetCurrentPage() const {
	return m_current_page;
}

const Widget::Ptr Notebook::GetNthPage( IndexType page_number ) const {
	if( ( page_number >= 0 ) && ( page_number < GetPageCount() ) ) {
		return m_children[page_number].child;
	}

	return Widget::Ptr();
}

const Widget::Ptr Notebook::GetNthTabLabel( IndexType page_number ) const {
	if( ( page_number >= 0 ) && ( page_number < GetPageCount() ) ) {
		return m_children[page_number].tab_label;
	}

	return Widget::Ptr();
}

Notebook::IndexType Notebook::GetPageCount() const {
	return static_cast<IndexType>( m_children.size() );
}

const Widget::Ptr Notebook::GetTabLabel( const Widget::Ptr& child ) const {
	IndexType page_number = GetPageOf( child );

	if( page_number < 0 ) {
		return Widget::Ptr();
	}

	return m_children[page_number].tab_label;
}

void Notebook::SetTabLabel( const Widget::Ptr& child, const Widget::Ptr& tab_label ) {
	IndexType page_number = GetPageOf( child );

	if( page_number < 0 ) {
		return;
	}

	m_children[page_number].tab_label = tab_label;
}

void Notebook::SetCurrentPage( IndexType page_number ) {
	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[m_current_page].child->Show( false );
	}

	if( ( page_number >= 0 ) && ( page_number < GetPageCount() ) ) {
		m_current_page = page_number;
	}
	else {
		m_current_page = GetPageCount() - 1;
	}

	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[m_current_page].child->Show( true );
	}

	Invalidate();
}

Notebook::IndexType Notebook::GetPrelightTab() const {
	return m_prelight_tab;
}

Notebook::ChildLabelPair::ChildLabelPair( const Widget::Ptr& child_, const Widget::Ptr& tab_label_ ) :
	child( child_ ),
	tab_label( tab_label_ )
{
}

bool Notebook::ChildLabelPair::operator==( const ChildLabelPair& rhs ) const {
	return child == rhs.child;
}

void Notebook::HandleMouseMoveEvent( int x, int y ) {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );

	std::size_t children_size = m_children.size();

	IndexType m_old_prelight_tab = m_prelight_tab;
	m_prelight_tab = -1;

	for( std::size_t index = 0; index < children_size; ++index ) {
		sf::FloatRect allocation = m_children[index].tab_label->GetAllocation();

		allocation.left -= padding;
		allocation.top -= padding;
		allocation.width += 2.f * padding;
		allocation.height += 2.f * padding;

		if( allocation.contains( sf::Vector2f( static_cast<float>( x ) - GetAllocation().left, static_cast<float>( y ) - GetAllocation().top ) ) ) {
			m_prelight_tab = static_cast<IndexType>( index );
			break;
		}
	}

	if( m_old_prelight_tab != m_prelight_tab ) {
		Invalidate();
	}
}

void Notebook::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( ( button != sf::Mouse::Left ) || !press || ( m_prelight_tab < 0 ) ) {
		return;
	}

	SetCurrentPage( m_prelight_tab );
}

void Notebook::HandleAdd( const Widget::Ptr& child ) {
	Container::HandleAdd( child );

	bool allowed = false;

	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		if( ( m_children[index].child == child ) || ( m_children[index].tab_label == child ) ) {
			allowed = true;
			break;
		}
	}

	// If the child or label isn't found on the children list, the user added the widget
	// manually, which is not allowed for this class.
	if( !allowed ) {

#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Child must be added via AppendPage() or PrependPage() or InsertPage() for sfg::Notebook widgets." << std::endl;
#endif

		Remove( child );
	}

	RequestResize();
	Invalidate();
}

void Notebook::HandleRemove( const Widget::Ptr& child ) {
	IndexType page_number = GetPageOf( child );

	if( page_number >= 0 ) {
		m_children.erase( m_children.begin() + GetPageOf( child ) );
	}

	RequestResize();
	Invalidate();
}

void Notebook::HandleAllocationChange( const sf::FloatRect& /*old_allocation*/ ) {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	std::size_t children_size = m_children.size();

	if( GetTabPosition() == TOP ) {
		// Tabs are positioned at top.
		float tab_current_x = 0.f;

		for( std::size_t index = 0; index < children_size; ++index ) {
			m_children[index].child->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					m_tab_requisition.y + border_width + padding,
					GetAllocation().width - 2.f * ( border_width + padding ),
					GetAllocation().height - ( 2.f * ( border_width + padding ) + m_tab_requisition.y )
				)
			);

			m_children[index].tab_label->SetAllocation(
				sf::FloatRect(
					tab_current_x + border_width + padding,
					border_width + padding,
					m_children[index].tab_label->GetRequisition().x,
					m_tab_requisition.y - 2.f * padding - border_width
				)
			);

			tab_current_x += ( border_width + 2.f * padding + m_children[index].tab_label->GetRequisition().x );
		}
	}
	else if( GetTabPosition() == BOTTOM ) {
		// Tabs are positioned at bottom.
		float tab_current_x = 0.f;

		for( std::size_t index = 0; index < children_size; ++index ) {
			m_children[index].child->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					border_width + padding,
					GetAllocation().width - 2.f * ( border_width + padding ),
					GetAllocation().height - ( 2.f * ( border_width + padding ) + m_tab_requisition.y )
				)
			);

			m_children[index].tab_label->SetAllocation(
				sf::FloatRect(
					tab_current_x + border_width + padding,
					GetAllocation().height - m_tab_requisition.y + padding,
					m_children[index].tab_label->GetRequisition().x,
					m_tab_requisition.y - 2.f * padding - border_width
				)
			);

			tab_current_x += ( border_width + 2.f * padding + m_children[index].tab_label->GetRequisition().x );
		}
	}
	else if( GetTabPosition() == LEFT ) {
		// Tabs are positioned at left.
		float tab_current_y = 0.f;

		for( std::size_t index = 0; index < children_size; ++index ) {
			m_children[index].child->SetAllocation(
				sf::FloatRect(
					m_tab_requisition.x + border_width + padding,
					border_width + padding,
					GetAllocation().width - ( 2.f * ( border_width + padding ) + m_tab_requisition.x ),
					GetAllocation().height - 2.f * ( border_width + padding )
				)
			);

			m_children[index].tab_label->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					tab_current_y + border_width + padding,
					m_tab_requisition.x - 2.f * padding - border_width,
					m_children[index].tab_label->GetRequisition().y
				)
			);

			tab_current_y += ( border_width + 2.f * padding + m_children[index].tab_label->GetRequisition().y );
		}
	}
	else if( GetTabPosition() == RIGHT ) {
		// Tabs are positioned at right.
		float tab_current_y = 0.f;

		for( std::size_t index = 0; index < children_size; ++index ) {
			m_children[index].child->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					border_width + padding,
					GetAllocation().width - ( 2.f * ( border_width + padding ) + m_tab_requisition.x ),
					GetAllocation().height - 2.f * ( border_width + padding )
				)
			);

			m_children[index].tab_label->SetAllocation(
				sf::FloatRect(
					GetAllocation().width - m_tab_requisition.x + padding,
					tab_current_y + border_width + padding,
					m_tab_requisition.x - 2.f * padding - border_width,
					m_children[index].tab_label->GetRequisition().y
				)
			);

			tab_current_y += ( border_width + 2.f * padding + m_children[index].tab_label->GetRequisition().y );
		}
	}

	Invalidate();
}

}
