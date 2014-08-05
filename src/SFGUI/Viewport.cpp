#include <SFGUI/Viewport.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Window/Event.hpp>
#include <cmath>

namespace sfg {

Viewport::Viewport() :
	m_horizontal_adjustment_signal_serial( 0 ),
	m_vertical_adjustment_signal_serial( 0 )
{
	m_children_viewport = Renderer::Get().CreateViewport();
}

Viewport::Ptr Viewport::Create() {
	return Viewport::Create( Adjustment::Create(), Adjustment::Create() );
}

Viewport::Ptr Viewport::Create( Adjustment::Ptr horizontal_adjustment, Adjustment::Ptr vertical_adjustment ) {
	auto ptr = Ptr( new Viewport );

	ptr->SetHorizontalAdjustment( horizontal_adjustment );
	ptr->SetVerticalAdjustment( vertical_adjustment );

	return ptr;
}

std::unique_ptr<RenderQueue> Viewport::InvalidateImpl() const {
	m_children_viewport->SetSourceOrigin(
		sf::Vector2f(
			std::floor( m_horizontal_adjustment->GetValue() + .5f ),
			std::floor( m_vertical_adjustment->GetValue() + .5f )
		)
	);

	return nullptr;
}

sf::Vector2f Viewport::CalculateRequisition() {
	return sf::Vector2f( 0.f, 0.f );
}

void Viewport::HandleSizeChange() {
	auto allocation = GetAllocation();

	m_children_viewport->SetSize(
		sf::Vector2f(
			std::floor( allocation.width + .5f ),
			std::floor( allocation.height + .5f )
		)
	);
}

void Viewport::HandleAbsolutePositionChange() {
	auto position = Widget::GetAbsolutePosition();

	m_children_viewport->SetDestinationOrigin(
		sf::Vector2f(
			std::floor( position.x + .5f ),
			std::floor( position.y + .5f )
		)
	);

	// Send AbsolutePositionChange notifications to children so they can
	// perform necessary actions e.g. Canvas.
	Container::HandleAbsolutePositionChange();
}

void Viewport::HandleEvent( const sf::Event& event ) {
	// Ignore event when widget is not visible.
	if( !IsGloballyVisible() ) {
		return;
	}

	// Pass event to child
	if( GetChild() ) {
		auto offset_x = ( -GetAllocation().left + m_horizontal_adjustment->GetValue() );
		auto offset_y = ( -GetAllocation().top + m_vertical_adjustment->GetValue() );

		switch( event.type ) {
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased: { // All MouseButton events
			if( !GetAllocation().contains( static_cast<float>( event.mouseButton.x ), static_cast<float>( event.mouseButton.y ) ) ) {
				break;
			}

			sf::Event altered_event( event );
			altered_event.mouseButton.x += static_cast<int>( offset_x );
			altered_event.mouseButton.y += static_cast<int>( offset_y );

			GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseLeft: {
			// Nice hack to cause scrolledwindow children to get out of
			// prelight state when the mouse leaves the child allocation.
			sf::Event altered_event( event );
			altered_event.mouseMove.x = -1;
			altered_event.mouseMove.y = -1;
			GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseMoved: { // All MouseMove events
			sf::Event altered_event( event );
			if( !GetAllocation().contains( static_cast<float>( event.mouseMove.x ), static_cast<float>( event.mouseMove.y ) ) ) {
				// Nice hack to cause scrolledwindow children to get out of
				// prelight state when the mouse leaves the child allocation.
				altered_event.mouseMove.x = -1;
				altered_event.mouseMove.y = -1;
			}
			else {
				altered_event.mouseMove.x += static_cast<int>( offset_x );
				altered_event.mouseMove.y += static_cast<int>( offset_y );
			}
			GetChild()->HandleEvent( altered_event );
		} break;
		case sf::Event::MouseWheelMoved: { // All MouseWheel events
			if( !GetAllocation().contains( static_cast<float>( event.mouseWheel.x ), static_cast<float>( event.mouseWheel.y ) ) ) {
				break;
			}

			sf::Event altered_event( event );
			altered_event.mouseWheel.x += static_cast<int>( offset_x );
			altered_event.mouseWheel.y += static_cast<int>( offset_y );

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
	if( m_horizontal_adjustment ) {
		m_horizontal_adjustment->GetSignal( Adjustment::OnChange ).Disconnect( m_horizontal_adjustment_signal_serial );
	}

	m_horizontal_adjustment = horizontal_adjustment;

	auto weak_this = std::weak_ptr<Widget>( shared_from_this() );

	m_horizontal_adjustment_signal_serial = m_horizontal_adjustment->GetSignal( Adjustment::OnChange ).Connect( [weak_this] {
		auto shared_this = weak_this.lock();

		if( !shared_this ) {
			return;
		}

		auto viewport = std::dynamic_pointer_cast<Viewport>( shared_this );

		if( !viewport ) {
			return;
		}

		viewport->UpdateView();
	} );
}

Adjustment::Ptr Viewport::GetVerticalAdjustment() const {
	return m_vertical_adjustment;
}

void Viewport::SetVerticalAdjustment( Adjustment::Ptr vertical_adjustment ) {
	if( m_vertical_adjustment ) {
		m_vertical_adjustment->GetSignal( Adjustment::OnChange ).Disconnect( m_vertical_adjustment_signal_serial );
	}

	m_vertical_adjustment = vertical_adjustment;

	auto weak_this = std::weak_ptr<Widget>( shared_from_this() );

	m_vertical_adjustment_signal_serial = m_vertical_adjustment->GetSignal( Adjustment::OnChange ).Connect( [weak_this] {
		auto shared_this = weak_this.lock();

		if( !shared_this ) {
			return;
		}

		auto viewport = std::dynamic_pointer_cast<Viewport>( shared_this );

		if( !viewport ) {
			return;
		}

		viewport->UpdateView();
	} );
}

void Viewport::HandleRequisitionChange() {
	// A child just requested it's size. Because we are a viewport
	// and have a virtual screen we give it everything it wants.
	if( GetChild() ) {
		auto new_allocation = GetChild()->GetAllocation();
		new_allocation.width = GetChild()->GetRequisition().x;
		new_allocation.height = GetChild()->GetRequisition().y;
		GetChild()->SetAllocation( new_allocation );
	}
}

const std::string& Viewport::GetName() const {
	static const std::string name( "Viewport" );
	return name;
}

bool Viewport::HandleAdd( Widget::Ptr child ) {
	if( !GetChildren().empty() ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Only one widget can be added to a Bin.\n";
#endif

		return false;
	}

	Container::HandleAdd( child );

	child->SetViewport( m_children_viewport );

	return true;
}

void Viewport::HandleViewportUpdate() {
	Widget::Ptr child( GetChild() );

	if( child ) {
		child->SetViewport( m_children_viewport );
	}
}

void Viewport::UpdateView() {
	m_children_viewport->SetSourceOrigin(
		sf::Vector2f(
			std::floor( m_horizontal_adjustment->GetValue() + .5f ),
			std::floor( m_vertical_adjustment->GetValue() + .5f )
		)
	);
}

sf::Vector2f Viewport::GetChildRequisition() {
	if( GetChild() ) {
		return GetChild()->GetRequisition();
	}

	return sf::Vector2f( 0.f, 0.f );
}

}
