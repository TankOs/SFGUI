#include <SFGUI/Notebook.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Notebook::Notebook() :
	m_current_page( 0 ),
	m_prelight_tab( -1 ),
	m_first_tab( 0 ),
	m_num_displayed_tabs( 0 ),
	m_tab_position( TabPosition::TOP ),
	m_elapsed_time( 0.f ),
	m_scrollable( false ),
	m_scrolling_forward( false ),
	m_scrolling_backward( false ),
	m_forward_scroll_prelight( false ),
	m_backward_scroll_prelight( false )
{
	SetCurrentPage( 0 );
}

Notebook::Ptr Notebook::Create() {
	return Ptr( new Notebook );
}

std::unique_ptr<RenderQueue> Notebook::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateNotebookDrawable( std::dynamic_pointer_cast<const Notebook>( shared_from_this() ) );
}

sf::Vector2f Notebook::CalculateRequisition() {
	if( !GetPageCount() ) {
		return sf::Vector2f( 0.f, 0.f );
	}

	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	sf::Vector2f tab_requisition( 0.f, 0.f );
	sf::Vector2f child_requisition( 0.f, 0.f );

	for( const auto& child : m_children ) {
		if( ( GetTabPosition() == TabPosition::TOP ) || ( GetTabPosition() == TabPosition::BOTTOM ) ) {
			// Tabs are positioned horizontally.
			tab_requisition.x += ( child.tab_label->GetRequisition().x + 2.f * padding + border_width );
			tab_requisition.y = std::max( tab_requisition.y, child.tab_label->GetRequisition().y );
		}
		else {
			// Tabs are positioned vertically.
			tab_requisition.x = std::max( tab_requisition.x, child.tab_label->GetRequisition().x );
			tab_requisition.y += ( child.tab_label->GetRequisition().y + 2.f * padding + border_width );
		}

		child_requisition.x = std::max( child_requisition.x, child.child->GetRequisition().x );
		child_requisition.y = std::max( child_requisition.y, child.child->GetRequisition().y );
	}

	sf::Vector2f requisition( 0.f, 0.f );

	child_requisition.x += 2.f * ( padding + border_width );
	child_requisition.y += 2.f * ( padding + border_width );

	if( ( GetTabPosition() == TabPosition::TOP ) || ( GetTabPosition() == TabPosition::BOTTOM ) ) {
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

	if( GetScrollable() ) {
		if( ( GetTabPosition() == TabPosition::TOP ) || ( GetTabPosition() == TabPosition::BOTTOM ) ) {
			m_tab_requisition.x = 0.f;
			requisition.x = m_child_requisition.x + 2.f * ( padding + border_width );
		}
		else {
			m_tab_requisition.y = 0.f;
			requisition.y = m_child_requisition.y + 2.f * ( padding + border_width );
		}
	}

	return requisition;
}

const std::string& Notebook::GetName() const {
	static const std::string name( "Notebook" );
	return name;
}

Notebook::IndexType Notebook::AppendPage( Widget::Ptr child, Widget::Ptr tab_label ) {
	return InsertPage( child, tab_label, -1 );
}

Notebook::IndexType Notebook::PrependPage( Widget::Ptr child, Widget::Ptr tab_label ) {
	return InsertPage( child, tab_label, 0 );
}

Notebook::IndexType Notebook::InsertPage( Widget::Ptr child, Widget::Ptr tab_label, IndexType position ) {
	if( ( position >= GetPageCount() ) || ( position < 0 ) ) {
		m_children.push_back( ChildLabelPair( child, tab_label ) );

		Add( child );
		Add( tab_label );

		child->Show( false );

		SetCurrentPage( GetCurrentPage() );

		RecalculateSize();

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

	RecalculateSize();

	return position;
}

void Notebook::RemovePage( IndexType page_number ) {
	if( GetPageCount() <= 1 ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Cannot remove the only remaining page of a Notebook.\n";
#endif
		return;
	}

	if( page_number == GetCurrentPage() ) {
		// We need to change the page before removing.
		if( page_number == GetPageCount() - 1 ) {
			// If the requested page is the last one, flip to previous page.
			PreviousPage();
		}
		else {
			// Else flip to the next page.
			NextPage();
		}
	}

	if( ( page_number < 0 ) || ( page_number >= GetPageCount() ) ) {
		Remove( m_children.back().child );
	}
	else {
		Remove( ( m_children.begin() + page_number )->child );
	}

	RecalculateSize();

	// Correct m_current_page if the removed page was before the current page.
	// It can't be the current page because we took care of that already.
	if( page_number < GetCurrentPage() ) {
		m_current_page--;
	}

	// The same for the displayed tabs
	m_first_tab = std::max( m_first_tab - 1, 0 );

	Invalidate();
}

Notebook::IndexType Notebook::GetPageOf( Widget::Ptr widget ) const {
	ChildrenList::const_iterator iter( std::find( m_children.begin(), m_children.end(), ChildLabelPair( widget, Widget::Ptr() ) ) );

	if( iter == m_children.end() ) {
		return -1;
	}

	return static_cast<IndexType>( iter - m_children.begin() );
}

void Notebook::NextPage() {
	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[static_cast<std::size_t>( m_current_page )].child->Show( false );
	}

	m_current_page = std::min( m_current_page + 1, GetPageCount() - 1 );

	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[static_cast<std::size_t>( m_current_page )].child->Show( true );
	}

	Invalidate();
}

void Notebook::PreviousPage() {
	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[static_cast<std::size_t>( m_current_page )].child->Show( false );
	}

	m_current_page = std::max( m_current_page - 1, 0 );

	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[static_cast<std::size_t>( m_current_page )].child->Show( true );
	}

	Invalidate();
}

void Notebook::ReorderChild( Widget::Ptr child, IndexType position ) {
	auto tab_label = GetTabLabel( child );

	if( !tab_label ) {
		return;
	}

	auto old_page_number = GetPageOf( child );

	RemovePage( old_page_number );
	InsertPage( child, tab_label, position - ( old_page_number > position ? 1 : 0 ) );
}

Notebook::TabPosition Notebook::GetTabPosition() const {
	return static_cast<TabPosition>( m_tab_position );
}

void Notebook::SetTabPosition( TabPosition tab_position ) {
	m_tab_position = tab_position;
}

Notebook::IndexType Notebook::GetCurrentPage() const {
	return m_current_page;
}

const Widget::Ptr Notebook::GetNthPage( IndexType page_number ) const {
	if( ( page_number >= 0 ) && ( page_number < GetPageCount() ) ) {
		return m_children[static_cast<std::size_t>( page_number )].child;
	}

	return Widget::Ptr();
}

const Widget::Ptr Notebook::GetNthTabLabel( IndexType page_number ) const {
	if( ( page_number >= 0 ) && ( page_number < GetPageCount() ) ) {
		return m_children[static_cast<std::size_t>( page_number )].tab_label;
	}

	return Widget::Ptr();
}

Notebook::IndexType Notebook::GetPageCount() const {
	return static_cast<IndexType>( m_children.size() );
}

const Widget::Ptr Notebook::GetTabLabel( Widget::Ptr child ) const {
	auto page_number = GetPageOf( child );

	if( page_number < 0 ) {
		return Widget::Ptr();
	}

	return m_children[static_cast<std::size_t>( page_number )].tab_label;
}

void Notebook::SetTabLabel( Widget::Ptr child, Widget::Ptr tab_label ) {
	auto page_number = GetPageOf( child );

	if( page_number < 0 ) {
		return;
	}

	m_children[static_cast<std::size_t>( page_number )].tab_label = tab_label;
}

void Notebook::SetCurrentPage( IndexType page_number ) {
	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[static_cast<std::size_t>( m_current_page )].child->Show( false );
	}

	if( ( page_number >= 0 ) && ( page_number < GetPageCount() ) ) {
		m_current_page = page_number;
	}
	else {
		m_current_page = GetPageCount() - 1;
	}

	if( ( m_current_page >= 0 ) && ( m_current_page < GetPageCount() ) ) {
		m_children[static_cast<std::size_t>( m_current_page )].child->Show( true );
	}

	Invalidate();
}

Notebook::IndexType Notebook::GetPrelightTab() const {
	return m_prelight_tab;
}

Notebook::ChildLabelPair::ChildLabelPair( Widget::Ptr child_, Widget::Ptr tab_label_ ) :
	child( child_ ),
	tab_label( tab_label_ )
{
}

bool Notebook::ChildLabelPair::operator==( const ChildLabelPair& rhs ) const {
	return child == rhs.child;
}

void Notebook::HandleMouseMoveEvent( int x, int y ) {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float scroll_button_size( Context::Get().GetEngine().GetProperty<float>( "ScrollButtonSize", shared_from_this() ) );

	auto old_prelight_tab = m_prelight_tab;
	m_prelight_tab = -1;

	auto old_forward_scroll_prelight = m_forward_scroll_prelight;
	auto old_backward_scroll_prelight = m_backward_scroll_prelight;

	m_forward_scroll_prelight = false;
	m_backward_scroll_prelight = false;

	if( !GetDisplayedTabCount() ) {
		return;
	}

	sf::Vector2f tab_size( GetNthTabLabel( 0 )->GetAllocation().width, GetNthTabLabel( 0 )->GetAllocation().height );

	sf::FloatRect scroll_button_allocation( GetAllocation() );

	if( GetFirstDisplayedTab() != 0 ) {
		// Handle backward scrolling button.
		switch( GetTabPosition() ) {
			case TabPosition::TOP:
				scroll_button_allocation.left += 0.f;
				scroll_button_allocation.top += 0.f;
				scroll_button_allocation.width = scroll_button_size;
				scroll_button_allocation.height = tab_size.y + 2.f * ( padding + border_width );
				break;
			case TabPosition::BOTTOM:
				scroll_button_allocation.left += 0.f;
				scroll_button_allocation.top += GetAllocation().height - ( scroll_button_size + padding );
				scroll_button_allocation.width = scroll_button_size;
				scroll_button_allocation.height = tab_size.y + 2.f * ( padding + border_width );
				break;
			case TabPosition::LEFT:
				scroll_button_allocation.left += 0.f;
				scroll_button_allocation.top += 0.f;
				scroll_button_allocation.width = tab_size.x + 2.f * ( padding + border_width );
				scroll_button_allocation.height = scroll_button_size;
				break;
			case TabPosition::RIGHT:
				scroll_button_allocation.left += ( GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) );
				scroll_button_allocation.top += 0.f;
				scroll_button_allocation.width = tab_size.x + 2.f * ( padding + border_width );
				scroll_button_allocation.height = scroll_button_size;
				break;
		}

		if( scroll_button_allocation.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			m_backward_scroll_prelight = true;
		}
	}

	if( GetFirstDisplayedTab() + GetDisplayedTabCount() < GetPageCount() ) {
		// Handle forward scrolling button.
		switch( GetTabPosition() ) {
			case TabPosition::TOP:
				scroll_button_allocation.left += ( GetAllocation().width - scroll_button_size );
				scroll_button_allocation.top += 0.f;
				scroll_button_allocation.width = scroll_button_size;
				scroll_button_allocation.height = tab_size.y + 2.f * ( padding + border_width );
				break;
			case TabPosition::BOTTOM:
				scroll_button_allocation.left += ( GetAllocation().width - scroll_button_size );
				scroll_button_allocation.top += GetAllocation().height - ( scroll_button_size + padding );
				scroll_button_allocation.width = scroll_button_size;
				scroll_button_allocation.height = tab_size.y + 2.f * ( padding + border_width );
				break;
			case TabPosition::LEFT:
				scroll_button_allocation.left += 0.f;
				scroll_button_allocation.top += GetAllocation().height - scroll_button_size;
				scroll_button_allocation.width = tab_size.x + 2.f * ( padding + border_width );
				scroll_button_allocation.height = scroll_button_size;
				break;
			case TabPosition::RIGHT:
				scroll_button_allocation.left += ( GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) );
				scroll_button_allocation.top += GetAllocation().height - scroll_button_size;
				scroll_button_allocation.width = tab_size.x + 2.f * ( padding + border_width );
				scroll_button_allocation.height = scroll_button_size;
				break;
		}

		if( scroll_button_allocation.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			m_forward_scroll_prelight = true;
		}
	}

	for( auto index = GetFirstDisplayedTab(); index < GetFirstDisplayedTab() + GetDisplayedTabCount(); ++index ) {
		auto allocation = m_children[static_cast<std::size_t>( index )].tab_label->GetAllocation();

		allocation.left -= padding;
		allocation.top -= padding;
		allocation.width += 2.f * padding;
		allocation.height += 2.f * padding;

		if( allocation.contains( sf::Vector2f( static_cast<float>( x ) - GetAllocation().left, static_cast<float>( y ) - GetAllocation().top ) ) ) {
			m_prelight_tab = static_cast<IndexType>( index );
			break;
		}
	}

	if( old_prelight_tab != m_prelight_tab || old_forward_scroll_prelight != m_forward_scroll_prelight || old_backward_scroll_prelight != m_backward_scroll_prelight ) {
		Invalidate();
	}
}

void Notebook::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( ( button != sf::Mouse::Left ) ) {
		return;
	}

	float scroll_speed( Context::Get().GetEngine().GetProperty<float>( "ScrollSpeed", shared_from_this() ) );

	m_scrolling_forward = false;
	m_scrolling_backward = false;

	Invalidate();

	if( m_forward_scroll_prelight && press ) {
		m_scrolling_forward = true;
		m_elapsed_time = ( 1.f / scroll_speed );
		return;
	}
	else if( m_backward_scroll_prelight && press ) {
		m_scrolling_backward = true;
		m_elapsed_time = ( 1.f / scroll_speed );
		return;
	}

	if( !press ) {
		return;
	}

	if( m_prelight_tab < 0 ) {
		return;
	}

	SetCurrentPage( m_prelight_tab );
}

bool Notebook::HandleAdd( Widget::Ptr child ) {
	auto allowed = false;

	for( const auto& tab : m_children ) {
		if( ( tab.child == child ) || ( tab.tab_label == child ) ) {
			allowed = true;
			break;
		}
	}

	// If the child or label isn't found on the children list, the user added the widget
	// manually, which is not allowed for this class.
	if( !allowed ) {

#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Child must be added via AppendPage() or PrependPage() or InsertPage() for sfg::Notebook widgets.\n";
#endif

		return false;
	}

	Container::HandleAdd( child );

	RequestResize();
	Invalidate();

	return true;
}

void Notebook::HandleRemove( Widget::Ptr child ) {
	auto page_number = GetPageOf( child );

	if( page_number >= 0 ) {
		Remove( ( m_children.begin() + GetPageOf( child ) )->tab_label );
		m_children.erase( m_children.begin() + GetPageOf( child ) );
	}

	RequestResize();
	Invalidate();
}

void Notebook::HandleSizeChange() {
	m_first_tab = 0;

	RecalculateSize();
}

void Notebook::HandleUpdate( float seconds ) {
	Container::HandleUpdate( seconds );

	float scroll_speed( Context::Get().GetEngine().GetProperty<float>( "ScrollSpeed", shared_from_this() ) );

	m_elapsed_time += seconds;

	if( m_elapsed_time < ( 1.f / scroll_speed ) ) {
		return;
	}

	m_elapsed_time -= ( 1.f / scroll_speed );

	if( IsScrollingForward() ) {
		m_first_tab = std::min( m_first_tab + 1, GetPageCount() - GetDisplayedTabCount() );
		RecalculateSize();
	}
	else if( IsScrollingBackward() ) {
		m_first_tab = std::max( m_first_tab - 1, 0 );
		RecalculateSize();
	}
}

void Notebook::RecalculateSize() {
	if( !IsGloballyVisible() ) {
		return;
	}

	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float scroll_button_size( Context::Get().GetEngine().GetProperty<float>( "ScrollButtonSize", shared_from_this() ) );

	for( const auto& child : m_children ) {
		child.tab_label->Show( false );
	}

	auto children_size = static_cast<IndexType>( m_children.size() );

	m_num_displayed_tabs = children_size - GetFirstDisplayedTab();

	if( GetTabPosition() == TabPosition::TOP ) {
		// Tabs are positioned at top.
		auto tab_current_x = ( GetScrollable() && GetFirstDisplayedTab() != 0 ) ? scroll_button_size : 0.f;

		for( auto index = GetFirstDisplayedTab(); index < children_size; ++index ) {
			if( GetScrollable() && ( tab_current_x + border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().x + scroll_button_size ) > GetAllocation().width ) {
				m_num_displayed_tabs = index - GetFirstDisplayedTab();
				break;
			}

			m_children[static_cast<std::size_t>( index )].tab_label->Show( true );

			m_children[static_cast<std::size_t>( index )].child->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					m_tab_requisition.y + border_width + padding,
					GetAllocation().width - 2.f * ( border_width + padding ),
					GetAllocation().height - ( 2.f * ( border_width + padding ) + m_tab_requisition.y )
				)
			);

			m_children[static_cast<std::size_t>( index )].tab_label->SetAllocation(
				sf::FloatRect(
					tab_current_x + border_width + padding,
					border_width + padding,
					m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().x,
					m_tab_requisition.y - 2.f * padding - border_width
				)
			);

			tab_current_x += ( border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().x );
		}
	}
	else if( GetTabPosition() == TabPosition::BOTTOM ) {
		// Tabs are positioned at bottom.
		auto tab_current_x = ( GetScrollable() && GetFirstDisplayedTab() != 0 ) ? scroll_button_size : 0.f;

		for( auto index = GetFirstDisplayedTab(); index < children_size; ++index ) {
			if( GetScrollable() && ( tab_current_x + border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().x + scroll_button_size ) > GetAllocation().width ) {
				m_num_displayed_tabs = index - GetFirstDisplayedTab();
				break;
			}

			m_children[static_cast<std::size_t>( index )].tab_label->Show( true );

			m_children[static_cast<std::size_t>( index )].child->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					border_width + padding,
					GetAllocation().width - 2.f * ( border_width + padding ),
					GetAllocation().height - ( 2.f * ( border_width + padding ) + m_tab_requisition.y )
				)
			);

			m_children[static_cast<std::size_t>( index )].tab_label->SetAllocation(
				sf::FloatRect(
					tab_current_x + border_width + padding,
					GetAllocation().height - m_tab_requisition.y + padding,
					m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().x,
					m_tab_requisition.y - 2.f * padding - border_width
				)
			);

			tab_current_x += ( border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().x );
		}
	}
	else if( GetTabPosition() == TabPosition::LEFT ) {
		// Tabs are positioned at left.
		auto tab_current_y = ( GetScrollable() && GetFirstDisplayedTab() != 0 ) ? scroll_button_size : 0.f;

		for( auto index = GetFirstDisplayedTab(); index < children_size; ++index ) {
			if( GetScrollable() && ( tab_current_y + border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().y + scroll_button_size ) > GetAllocation().height ) {
				m_num_displayed_tabs = index - GetFirstDisplayedTab();
				break;
			}

			m_children[static_cast<std::size_t>( index )].tab_label->Show( true );

			m_children[static_cast<std::size_t>( index )].child->SetAllocation(
				sf::FloatRect(
					m_tab_requisition.x + border_width + padding,
					border_width + padding,
					GetAllocation().width - ( 2.f * ( border_width + padding ) + m_tab_requisition.x ),
					GetAllocation().height - 2.f * ( border_width + padding )
				)
			);

			m_children[static_cast<std::size_t>( index )].tab_label->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					tab_current_y + border_width + padding,
					m_tab_requisition.x - 2.f * padding - border_width,
					m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().y
				)
			);

			tab_current_y += ( border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().y );
		}
	}
	else if( GetTabPosition() == TabPosition::RIGHT ) {
		// Tabs are positioned at right.
		auto tab_current_y = ( GetScrollable() && GetFirstDisplayedTab() != 0 ) ? scroll_button_size : 0.f;

		for( auto index = GetFirstDisplayedTab(); index < children_size; ++index ) {
			if( GetScrollable() && ( tab_current_y + border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().y + scroll_button_size ) > GetAllocation().height ) {
				m_num_displayed_tabs = index - GetFirstDisplayedTab();
				break;
			}

			m_children[static_cast<std::size_t>( index )].tab_label->Show( true );

			m_children[static_cast<std::size_t>( index )].child->SetAllocation(
				sf::FloatRect(
					border_width + padding,
					border_width + padding,
					GetAllocation().width - ( 2.f * ( border_width + padding ) + m_tab_requisition.x ),
					GetAllocation().height - 2.f * ( border_width + padding )
				)
			);

			m_children[static_cast<std::size_t>( index )].tab_label->SetAllocation(
				sf::FloatRect(
					GetAllocation().width - m_tab_requisition.x + padding,
					tab_current_y + border_width + padding,
					m_tab_requisition.x - 2.f * padding - border_width,
					m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().y
				)
			);

			tab_current_y += ( border_width + 2.f * padding + m_children[static_cast<std::size_t>( index )].tab_label->GetRequisition().y );
		}
	}

	Invalidate();
}

void Notebook::HandleGlobalVisibilityChange() {
	Container::HandleGlobalVisibilityChange();

	RecalculateSize();
}

void Notebook::SetScrollable( bool scrollable ) {
	m_scrollable = scrollable;

	RecalculateSize();
}

bool Notebook::GetScrollable() const {
	return m_scrollable;
}

Notebook::IndexType Notebook::GetFirstDisplayedTab() const {
	if( !m_scrollable ) {
		return 0;
	}

	return m_first_tab;
}

Notebook::IndexType Notebook::GetDisplayedTabCount() const {
	if( !m_scrollable ) {
		return GetPageCount();
	}

	return m_num_displayed_tabs;
}

bool Notebook::IsScrollingForward() const {
	return m_scrolling_forward;
}

bool Notebook::IsScrollingBackward() const {
	return m_scrolling_backward;
}

bool Notebook::IsForwardScrollPrelight() const {
	return m_forward_scroll_prelight;
}

bool Notebook::IsBackwardScrollPrelight() const {
	return m_backward_scroll_prelight;
}

}
