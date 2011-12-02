#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ScrolledWindow::ScrolledWindow( const Adjustment::Ptr& horizontal_adjustment, const Adjustment::Ptr& vertical_adjustment ) :
	Container(),
	m_content_allocation(),
	m_horizontal_scrollbar(),
	m_vertical_scrollbar(),
	m_policy( DEFAULT_POLICY ),
	m_placement( DEFAULT_PLACEMENT ),
	m_recalc_adjustments( false ),
	m_recalc_content_allocation( false )
{
	m_horizontal_scrollbar = Scrollbar::Create( horizontal_adjustment, Scrollbar::HORIZONTAL );
	m_vertical_scrollbar = Scrollbar::Create( vertical_adjustment, Scrollbar::VERTICAL );
}

ScrolledWindow::Ptr ScrolledWindow::Create() {
	return ScrolledWindow::Create( Adjustment::Create(), Adjustment::Create() );
}

ScrolledWindow::Ptr ScrolledWindow::Create( const Adjustment::Ptr& horizontal_adjustment, const Adjustment::Ptr& vertical_adjustment ) {
	ScrolledWindow::Ptr ptr( new ScrolledWindow( horizontal_adjustment, vertical_adjustment ) );

	StaticPointerCast<Container>( ptr )->Add( ptr->m_horizontal_scrollbar );
	StaticPointerCast<Container>( ptr )->Add( ptr->m_vertical_scrollbar );

	return ptr;
}

const Adjustment::Ptr& ScrolledWindow::GetHorizontalAdjustment() const {
	return m_horizontal_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetHorizontalAdjustment( const Adjustment::Ptr& adjustment ) {
	m_horizontal_scrollbar->SetAdjustment( adjustment );

	m_recalc_content_allocation = true;
	Invalidate();
}

const Adjustment::Ptr& ScrolledWindow::GetVerticalAdjustment() const {
	return m_vertical_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetVerticalAdjustment( const Adjustment::Ptr& adjustment ) {
	m_vertical_scrollbar->SetAdjustment( adjustment );

	m_recalc_content_allocation = true;
	Invalidate();
}

unsigned char ScrolledWindow::GetScrollbarPolicy() const {
	return m_policy;
}

void ScrolledWindow::SetScrollbarPolicy( unsigned char policy ) {
	m_policy = policy;

	m_recalc_content_allocation = true;
	Invalidate();
}

void ScrolledWindow::SetPlacement( Placement placement ) {
	if(
		(((placement & TOP) == TOP) ^ ((placement & BOTTOM) == BOTTOM)) &&
		(((placement & LEFT) == LEFT) ^ ((placement & RIGHT) == RIGHT))
	) {
		m_placement = placement;

		m_recalc_content_allocation = true;
		Invalidate();
	}
}

bool ScrolledWindow::IsHorizontalScrollbarVisible() const {
	if( m_policy & HORIZONTAL_ALWAYS ) {
		return true;
	}

	if( m_policy & HORIZONTAL_NEVER ) {
		return false;
	}

	// If we get here Scrollbar is set to Automatic

	Adjustment::Ptr adjustment = m_horizontal_scrollbar->GetAdjustment();
	float value_range = adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize();

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

	Adjustment::Ptr adjustment = m_vertical_scrollbar->GetAdjustment();
	float value_range = adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize();

	if( value_range <= .0f ) {
		// Nothing to scroll
		return false;
	}

	return true;
}

const sf::FloatRect& ScrolledWindow::GetContentAllocation() const {
	return m_content_allocation;
}

RenderQueue* ScrolledWindow::InvalidateImpl() const {
	if( m_recalc_adjustments ) {
		RecalculateAdjustments();
	}

	if( m_recalc_content_allocation ) {
		RecalculateContentAllocation();
	}

	return Context::Get().GetEngine().CreateScrolledWindowDrawable( DynamicPointerCast<const ScrolledWindow>( shared_from_this() ) );
}

sf::Vector2f ScrolledWindow::CalculateRequisition() {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrollbarWidth", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrollbarSpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	return sf::Vector2f( scrollbar_width + scrollbar_spacing + border_width, scrollbar_width + scrollbar_spacing + border_width );
}

void ScrolledWindow::RecalculateAdjustments() const {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrollbarWidth", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrollbarSpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	if( GetViewport() && GetViewport()->GetChild() ) {
		float max_horiz_val = std::max( GetViewport()->GetChild()->GetAllocation().Width + 2.f, GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		Adjustment::Ptr h_adjustment = m_horizontal_scrollbar->GetAdjustment();
		h_adjustment->SetLower( .0f );
		h_adjustment->SetUpper( max_horiz_val );
		h_adjustment->SetMinorStep( 1.f );
		h_adjustment->SetMajorStep( GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		h_adjustment->SetPageSize( GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		float max_vert_val = std::max( GetViewport()->GetChild()->GetAllocation().Height + 2.f, GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		Adjustment::Ptr v_adjustment = m_vertical_scrollbar->GetAdjustment();
		v_adjustment->SetLower( .0f );
		v_adjustment->SetUpper( max_vert_val );
		v_adjustment->SetMinorStep( 1.f );
		v_adjustment->SetMajorStep( GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		v_adjustment->SetPageSize( GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
	}

	m_recalc_adjustments = false;
	m_recalc_content_allocation = true;

	m_horizontal_scrollbar->Show( IsHorizontalScrollbarVisible() );
	m_vertical_scrollbar->Show( IsVerticalScrollbarVisible() );
}

void ScrolledWindow::RecalculateContentAllocation() const {
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrollbarSpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	m_content_allocation = GetAllocation();

	m_content_allocation.Left = border_width;
	m_content_allocation.Top = border_width;
	m_content_allocation.Width -= 2.f * border_width;
	m_content_allocation.Height -= 2.f * border_width;

	if( IsVerticalScrollbarVisible() ) {
		m_content_allocation.Width -= ( m_vertical_scrollbar->GetRequisition().x + scrollbar_spacing );

		if( m_placement & RIGHT ) { // Content placed at Right
			m_content_allocation.Left += ( m_vertical_scrollbar->GetRequisition().x + scrollbar_spacing );
		}
	}

	if( IsHorizontalScrollbarVisible() ) {
		m_content_allocation.Height -= ( m_horizontal_scrollbar->GetRequisition().x + scrollbar_spacing );

		if( m_placement & BOTTOM ) { // Content placed at Bottom
			m_content_allocation.Top += ( m_horizontal_scrollbar->GetRequisition().x + scrollbar_spacing );
		}
	}

	if( m_placement & TOP ) { // Content placed at Top
		m_horizontal_scrollbar->SetAllocation(
			sf::FloatRect(
				m_content_allocation.Left - border_width,
				m_content_allocation.Height + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Width + 2.f * border_width,
				m_horizontal_scrollbar->GetRequisition().y
			)
		);
	}
	else { // Content placed at Bottom
		m_horizontal_scrollbar->SetAllocation(
			sf::FloatRect(
				m_content_allocation.Left - border_width,
				0.f,
				m_content_allocation.Width + 2.f * border_width,
				m_horizontal_scrollbar->GetRequisition().y
			)
		);
	}

	if( m_placement & LEFT ) { // Content placed at Left
		m_vertical_scrollbar->SetAllocation(
			sf::FloatRect(
				m_content_allocation.Width + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Top - border_width,
				m_vertical_scrollbar->GetRequisition().x,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	}
	else { // Content placed at Right
		m_vertical_scrollbar->SetAllocation(
			sf::FloatRect(
				0.f,
				m_content_allocation.Top - border_width,
				m_vertical_scrollbar->GetRequisition().x,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	}

	// Update Scrollbars and viewport allocation if the content allocation changed.
	if( GetViewport() ) {
		m_horizontal_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.Width );
		m_horizontal_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.Width );

		m_vertical_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.Height );
		m_vertical_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.Height );

		GetViewport()->SetAllocation( m_content_allocation );
	}

	m_recalc_adjustments = false;
	m_recalc_content_allocation = false;
}

void ScrolledWindow::HandleAllocationChange( const sf::FloatRect& old_allocation ) {
	Container::HandleAllocationChange( old_allocation );

	// A parent caused us to move/resize, have to recalculate everything.
	m_recalc_adjustments = true;
	m_recalc_content_allocation = true;
	Invalidate();
}

void ScrolledWindow::HandleAdd( const Widget::Ptr& child ) {
	if( GetChildren().size() > 3 ) {

#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Only one widget can be added to a ScrolledWindow." << std::endl;
#endif

		Remove( child );
		return;
	}

	m_recalc_content_allocation = true;
	Invalidate();
}

void ScrolledWindow::AddWithViewport( const Widget::Ptr& widget ) {
	Viewport::Ptr viewport = Viewport::Create();

	viewport->SetHorizontalAdjustment( m_horizontal_scrollbar->GetAdjustment() );
	viewport->SetVerticalAdjustment( m_vertical_scrollbar->GetAdjustment() );

	viewport->Add( widget );

	Container::Add( viewport );
}

void ScrolledWindow::Add( const Widget::Ptr& /*widget*/ ) {
#ifdef SFGUI_DEBUG
	std::cerr << "SFGUI warning: Widgets can only be added to a ScrolledWindow with a Viewport." << std::endl;
#endif
}

void ScrolledWindow::Remove( const Widget::Ptr& widget ) {
	Viewport::Ptr viewport = GetViewport();

	if( viewport->GetChild() == widget ) {
		viewport->Remove( widget );
		Container::Remove( viewport );
	}
}

Viewport::Ptr ScrolledWindow::GetViewport() const {
	if( GetChildren().size() < 3 ) {
		return Viewport::Ptr();
	}

	return StaticPointerCast<Viewport>( *( --GetChildren().end() ) );
}

void ScrolledWindow::HandleChildInvalidate( const Widget::PtrConst& child ) const {
	// A child has been invalidated. Update Scrollbars.
	m_recalc_adjustments = true;
	Invalidate();
	Container::HandleChildInvalidate( child );
}

const std::string& ScrolledWindow::GetName() const {
	static const std::string name( "ScrolledWindow" );
	return name;
}

}
