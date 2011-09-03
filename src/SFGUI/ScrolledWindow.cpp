#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ScrolledWindow::ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) :
	Bin(),
	m_horizontal_scrollbar(),
	m_vertical_scrollbar(),
	m_render_image(),
	m_sprite(),
	m_recalc_adjustments( false ),
	m_content_allocation(),
	m_recalc_content_allocation( false )
{
	OnSizeAllocate.Connect( &ScrolledWindow::HandleSizeAllocate, this );
	OnPositionChange.Connect( &ScrolledWindow::HandlePositionChange, this );
	m_expose_connection = OnExpose.Connect( &ScrolledWindow::HandleExpose, this );

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

	m_recalc_content_allocation = true;
}

Adjustment::Ptr ScrolledWindow::GetVerticalAdjustment() const {
	return m_vertical_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetVerticalAdjustment( Adjustment::Ptr adjustment ) {
	m_vertical_scrollbar->SetAdjustment( adjustment );

	m_recalc_content_allocation = true;
}

const ScrolledWindow::ScrollbarPolicyPair& ScrolledWindow::GetScrollbarPolicies() const {
	return m_policies;
}

void ScrolledWindow::SetScrollbarPolicies( const ScrolledWindow::ScrollbarPolicyPair& policies ) {
	m_policies = policies;

	m_recalc_content_allocation = true;
}

void ScrolledWindow::SetPlacement( Placement placement ) {
	m_placement = placement;

	m_recalc_content_allocation = true;
}

bool ScrolledWindow::IsHorizontalScrollbarVisible() const {
	if( m_policies.horizontal_policy == Always ) {
		return true;
	}

	if( m_policies.horizontal_policy == Never ) {
		return false;
	}

	float value_range = m_horizontal_scrollbar->GetAdjustment()->GetUpper() - m_horizontal_scrollbar->GetAdjustment()->GetLower() - m_horizontal_scrollbar->GetAdjustment()->GetPageSize();

	if( value_range <= .0f ) {
		return false;
	}

	return true;
}

bool ScrolledWindow::IsVerticalScrollbarVisible() const {
	if( m_policies.vertical_policy == Always ) {
		return true;
	}

	if( m_policies.vertical_policy == Never ) {
		return false;
	}

	float value_range = m_vertical_scrollbar->GetAdjustment()->GetUpper() - m_vertical_scrollbar->GetAdjustment()->GetLower() - m_vertical_scrollbar->GetAdjustment()->GetPageSize();

	if( value_range <= .0f ) {
		return false;
	}

	return true;
}

const sf::FloatRect& ScrolledWindow::GetContentAllocation() const {
	return m_content_allocation;
}

ScrolledWindow::HandleEventResult ScrolledWindow::HandleEvent( const sf::Event& event ) {
	// Pass event to scrollbars
	if( IsHorizontalScrollbarVisible() && m_horizontal_scrollbar->HandleEvent( event ) == EatEvent ) {
		return EatEvent;
	}

	if( IsVerticalScrollbarVisible() && m_vertical_scrollbar->HandleEvent( event ) == EatEvent ) {
		return EatEvent;
	}

	// Pass event to child
	if( GetChild() ) {
		sf::FloatRect child_rect = GetContentAllocation();

		// Adjust a possible mouse event
		sf::Event altered_event( event );

		float offset_x = ( -m_content_allocation.Left + m_horizontal_scrollbar->GetValue() );
		float offset_y = ( -m_content_allocation.Top + m_vertical_scrollbar->GetValue() );

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
		default: {
			return GetChild()->HandleEvent( altered_event );
		} break;
		}
	}

	return PassEvent;
}

sf::Drawable* ScrolledWindow::InvalidateImpl( const sf::RenderTarget& target ) {
	if( m_recalc_adjustments ) {
		RecalculateAdjustments();
	}

	if( m_recalc_content_allocation ) {
		RecalculateContentAllocation();
	}

	return Context::Get().GetEngine().CreateScrolledWindowDrawable( boost::shared_dynamic_cast<ScrolledWindow>( shared_from_this() ), target );
}

sf::Vector2f ScrolledWindow::GetRequisitionImpl() const {
	m_recalc_adjustments = true;
	m_recalc_content_allocation = true;

	sf::Vector2f requisition( 0.f, 0.f );

	// The only child, always gets what it wants ;)
	if( GetChild() ) {
		sf::FloatRect new_allocation = GetChild()->GetAllocation();
		new_allocation.Width = GetChild()->GetRequisition().x;
		new_allocation.Height = GetChild()->GetRequisition().y;
		GetChild()->AllocateSize( new_allocation );
	}

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

	m_recalc_content_allocation = true;
}

void ScrolledWindow::RecalculateContentAllocation() {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Width", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Spacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

	m_content_allocation = GetAllocation();

	m_content_allocation.Left += border_width;
	m_content_allocation.Top += border_width;
	m_content_allocation.Width -= 2.f * border_width;
	m_content_allocation.Height -= 2.f * border_width;

	if( IsVerticalScrollbarVisible() ) {
		m_content_allocation.Width -= ( scrollbar_width + scrollbar_spacing );
	}

	if( IsHorizontalScrollbarVisible() ) {
		m_content_allocation.Height -= ( scrollbar_width + scrollbar_spacing );
	}

	switch( m_placement ) {
	case TopLeft: break;
	case TopRight: {
		if( IsVerticalScrollbarVisible() ) {
			m_content_allocation.Left += ( scrollbar_width + scrollbar_spacing );
		}
	} break;
	case BottomLeft: {
		if( IsHorizontalScrollbarVisible() ) {
			m_content_allocation.Top += ( scrollbar_width + scrollbar_spacing );
		}
	} break;
	case BottomRight: {
		if( IsHorizontalScrollbarVisible() ) {
			m_content_allocation.Top += ( scrollbar_width + scrollbar_spacing );
		}
		if( IsVerticalScrollbarVisible() ) {
			m_content_allocation.Left += ( scrollbar_width + scrollbar_spacing );
		}
	} break;
	default: break;
	}

	switch( m_placement ) {
	case TopLeft: {
		m_horizontal_scrollbar->AllocateSize(
			sf::FloatRect(
				m_content_allocation.Left - border_width,
				GetAllocation().Top + m_content_allocation.Height + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Width + 2.f * border_width,
				scrollbar_width
			)
		);

		m_vertical_scrollbar->AllocateSize(
			sf::FloatRect(
				GetAllocation().Left + m_content_allocation.Width + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Top - border_width,
				scrollbar_width,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	} break;
	case TopRight: {
		m_horizontal_scrollbar->AllocateSize(
			sf::FloatRect(
				m_content_allocation.Left - border_width,
				GetAllocation().Top + m_content_allocation.Height + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Width + 2.f * border_width,
				scrollbar_width
			)
		);

		m_vertical_scrollbar->AllocateSize(
			sf::FloatRect(
				GetAllocation().Left,
				m_content_allocation.Top - border_width,
				scrollbar_width,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	} break;
	case BottomLeft: {
		m_horizontal_scrollbar->AllocateSize(
			sf::FloatRect(
				m_content_allocation.Left - border_width,
				GetAllocation().Top,
				m_content_allocation.Width + 2.f * border_width,
				scrollbar_width
			)
		);

		m_vertical_scrollbar->AllocateSize(
			sf::FloatRect(
				GetAllocation().Left + m_content_allocation.Width + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Top - border_width,
				scrollbar_width,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	} break;
	case BottomRight: {
		m_horizontal_scrollbar->AllocateSize(
			sf::FloatRect(
				m_content_allocation.Left - border_width,
				GetAllocation().Top,
				m_content_allocation.Width + 2.f * border_width,
				scrollbar_width
			)
		);

		m_vertical_scrollbar->AllocateSize(
			sf::FloatRect(
				GetAllocation().Left,
				m_content_allocation.Top - border_width,
				scrollbar_width,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	} break;
	default: break;
	}

	if( GetChild() ) {
		m_horizontal_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.Width );
		m_horizontal_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.Width );

		m_vertical_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.Height );
		m_vertical_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.Height );
	}

	bool result = m_render_image.Create(
		std::max( m_content_allocation.Width, static_cast<float>( m_render_image.GetWidth() ) ),
		std::max( m_content_allocation.Height, static_cast<float>( m_render_image.GetHeight() ) )
	);

	if( !result ) {
		std::cerr << "Failed to create RenderImage." << std::endl;
	}

	m_sprite.SetImage( m_render_image.GetImage() );
	m_sprite.SetSubRect( sf::IntRect( 0, 0, static_cast<int>( m_content_allocation.Width ), static_cast<int>( m_content_allocation.Height ) ) );
	m_sprite.SetPosition( GetContentAllocation().Left, GetContentAllocation().Top );

	m_recalc_content_allocation = false;
}

void ScrolledWindow::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& oldallocation ) {
	if( oldallocation.Top == GetAllocation().Top &&
			oldallocation.Left == GetAllocation().Left &&
			oldallocation.Width == GetAllocation().Width &&
			oldallocation.Height == GetAllocation().Height ) {
		return;
	}

	m_recalc_content_allocation = true;

	Invalidate();
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

	m_recalc_content_allocation = true;

	Invalidate();
}

void ScrolledWindow::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& target ) {
	float scroll_speed( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Stepper.Speed", shared_from_this() ) );

	m_horizontal_scrollbar->SetProperty( "Scrollbar.Stepper.Speed", scroll_speed );
	m_vertical_scrollbar->SetProperty( "Scrollbar.Stepper.Speed", scroll_speed );

	if( IsHorizontalScrollbarVisible() ) {
		m_horizontal_scrollbar->Expose( target );
	}

	if( IsVerticalScrollbarVisible() ) {
		m_vertical_scrollbar->Expose( target );
	}

	if( !GetChild() ) {
		return;
	}

	// Set viewing region of the surface
	sf::View view(
		sf::FloatRect(
			m_horizontal_scrollbar->GetValue(),
			m_vertical_scrollbar->GetValue(),
			m_render_image.GetWidth(),
			m_render_image.GetHeight()
		)
	);
	m_render_image.SetView( view );

	// Clear surface
	m_render_image.Clear( sf::Color( 0, 0, 0, 0 ) );

	// Render child to surface
	GetChild()->Expose( m_render_image );

	// Display
	m_render_image.Display();

	// Draw Sprite to target
	target.Draw( m_sprite );
}

}
