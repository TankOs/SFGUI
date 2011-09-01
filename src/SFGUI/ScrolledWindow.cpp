#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ScrolledWindow::ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) :
	Bin( true ),
	m_horizontal_scrollbar(),
	m_vertical_scrollbar(),
	m_render_image(),
	m_sprite()
{
	OnSizeAllocate.Connect( &ScrolledWindow::HandleSizeAllocate, this );
	OnPositionChange.Connect( &ScrolledWindow::HandlePositionChange, this );
	OnExpose.Connect( &ScrolledWindow::HandleExpose, this );

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

void ScrolledWindow::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& /*oldallocation*/ ) {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.ScrollbarWidth", shared_from_this() ) );

	m_horizontal_scrollbar->AllocateSize(
		sf::FloatRect(
			GetAllocation().Left,
			GetAllocation().Top + GetAllocation().Height - scrollbar_width,
			GetAllocation().Width - m_vertical_scrollbar->GetAllocation().Width,
			scrollbar_width
		)
	);

	m_horizontal_scrollbar->Invalidate();

	std::cout << GetAllocation().Left << " " << GetAllocation().Top << " " << GetAllocation().Width << " " << GetAllocation().Height << std::endl;
	std::cout << m_horizontal_scrollbar->GetAllocation().Left << " " << m_horizontal_scrollbar->GetAllocation().Top << " " << m_horizontal_scrollbar->GetAllocation().Width << " " << m_horizontal_scrollbar->GetAllocation().Height << std::endl << std::endl;

	m_vertical_scrollbar->AllocateSize(
		sf::FloatRect(
			GetAllocation().Left + GetAllocation().Width - scrollbar_width,
			GetAllocation().Top,
			scrollbar_width,
			GetAllocation().Height - scrollbar_width
		)
	);

	m_vertical_scrollbar->Invalidate();

	bool result = m_render_image.Create(
		GetAllocation().Width - m_vertical_scrollbar->GetAllocation().Width,
		GetAllocation().Height - m_horizontal_scrollbar->GetAllocation().Height
	);

	if( !result ) {
		std::cerr << "Failed to create RenderImage." << std::endl;
	}
}

void ScrolledWindow::HandlePositionChange( Widget::Ptr /*widget*/, const sf::FloatRect& oldallocation ) {
	sf::Vector2f delta( GetAllocation().Left - oldallocation.Left, GetAllocation().Top - oldallocation.Top );

	m_horizontal_scrollbar->SetPosition(
		sf::Vector2f(
			m_horizontal_scrollbar->GetAllocation().Left + delta.x,
			m_horizontal_scrollbar->GetAllocation().Top + delta.y
		)
	);

	m_vertical_scrollbar->SetPosition(
		sf::Vector2f(
			m_vertical_scrollbar->GetAllocation().Left + delta.x,
			m_vertical_scrollbar->GetAllocation().Top + delta.y
		)
	);

	if( GetChild() ) {
		GetChild()->SetPosition( sf::Vector2f( .0f, .0f ) );
	}
}

void ScrolledWindow::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& target ) {
	m_horizontal_scrollbar->Expose( target );
	//m_vertical_scrollbar->Expose( target );

	if( !GetChild() ) {
		return;
	}

	sf::FloatRect hscroll_rect = m_horizontal_scrollbar->GetAllocation();
	sf::FloatRect vscroll_rect = m_vertical_scrollbar->GetAllocation();

	// Clear surface
	m_render_image.Clear( sf::Color( 0, 0, 0, 0 ) );

	// Set viewing region of the surface
	sf::View view( sf::FloatRect( .0f, .0f, GetAllocation().Width - vscroll_rect.Width, GetAllocation().Height - hscroll_rect.Height ) );
	m_render_image.SetView( view );

	// Render child to surface
	GetChild()->Expose( m_render_image );

	// Finalize and bind to Sprite
	m_render_image.Display();
	m_sprite.SetImage( m_render_image.GetImage() );

	m_sprite.SetPosition( GetAllocation().Left, GetAllocation().Top );

	// Draw Sprite to target
	target.Draw( m_sprite );
}

}
