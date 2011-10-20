#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ScrolledWindow::ScrolledWindow( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) :
	Container(),
	m_horizontal_scrollbar(),
	m_vertical_scrollbar(),
	m_policy( PolicyDefault ),
	m_placement( PlacementDefault ),
	m_render_texture(),
	m_sprite(),
	m_recalc_adjustments( false ),
	m_content_allocation(),
	m_recalc_content_allocation( false )
{
	OnSizeAllocate.Connect( &ScrolledWindow::HandleSizeAllocate, this );
	OnAdd.Connect( &ScrolledWindow::HandleAdd, this );
	m_expose_connection = OnExpose.Connect( &ScrolledWindow::HandleExpose, this );

	m_horizontal_scrollbar = Scrollbar::Create( horizontal_adjustment, Scrollbar::Horizontal );
	m_vertical_scrollbar = Scrollbar::Create( vertical_adjustment, Scrollbar::Vertical );
}

ScrolledWindow::Ptr ScrolledWindow::Create() {
	ScrolledWindow::Ptr  ptr( new ScrolledWindow( Adjustment::Create(), Adjustment::Create() ) );

	ptr->Add( ptr->m_horizontal_scrollbar );
	ptr->Add( ptr->m_vertical_scrollbar );

	return ptr;
}

ScrolledWindow::Ptr ScrolledWindow::Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) {
	ScrolledWindow::Ptr  ptr( new ScrolledWindow( horizontal_adjustment, vertical_adjustment ) );

	ptr->Add( ptr->m_horizontal_scrollbar );
	ptr->Add( ptr->m_vertical_scrollbar );

	return ptr;
}

Adjustment::Ptr ScrolledWindow::GetHorizontalAdjustment() const {
	return m_horizontal_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetHorizontalAdjustment( Adjustment::Ptr adjustment ) {
	m_horizontal_scrollbar->SetAdjustment( adjustment );

	m_recalc_content_allocation = true;
	Invalidate();
}

Adjustment::Ptr ScrolledWindow::GetVerticalAdjustment() const {
	return m_vertical_scrollbar->GetAdjustment();
}

void ScrolledWindow::SetVerticalAdjustment( Adjustment::Ptr adjustment ) {
	m_vertical_scrollbar->SetAdjustment( adjustment );

	m_recalc_content_allocation = true;
	Invalidate();
}

int ScrolledWindow::GetScrollbarPolicy() const {
	return m_policy;
}

void ScrolledWindow::SetScrollbarPolicy( int policy ) {
	m_policy = policy;

	m_recalc_content_allocation = true;
	Invalidate();
}

void ScrolledWindow::SetPlacement( Placement placement ) {
	if( ( static_cast<bool>( placement & Top ) ^ static_cast<bool>( placement & Bottom ) ) &&
			( static_cast<bool>( placement & Left ) ^ static_cast<bool>( placement & Right ) ) ) {
		m_placement = placement;

		m_recalc_content_allocation = true;
		Invalidate();
	}
}

bool ScrolledWindow::IsHorizontalScrollbarVisible() const {
	if( m_policy & HorizontalAlways ) {
		return true;
	}

	if( m_policy & HorizontalNever ) {
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
	if( m_policy & VerticalAlways ) {
		return true;
	}

	if( m_policy & VerticalNever ) {
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

ScrolledWindow::HandleEventResult ScrolledWindow::HandleEvent( const sf::Event& event ) {
	if( Container::HandleEvent( event ) == EatEvent ) {
		return EatEvent;
	}

	// Pass event to child
	if( GetChild() ) {
		sf::FloatRect child_rect = GetContentAllocation();

		float offset_x = ( -m_content_allocation.Left + m_horizontal_scrollbar->GetValue() );
		float offset_y = ( -m_content_allocation.Top + m_vertical_scrollbar->GetValue() );

		switch( event.Type ) {
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased: { // All MouseButton events
			if( !child_rect.Contains( static_cast<float>( event.MouseButton.X ), static_cast<float>( event.MouseButton.Y ) ) ) {
				break;
			}

			sf::Event altered_event( event );
			altered_event.MouseButton.X += static_cast<int>( offset_x );
			altered_event.MouseButton.Y += static_cast<int>( offset_y );

			return GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
		case sf::Event::MouseMoved: { // All MouseMove events
			if( !child_rect.Contains( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) {
				break;
			}

			sf::Event altered_event( event );
			altered_event.MouseMove.X += static_cast<int>( offset_x );
			altered_event.MouseMove.Y += static_cast<int>( offset_y );

			return GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseWheelMoved: { // All MouseWheel events
			if( !child_rect.Contains( static_cast<float>( event.MouseWheel.X ), static_cast<float>( event.MouseWheel.Y ) ) ) {
				break;
			}

			sf::Event altered_event( event );
			altered_event.MouseWheel.X += static_cast<int>( offset_x );
			altered_event.MouseWheel.Y += static_cast<int>( offset_y );

			return GetChild()->HandleEvent( altered_event );
		} break;
		default: { // Pass event unaltered if it is a non-mouse event
			return GetChild()->HandleEvent( event );
		} break;
		}
	}

	// Nothing wanted the Event
	return PassEvent;
}

sf::Drawable* ScrolledWindow::InvalidateImpl() {
	if( m_recalc_adjustments ) {
		RecalculateAdjustments();
	}

	if( m_recalc_content_allocation ) {
		RecalculateContentAllocation();
	}

	return Context::Get().GetEngine().CreateScrolledWindowDrawable( boost::shared_dynamic_cast<ScrolledWindow>( shared_from_this() ) );
}

sf::Vector2f ScrolledWindow::GetRequisitionImpl() const {
	// A child or parent just got resized/moved,
	// have to recalculate everything.
	// TODO: This is wrong. Parent just asks for requisition here. Use
	// OnSizeRequest to handle size requests.
	m_recalc_adjustments = true;
	m_recalc_content_allocation = true;

	// The only child, always gets what it wants ;)
	if( GetChild() ) {
		sf::FloatRect new_allocation = GetChild()->GetAllocation();
		new_allocation.Width = GetChild()->GetRequisition().x;
		new_allocation.Height = GetChild()->GetRequisition().y;
		GetChild()->AllocateSize( new_allocation );
	}

	return sf::Vector2f( 0.f, 0.f );
}

void ScrolledWindow::RecalculateAdjustments() const {
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Width", shared_from_this() ) );
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Spacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

	if( GetChild() ) {
		float max_horiz_val = std::max( GetChild()->GetAllocation().Width + 2.f, GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		Adjustment::Ptr h_adjustment = m_horizontal_scrollbar->GetAdjustment();
		h_adjustment->SetLower( .0f );
		h_adjustment->SetUpper( max_horiz_val );
		h_adjustment->SetMinorStep( 1.f );
		h_adjustment->SetMajorStep( GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		h_adjustment->SetPageSize( GetAllocation().Width - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		float max_vert_val = std::max( GetChild()->GetAllocation().Height + 2.f, GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );

		Adjustment::Ptr v_adjustment = m_vertical_scrollbar->GetAdjustment();
		v_adjustment->SetLower( .0f );
		v_adjustment->SetUpper( max_vert_val );
		v_adjustment->SetMinorStep( 1.f );
		v_adjustment->SetMajorStep( GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
		v_adjustment->SetPageSize( GetAllocation().Height - scrollbar_width - scrollbar_spacing - border_width * 2.f );
	}

	m_recalc_adjustments = false;
	m_recalc_content_allocation = true;
}

void ScrolledWindow::RecalculateContentAllocation() {
	float scrollbar_spacing( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Spacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.BorderWidth", shared_from_this() ) );

	m_content_allocation = GetAllocation();

	m_content_allocation.Left += border_width;
	m_content_allocation.Top += border_width;
	m_content_allocation.Width -= 2.f * border_width;
	m_content_allocation.Height -= 2.f * border_width;

	if( IsVerticalScrollbarVisible() ) {
		m_content_allocation.Width -= ( m_vertical_scrollbar->GetRequisition().x + scrollbar_spacing );

		if( m_placement & Right ) { // Content placed at Right
			m_content_allocation.Left += ( m_vertical_scrollbar->GetRequisition().x + scrollbar_spacing );
		}
	}

	if( IsHorizontalScrollbarVisible() ) {
		m_content_allocation.Height -= ( m_horizontal_scrollbar->GetRequisition().x + scrollbar_spacing );

		if( m_placement & Bottom ) { // Content placed at Bottom
			m_content_allocation.Top += ( m_horizontal_scrollbar->GetRequisition().x + scrollbar_spacing );
		}
	}

	if( m_placement & Top ) { // Content placed at Top
		m_horizontal_scrollbar->AllocateSize(
			sf::FloatRect(
				m_content_allocation.Left - GetAllocation().Left - border_width,
				m_content_allocation.Height + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Width + 2.f * border_width,
				m_horizontal_scrollbar->GetRequisition().y
			)
		);
	}
	else { // Content placed at Bottom
		m_horizontal_scrollbar->AllocateSize(
			sf::FloatRect(
				m_content_allocation.Left - GetAllocation().Left - border_width,
				0.f,
				m_content_allocation.Width + 2.f * border_width,
				m_horizontal_scrollbar->GetRequisition().y
			)
		);
	}

	if( m_placement & Left ) { // Content placed at Left
		m_vertical_scrollbar->AllocateSize(
			sf::FloatRect(
				m_content_allocation.Width + 2.f * border_width + scrollbar_spacing,
				m_content_allocation.Top - GetAllocation().Top - border_width,
				m_vertical_scrollbar->GetRequisition().x,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	}
	else { // Content placed at Right
		m_vertical_scrollbar->AllocateSize(
			sf::FloatRect(
				0.f,
				m_content_allocation.Top - GetAllocation().Top - border_width,
				m_vertical_scrollbar->GetRequisition().x,
				m_content_allocation.Height + 2.f * border_width
			)
		);
	}

	// Update Scrollbars if the content allocation changed.
	if( GetChild() ) {
		m_horizontal_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.Width );
		m_horizontal_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.Width );

		m_vertical_scrollbar->GetAdjustment()->SetMajorStep( m_content_allocation.Height );
		m_vertical_scrollbar->GetAdjustment()->SetPageSize( m_content_allocation.Height );
	}

	// Only recreate the RenderImage if the content allocation size changed.
	if( ( m_content_allocation.Width != static_cast<float>( m_render_texture.GetWidth() ) ) || ( m_content_allocation.Height != static_cast<float>( m_render_texture.GetHeight() ) ) ) {
		// Avoid creating images with non-positive size and assure compatibility
		// on systems which only support multiple-of-2 texture sizes.
		bool result = m_render_texture.Create(
			static_cast<unsigned int>( std::max( m_content_allocation.Width, 2.f ) ),
			static_cast<unsigned int>( std::max( m_content_allocation.Height, 2.f ) )
		);

#ifdef SFGUI_DEBUG
		if( !result ) {
			std::cerr << "Failed to create RenderImage." << std::endl;
		}
#endif

		m_sprite.SetTexture( m_render_texture.GetTexture() );
		m_sprite.SetSubRect( sf::IntRect( 0, 0, static_cast<int>( m_content_allocation.Width ), static_cast<int>( m_content_allocation.Height ) ) );
	}

	m_recalc_content_allocation = false;
}

void ScrolledWindow::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& oldallocation ) {
	// Don't go through expensive content recalculation if the allocation
	// of the ScrolledWindow didn't even change. HandleSizeAllocate is used
	// by some Widgets to signal other kinds of changes.
	if( oldallocation.Top == GetAllocation().Top &&
			oldallocation.Left == GetAllocation().Left &&
			oldallocation.Width == GetAllocation().Width &&
			oldallocation.Height == GetAllocation().Height ) {
		return;
	}

	m_recalc_content_allocation = true;
	Invalidate();
}

void ScrolledWindow::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& target ) {
	float scroll_speed( Context::Get().GetEngine().GetProperty<float>( "ScrolledWindow.Scrollbar.Stepper.Speed", shared_from_this() ) );

	// This is just a temporary solution for setting the scroll speed
	// of the Scrollbars until a better property system comes around.
	m_horizontal_scrollbar->SetProperty( "Scrollbar.Stepper.Speed", scroll_speed );
	m_vertical_scrollbar->SetProperty( "Scrollbar.Stepper.Speed", scroll_speed );

	// Draw the Scrollbars
	if( IsHorizontalScrollbarVisible() ) {
		m_horizontal_scrollbar->Expose( target );
	}

	if( IsVerticalScrollbarVisible() ) {
		m_vertical_scrollbar->Expose( target );
	}

	// If there is no child no need to proceed beyond here.
	if( !GetChild() ) {
		return;
	}

	// Set viewing region of the surface
	sf::View view(
		sf::FloatRect(
			m_horizontal_scrollbar->GetValue(),
			m_vertical_scrollbar->GetValue(),
			static_cast<float>( m_render_texture.GetWidth() ),
			static_cast<float>( m_render_texture.GetHeight() )
		)
	);
	m_render_texture.SetView( view );

	// Clear surface
	m_render_texture.Clear( sf::Color( 0, 0, 0, 0 ) );

	// Convert child position to relative
	GetChild()->SetPosition( sf::Vector2f( -GetAbsolutePosition().x, -GetAbsolutePosition().y ) );

	// Render child to surface
	GetChild()->Expose( m_render_texture );

	// Convert child position back to absolute
	GetChild()->SetPosition( sf::Vector2f( 0.f, 0.f ) );

	// Display
	m_render_texture.Display();

	// Set Sprite position
	sf::Vector2f relative_position( GetAbsolutePosition().x + GetContentAllocation().Left - GetAllocation().Left, GetAbsolutePosition().y + GetContentAllocation().Top - GetAllocation().Top );

	m_sprite.SetPosition( relative_position );

	// Draw Sprite to target
	target.Draw( m_sprite );
}

void ScrolledWindow::HandleAdd( Widget::Ptr /*widget*/, Widget::Ptr child ) {
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

Widget::Ptr ScrolledWindow::GetChild() const {
	if( GetChildren().size() < 3 ) {
		return Widget::Ptr();
	}

	return *( --GetChildren().end() );
}

const std::string& ScrolledWindow::GetName() const {
	static const std::string name( "ScrolledWindow" );
	return name;
}

}
