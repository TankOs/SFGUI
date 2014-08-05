#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Viewport.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Scrollbar.hpp>

namespace sfg {

ScrolledWindow::ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) :
	m_content_allocation(),
	m_horizontal_scrollbar(),
	m_vertical_scrollbar(),
	m_viewport(),
	m_policy( ScrollbarPolicy::DEFAULT ),
	m_placement( Placement::DEFAULT )
{
	m_horizontal_scrollbar = Scrollbar::Create( horizontal_adjustment, Scrollbar::Orientation::HORIZONTAL );
	m_vertical_scrollbar = Scrollbar::Create( vertical_adjustment, Scrollbar::Orientation::VERTICAL );
}

ScrolledWindow::Ptr ScrolledWindow::Create() {
	return ScrolledWindow::Create( Adjustment::Create(), Adjustment::Create() );
}

ScrolledWindow::Ptr ScrolledWindow::Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) {
	Ptr ptr( new ScrolledWindow( horizontal_adjustment, vertical_adjustment ) );

	std::static_pointer_cast<Container>( ptr )->Add( ptr->m_horizontal_scrollbar );
	std::static_pointer_cast<Container>( ptr )->Add( ptr->m_vertical_scrollbar );

	return ptr;
}

Adjustment::Ptr ScrolledWindow::GetHorizontalAdjustment() const {
	return m_horizontal_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetHorizontalAdjustment( Adjustment::Ptr adjustment ) {
	m_horizontal_scrollbar->SetAdjustment( adjustment );

	if( m_viewport ) {
		m_viewport->SetHorizontalAdjustment( adjustment );
	}

	RecalculateContentAllocation();
	Invalidate();
}

Adjustment::Ptr ScrolledWindow::GetVerticalAdjustment() const {
	return m_vertical_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetVerticalAdjustment( Adjustment::Ptr adjustment ) {
	m_vertical_scrollbar->SetAdjustment( adjustment );

	if( m_viewport ) {
		m_viewport->SetVerticalAdjustment( adjustment );
	}

	RecalculateContentAllocation();
	Invalidate();
}

char ScrolledWindow::GetScrollbarPolicy() const {
	return m_policy;
}

void ScrolledWindow::SetScrollbarPolicy( char policy ) {
	m_policy = policy;

	RecalculateContentAllocation();
	Invalidate();
}

void ScrolledWindow::SetPlacement( Placement placement ) {
	m_placement = placement;

	RecalculateContentAllocation();
	Invalidate();
}

bool ScrolledWindow::IsHorizontalScrollbarVisible() const {
	if( m_policy & HORIZONTAL_ALWAYS ) {
		return true;
	}

	if( m_policy & HORIZONTAL_NEVER ) {
		return false;
	}

	// If we get here Scrollbar is set to Automatic

	auto adjustment = m_horizontal_scrollbar->GetAdjustment();
	auto value_range = adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize();

	if( value_range <= .0f ) {
		// Nothing to scroll
		return false;
	}

	return true;
}

bool ScrolledWindow::IsVerticalScrollbarVisible() const {
	if( m_policy & VERTICAL_ALWAYS ) {
		return true;
	}

	if( m_policy & VERTICAL_NEVER ) {
		return false;
	}

	// If we get here Scrollbar is set to Automatic

	auto adjustment = m_vertical_scrollbar->GetAdjustment();
	auto value_range = adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize();

	if( value_range <= .0f ) {
		// Nothing to scroll
		return false;
	}

	return true;
}

const sf::FloatRect& ScrolledWindow::GetContentAllocation() const {
	return m_content_allocation;
}

std::unique_ptr<RenderQueue> ScrolledWindow::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateScrolledWindowDrawable( std::dynamic_pointer_cast<const ScrolledWindow>( shared_from_this() ) );
}

sf::Vector2f ScrolledWindow::CalculateRequisition() {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrollbarWidth", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrollbarSpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	sf::Vector2f requisition( scrollbar_width + scrollbar_spacing + border_width, scrollbar_width + scrollbar_spacing + border_width );

	sfg::Viewport::Ptr viewport( GetViewport() );

	if( viewport ) {
		if( GetScrollbarPolicy() & HORIZONTAL_NEVER ) {
			requisition += sf::Vector2f( viewport->GetChildRequisition().x, 0.f );
		}

		if( GetScrollbarPolicy() & VERTICAL_NEVER ) {
			requisition += sf::Vector2f( 0.f, viewport->GetChildRequisition().y );
		}
	}

	return requisition;
}

void ScrolledWindow::RecalculateAdjustments() const {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrollbarWidth", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrollbarSpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	if( GetViewport() && GetViewport()->GetChild() ) {
		auto max_horiz_val = std::max( GetViewport()->GetChild()->GetAllocation().width + border_width * 2.f, GetAllocation().width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		auto h_adjustment = m_horizontal_scrollbar->GetAdjustment();
		h_adjustment->SetLower( .0f );
		h_adjustment->SetUpper( max_horiz_val );
		h_adjustment->SetMinorStep( 1.f );
		h_adjustment->SetMajorStep( GetAllocation().width - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		h_adjustment->SetPageSize( GetAllocation().width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		auto max_vert_val = std::max( GetViewport()->GetChild()->GetAllocation().height + border_width * 2.f, GetAllocation().height - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		auto v_adjustment = m_vertical_scrollbar->GetAdjustment();
		v_adjustment->SetLower( .0f );
		v_adjustment->SetUpper( max_vert_val );
		v_adjustment->SetMinorStep( 1.f );
		v_adjustment->SetMajorStep( GetAllocation().height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		v_adjustment->SetPageSize( GetAllocation().height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
	}

	RecalculateContentAllocation();

	auto horizontal_scrollbar_visible = m_horizontal_scrollbar->IsLocallyVisible();
	auto vertical_scrollbar_visible = m_vertical_scrollbar->IsLocallyVisible();

	m_horizontal_scrollbar->Show( IsHorizontalScrollbarVisible() );
	m_vertical_scrollbar->Show( IsVerticalScrollbarVisible() );

	// Recalculate if a scrollbar transitions from hidden to visible.
	// This is required to find the "optimal" scrollbar visibility
	// setting for a given viewport allocation.
	if( ( horizontal_scrollbar_visible != IsHorizontalScrollbarVisible() ) || ( vertical_scrollbar_visible != IsVerticalScrollbarVisible() ) ) {
		RecalculateAdjustments();
	}
}

void ScrolledWindow::RecalculateContentAllocation() const {
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrollbarSpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	m_content_allocation = GetAllocation();

	m_content_allocation.left = border_width;
	m_content_allocation.top = border_width;
	m_content_allocation.width -= 2.f * border_width;
	m_content_allocation.height -= 2.f * border_width;

	if( IsVerticalScrollbarVisible() ) {
		m_content_allocation.width -= ( m_vertical_scrollbar->GetRequisition().x + scrollbar_spacing );

		if( ( m_placement == Placement::TOP_RIGHT ) || ( m_placement == Placement::BOTTOM_RIGHT ) ) { // Content placed at Right
			m_content_allocation.left += ( m_vertical_scrollbar->GetRequisition().x + scrollbar_spacing );
		}
	}

	if( IsHorizontalScrollbarVisible() ) {
		m_content_allocation.height -= ( m_horizontal_scrollbar->GetRequisition().y + scrollbar_spacing );

		if( ( m_placement == Placement::BOTTOM_RIGHT ) || ( m_placement == Placement::BOTTOM_LEFT ) ) { // Content placed at Bottom
			m_content_allocation.top += ( m_horizontal_scrollbar->GetRequisition().y + scrollbar_spacing );
		}
	}

	if( ( m_placement == Placement::TOP_LEFT ) || ( m_placement == Placement::TOP_RIGHT ) ) { // Content placed at Top
		m_horizontal_scrollbar->SetAllocation(
			sf::FloatRect(
				m_content_allocation.left - border_width,
				m_content_allocation.height + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.width + 2.f * border_width,
				m_horizontal_scrollbar->GetRequisition().y
			)
		);
	}
	else { // Content placed at Bottom
		m_horizontal_scrollbar->SetAllocation(
			sf::FloatRect(
				m_content_allocation.left - border_width,
				0.f,
				m_content_allocation.width + 2.f * border_width,
				m_horizontal_scrollbar->GetRequisition().y
			)
		);
	}

	if( ( m_placement == Placement::TOP_LEFT ) || ( m_placement == Placement::BOTTOM_LEFT ) ) { // Content placed at Left
		m_vertical_scrollbar->SetAllocation(
			sf::FloatRect(
				m_content_allocation.width + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.top - border_width,
				m_vertical_scrollbar->GetRequisition().x,
				m_content_allocation.height + 2.f * border_width
			)
		);
	}
	else { // Content placed at Right
		m_vertical_scrollbar->SetAllocation(
			sf::FloatRect(
				0.f,
				m_content_allocation.top - border_width,
				m_vertical_scrollbar->GetRequisition().x,
				m_content_allocation.height + 2.f * border_width
			)
		);
	}

	// Update Scrollbars and viewport allocation if the content allocation changed.
	if( GetViewport() ) {
		m_horizontal_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.width );
		m_horizontal_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.width );

		m_vertical_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.height );
		m_vertical_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.height );

		GetViewport()->SetAllocation( m_content_allocation );
	}

	m_horizontal_scrollbar->RequestResize();
	m_horizontal_scrollbar->Invalidate();

	m_vertical_scrollbar->RequestResize();
	m_vertical_scrollbar->Invalidate();

	Invalidate();
}

void ScrolledWindow::HandleSizeChange() {
	// A parent caused us to move/resize, have to recalculate everything.
	RecalculateAdjustments();
	RecalculateContentAllocation();

	Invalidate();
}

bool ScrolledWindow::HandleAdd( Widget::Ptr child ) {
	if( GetChildren().size() > 2 ) {

#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Only one widget can be added to a ScrolledWindow.\n";
#endif

		return false;
	}

	Container::HandleAdd( child );

	RecalculateContentAllocation();
	Invalidate();

	return true;
}

void ScrolledWindow::AddWithViewport( Widget::Ptr widget ) {
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	if( GetChildren().size() > 2 ) {

#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Only one widget can be added to a ScrolledWindow.\n";
#endif

		return;
	}

	m_viewport = Viewport::Create();

	m_viewport->SetHorizontalAdjustment( m_horizontal_scrollbar->GetAdjustment() );
	m_viewport->SetVerticalAdjustment( m_vertical_scrollbar->GetAdjustment() );

	m_viewport->Add( widget );

	Container::Add( m_viewport );

	// Make sure the viewport is offset by the border width.
	sf::FloatRect allocation( m_viewport->GetAllocation() );

	allocation.left += border_width;
	allocation.top += border_width;

	m_viewport->SetAllocation( allocation );
}

void ScrolledWindow::Add( Widget::Ptr /*widget*/ ) {
#if defined( SFGUI_DEBUG )
	std::cerr << "SFGUI warning: Widgets can only be added to a ScrolledWindow with a Viewport.\n";
#endif
}

void ScrolledWindow::Remove( Widget::Ptr widget ) {
	if( m_viewport && m_viewport->GetChild() == widget ) {
		m_viewport->Remove( widget );
		Container::Remove( m_viewport );
		m_viewport.reset();
	}
}

Viewport::Ptr ScrolledWindow::GetViewport() const {
	return m_viewport;
}

void ScrolledWindow::HandleChildInvalidate( Widget::PtrConst child ) const {
	// A child has been invalidated. Update Scrollbars.
	RecalculateAdjustments();
	RecalculateContentAllocation();
	Container::HandleChildInvalidate( child );
	Invalidate();
}

const std::string& ScrolledWindow::GetName() const {
	static const std::string name( "ScrolledWindow" );
	return name;
}

}
