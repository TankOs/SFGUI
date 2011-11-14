#include <SFGUI/Viewport.hpp>

namespace sfg {

Viewport::Viewport( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) :
	Bin()
{
	OnSizeRequest.Connect( &Viewport::HandleSizeRequest, this );

	SetHorizontalAdjustment( horizontal_adjustment );
	SetVerticalAdjustment( vertical_adjustment );
}

Viewport::Ptr Viewport::Create() {
	return Viewport::Create( Adjustment::Create(), Adjustment::Create() );
}

Viewport::Ptr Viewport::Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) {
	Viewport::Ptr  ptr( new Viewport( horizontal_adjustment, vertical_adjustment ) );
	return ptr;
}

sf::Vector2f Viewport::GetRequisitionImpl() const {
	return sf::Vector2f( 0.f, 0.f );
}

void Viewport::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) const {
}

void Viewport::Expose( CullingTarget& target ) const {
	if( IsVisible() ) {
		if( GetChild() ) {
			unsigned int target_width = target.GetWidth();
			unsigned int target_height = target.GetHeight();

			sf::View view(
				sf::FloatRect(
					std::floor( m_horizontal_adjustment->GetValue() + .5f ),
					std::floor( m_vertical_adjustment->GetValue() + .5f ),
					GetAllocation().Width,
					GetAllocation().Height
				)
			);

			float viewport_left = std::floor( Widget::GetAbsolutePosition().x + .5f ) / static_cast<float>( target_width );
			float viewport_top = std::floor( Widget::GetAbsolutePosition().y + .5f ) / static_cast<float>( target_height );
			float viewport_width = GetAllocation().Width / static_cast<float>( target_width );
			float viewport_height = GetAllocation().Height / static_cast<float>( target_height );

			// Make sure float to int rounding errors can't lead to streching effects.
			if( static_cast<int>( viewport_width * static_cast<float>( target_width ) ) < static_cast<int>( GetAllocation().Width ) ) {
				viewport_width += 1.f / static_cast<float>( target_width );
			}

			if( static_cast<int>( viewport_height * static_cast<float>( target_height ) ) < static_cast<int>( GetAllocation().Height ) ) {
				viewport_height += 1.f / static_cast<float>( target_height );
			}

			view.SetViewport(
				sf::FloatRect(
					viewport_left,
					viewport_top,
					viewport_width,
					viewport_height
				)
			);

			sf::View original_view = target.GetView();

			target.SetView( view );

			GetChild()->Expose( target );

			target.SetView( original_view );
		}

		OnExpose();
	}
}

void Viewport::HandleEvent( const sf::Event& event ) {
	// Ignore event when widget is not visible.
	if( !IsVisible() ) {
		return;
	}

	// Pass event to child
	if( GetChild() ) {
		float offset_x = ( -GetAllocation().Left + m_horizontal_adjustment->GetValue() );
		float offset_y = ( -GetAllocation().Top + m_vertical_adjustment->GetValue() );

		switch( event.Type ) {
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased: { // All MouseButton events
			if( !GetAllocation().Contains( static_cast<float>( event.MouseButton.X ), static_cast<float>( event.MouseButton.Y ) ) ) {
				break;
			}

			sf::Event altered_event( event );
			altered_event.MouseButton.X += static_cast<int>( offset_x );
			altered_event.MouseButton.Y += static_cast<int>( offset_y );

			GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
		case sf::Event::MouseMoved: { // All MouseMove events
			if( !GetAllocation().Contains( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) {
				// Nice hack to cause scrolledwindow children to get out of
				// prelight state when the mouse leaves the child allocation.
				sf::Event altered_event( event );
				altered_event.MouseMove.X = -1;
				altered_event.MouseMove.Y = -1;

				GetChild()->HandleEvent( altered_event );
				break;
			}

			sf::Event altered_event( event );
			altered_event.MouseMove.X += static_cast<int>( offset_x );
			altered_event.MouseMove.Y += static_cast<int>( offset_y );

			GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseWheelMoved: { // All MouseWheel events
			if( !GetAllocation().Contains( static_cast<float>( event.MouseWheel.X ), static_cast<float>( event.MouseWheel.Y ) ) ) {
				break;
			}

			sf::Event altered_event( event );
			altered_event.MouseWheel.X += static_cast<int>( offset_x );
			altered_event.MouseWheel.Y += static_cast<int>( offset_y );

			GetChild()->HandleEvent( altered_event );
		} break;
		default: { // Pass event unaltered if it is a non-mouse event
			GetChild()->HandleEvent( event );
		} break;
		}
	}
}

sf::Vector2f Viewport::GetAbsolutePosition() const {
	return sf::Vector2f( .0f, .0f );
}

Adjustment::Ptr Viewport::GetHorizontalAdjustment() const {
	return m_horizontal_adjustment;
}

void Viewport::SetHorizontalAdjustment( Adjustment::Ptr horizontal_adjustment ) {
	m_horizontal_adjustment = horizontal_adjustment;
}

Adjustment::Ptr Viewport::GetVerticalAdjustment() const {
	return m_vertical_adjustment;
}

void Viewport::SetVerticalAdjustment( Adjustment::Ptr vertical_adjustment ) {
	m_vertical_adjustment = vertical_adjustment;
}

void Viewport::HandleSizeRequest() {
	// A child just requested it's size. Because we are a viewport
	// and have a virtual screen we give it everything it wants.
	if( GetChild() ) {
		sf::FloatRect new_allocation = GetChild()->GetAllocation();
		new_allocation.Width = GetChild()->GetRequisition().x;
		new_allocation.Height = GetChild()->GetRequisition().y;
		GetChild()->AllocateSize( new_allocation );
	}
}

const std::string& Viewport::GetName() const {
	static const std::string name( "Viewport" );
	return name;
}

}
