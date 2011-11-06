#include <SFGUI/Viewport.hpp>

namespace sfg {

Viewport::Viewport() :
	Bin(),
	m_render_texture(),
	m_sprite(),
	m_rebuild_child( true )
{
	SetHorizontalAdjustment( Adjustment::Create() );
	SetVerticalAdjustment( Adjustment::Create() );
}

Viewport::Ptr Viewport::Create() {
	Viewport::Ptr  ptr( new Viewport );

	return ptr;
}

sf::Vector2f Viewport::GetRequisitionImpl() const {
	return sf::Vector2f( 0.f, 0.f );
}

void Viewport::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) {
	RecreateRenderTexture();
}

void Viewport::Expose( sf::RenderTarget& target ) {
	if( IsVisible() ) {
		//
		// Cached Variant
		//
		/**/
		if( m_rebuild_child && GetChild() && GetParent() ) {
			// Set viewing region of the surface
			sf::View view(
				sf::FloatRect(
					m_horizontal_adjustment->GetValue(),
					m_vertical_adjustment->GetValue(),
					static_cast<float>( m_render_texture.GetWidth() ),
					static_cast<float>( m_render_texture.GetHeight() )
				)
			);

			m_render_texture.SetView( view );

			// Clear surface
			m_render_texture.Clear( sf::Color( 0, 0, 0, 0 ) );

			// Render child to surface
			GetChild()->Expose( m_render_texture );

			// Display
			m_render_texture.Display();

			m_rebuild_child = false;
		}

		// Set Sprite position
		sf::Vector2f relative_position = GetParent()->GetAbsolutePosition() + sf::Vector2f( GetAllocation().Left, GetAllocation().Top );
		m_sprite.SetPosition( relative_position );

		// Draw Sprite to target
		target.Draw( m_sprite );
		/**/

		//
		// Uncached Variant
		//

		/*
		if( GetChild() && GetParent() ) {
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

			sf::Vector2f relative_position = GetParent()->GetAbsolutePosition() + sf::Vector2f( GetAllocation().Left, GetAllocation().Top );

			view.SetViewport(
				sf::FloatRect(
					std::floor( relative_position.x + .5f ) / static_cast<float>( target_width ),
					std::floor( relative_position.y + .5f ) / static_cast<float>( target_height ),
					GetAllocation().Width / static_cast<float>( target_width ),
					GetAllocation().Height / static_cast<float>( target_height )
				)
			);

			sf::View original_view = target.GetView();

			target.SetView( view );

			GetChild()->Expose( target );

			target.SetView( original_view );
		}
		*/

		OnExpose();
	}
}

void Viewport::HandleEvent( const sf::Event& event ) {
	// Ignore event when widget is not visible.
	if( !IsVisible() ) {
		return;
	}

	// Pass event to child
	if( GetParent() && GetChild() ) {
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

void Viewport::RecreateRenderTexture() {
	// Only recreate the RenderTexture if the content allocation size changed.
	if( ( GetAllocation().Width != static_cast<float>( m_render_texture.GetWidth() ) ) || ( GetAllocation().Height != static_cast<float>( m_render_texture.GetHeight() ) ) ) {
		// Avoid creating images with non-positive size and assure compatibility
		// on systems which only support multiple-of-2 texture sizes.
		if(
			!m_render_texture.Create(
				static_cast<unsigned int>( std::max( GetAllocation().Width, 2.f ) ),
				static_cast<unsigned int>( std::max( GetAllocation().Height, 2.f ) )
			)
		) {
#ifdef SFGUI_DEBUG
			std::cerr << "Failed to create RenderImage." << std::endl;
#endif
		}

		m_sprite.SetTexture( m_render_texture.GetTexture() );
		m_sprite.SetSubRect( sf::IntRect( 0, 0, static_cast<int>( GetAllocation().Width ), static_cast<int>( GetAllocation().Height ) ) );
	}
}

void Viewport::SetHorizontalAdjustment( Adjustment::Ptr horizontal_adjustment ) {
	m_horizontal_adjustment = horizontal_adjustment;
	m_horizontal_adjustment->OnChange.Connect( &Viewport::HandleAdjustmentChange, this );
}

void Viewport::SetVerticalAdjustment( Adjustment::Ptr vertical_adjustment ) {
	m_vertical_adjustment = vertical_adjustment;
	m_vertical_adjustment->OnChange.Connect( &Viewport::HandleAdjustmentChange, this );
}

void Viewport::HandleChildInvalidate( Widget::Ptr child  ) {
	m_rebuild_child = true;

	Container::HandleChildInvalidate( child );
}

void Viewport::HandleAdjustmentChange() {
	m_rebuild_child = true;
}

const std::string& Viewport::GetName() const {
	static const std::string name( "Viewport" );
	return name;
}

}
