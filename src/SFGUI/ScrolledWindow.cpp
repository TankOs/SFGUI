#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ScrolledWindow::ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) :
	Bin( true ),
	m_horizontal_scrollbar(),
	m_vertical_scrollbar(),
	m_render_image(),
	m_sprite(),
	m_recalc_adjustments( false )
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

ScrolledWindow::HandleEventResult ScrolledWindow::HandleEvent( const sf::Event& event ) {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Width", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Spacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

	// Pass event to scrollbars
	if( m_horizontal_scrollbar->HandleEvent( event ) == EatEvent ) {
		return EatEvent;
	}

	if( m_vertical_scrollbar->HandleEvent( event ) == EatEvent ) {
		return EatEvent;
	}

	// Pass event to child
	if( GetChild() ) {
		sf::FloatRect child_rect = GetAllocation();

		child_rect.Left += border_width;
		child_rect.Top += border_width;
		child_rect.Width -= ( scrollbar_width + scrollbar_spacing + 2.f * border_width );
		child_rect.Height -= ( scrollbar_width + scrollbar_spacing + 2.f * border_width );

		// Adjust a possible mouse event
		sf::Event altered_event( event );

		float offset_x = ( -GetAllocation().Left + m_horizontal_scrollbar->GetValue() );
		float offset_y = ( -GetAllocation().Top + m_vertical_scrollbar->GetValue() );

		switch( event.Type ) {
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased: {
			if( !child_rect.Contains( static_cast<float>( altered_event.MouseButton.X ), static_cast<float>( altered_event.MouseButton.Y ) ) ) {
				break;
			}
			altered_event.MouseButton.X += offset_x;
			altered_event.MouseButton.Y += offset_y;
			return GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
		case sf::Event::MouseMoved: {
			if( !child_rect.Contains( static_cast<float>( altered_event.MouseMove.X ), static_cast<float>( altered_event.MouseMove.Y ) ) ) {
				break;
			}
			altered_event.MouseMove.X += offset_x;
			altered_event.MouseMove.Y += offset_y;
			return GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseWheelMoved: {
			if( !child_rect.Contains( static_cast<float>( altered_event.MouseWheel.X ), static_cast<float>( altered_event.MouseWheel.Y ) ) ) {
				break;
			}
			altered_event.MouseWheel.X += offset_x;
			altered_event.MouseWheel.Y += offset_y;
			return GetChild()->HandleEvent( altered_event );
		} break;
		default: break;
		}
	}

	return PassEvent;
}

sf::Drawable* ScrolledWindow::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetEngine().CreateScrolledWindowDrawable( boost::shared_dynamic_cast<ScrolledWindow>( shared_from_this() ), target );
}

sf::Vector2f ScrolledWindow::GetRequisitionImpl() const {
	m_recalc_adjustments = true;

	sf::Vector2f requisition( 0.f, 0.f );

	return requisition;
}

void ScrolledWindow::RecalculateAdjustments() const {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Width", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Spacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

	if( GetChild() ) {
		float max_horiz_val = std::max( GetChild()->GetAllocation().Width + 2.f, GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		m_horizontal_scrollbar->GetAdjustment()->SetLower( .0f );
		m_horizontal_scrollbar->GetAdjustment()->SetUpper( max_horiz_val );
		m_horizontal_scrollbar->GetAdjustment()->SetMinorStep( 1.f );
		m_horizontal_scrollbar->GetAdjustment()->SetMajorStep( GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		m_horizontal_scrollbar->GetAdjustment()->SetPageSize( GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		float max_vert_val = std::max( GetChild()->GetAllocation().Height + 2.f, GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		m_vertical_scrollbar->GetAdjustment()->SetLower( .0f );
		m_vertical_scrollbar->GetAdjustment()->SetUpper( max_vert_val );
		m_vertical_scrollbar->GetAdjustment()->SetMinorStep( 1.f );
		m_vertical_scrollbar->GetAdjustment()->SetMajorStep( GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		m_vertical_scrollbar->GetAdjustment()->SetPageSize( GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
	}

	m_recalc_adjustments = false;
}

void ScrolledWindow::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& /*oldallocation*/ ) {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Width", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Spacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

	m_horizontal_scrollbar->AllocateSize(
		sf::FloatRect(
			GetAllocation().Left,
			GetAllocation().Top + GetAllocation().Height - scrollbar_width,
			GetAllocation().Width - m_vertical_scrollbar->GetAllocation().Width,
			scrollbar_width
		)
	);

	m_vertical_scrollbar->AllocateSize(
		sf::FloatRect(
			GetAllocation().Left + GetAllocation().Width - scrollbar_width,
			GetAllocation().Top,
			scrollbar_width,
			GetAllocation().Height - scrollbar_width
		)
	);

	bool result = m_render_image.Create(
		GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f,
		GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f
	);

	if( !result ) {
		std::cerr << "Failed to create RenderImage." << std::endl;
	}
}

void ScrolledWindow::HandlePositionChange( Widget::Ptr /*widget*/, const sf::FloatRect& oldallocation ) {
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

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
		GetChild()->SetPosition( sf::Vector2f( border_width, border_width ) );
	}
}

void ScrolledWindow::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& target ) {
	float scroll_speed( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Stepper.Speed", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Spacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

	m_horizontal_scrollbar->SetProperty( "Scrollbar.Stepper.Speed", scroll_speed );
	m_vertical_scrollbar->SetProperty( "Scrollbar.Stepper.Speed", scroll_speed );

	if( m_recalc_adjustments ) {
		RecalculateAdjustments();
	}

	m_horizontal_scrollbar->Expose( target );
	m_vertical_scrollbar->Expose( target );

	if( !GetChild() ) {
		return;
	}

	sf::FloatRect hscroll_rect = m_horizontal_scrollbar->GetAllocation();
	sf::FloatRect vscroll_rect = m_vertical_scrollbar->GetAllocation();

	// Clear surface
	m_render_image.Clear( sf::Color( 0, 0, 0, 0 ) );

	// Set viewing region of the surface
	sf::View view(
		sf::FloatRect(
			m_horizontal_scrollbar->GetValue(),
			m_vertical_scrollbar->GetValue(),
			GetAllocation().Width - vscroll_rect.Width - scrollbar_spacing - 2.f * border_width,
			GetAllocation().Height - hscroll_rect.Height - scrollbar_spacing - 2.f * border_width
		)
	);
	m_render_image.SetView( view );

	// Render child to surface
	GetChild()->Expose( m_render_image );

	// Finalize and bind to Sprite
	m_render_image.Display();
	m_sprite.SetImage( m_render_image.GetImage() );

	m_sprite.SetPosition( GetAllocation().Left + border_width, GetAllocation().Top + border_width );

	// Draw Sprite to target
	target.Draw( m_sprite );
}

}
