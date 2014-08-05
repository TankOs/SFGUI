#include <SFGUI/Scale.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

#include <limits>

namespace sfg {

Scale::Scale( Orientation orientation ) :
	Range( orientation ),
	m_dragging( false )
{
}

Scale::Ptr Scale::Create( Orientation orientation ) {
	Ptr ptr( new Scale( orientation ) );
	ptr->SetAdjustment( Adjustment::Create() );
	return ptr;
}

Scale::Ptr Scale::Create( float min, float max, float step, Orientation orientation ) {
	Ptr ptr( new Scale( orientation ) );
	ptr->SetAdjustment( Adjustment::Create() );
	ptr->SetRange( min, max );
	ptr->SetIncrements( step, 0.f );
	return ptr;
}

const sf::FloatRect Scale::GetSliderRect() const {
	auto slider_length = Context::Get().GetEngine().GetProperty<float>( "SliderLength", shared_from_this() );
	auto slider_width = (GetOrientation() == Orientation::HORIZONTAL) ? GetAllocation().height : GetAllocation().width;
	auto adjustment = GetAdjustment();
	auto current_value = adjustment->GetValue();
	auto value_range = adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize();

	if( GetOrientation() == Orientation::HORIZONTAL ) {
		auto slider_x = (GetAllocation().width - slider_length) * (current_value - adjustment->GetLower()) / value_range;
		auto slider_y = (GetAllocation().height - slider_width) / 2.f;

		return sf::FloatRect( slider_x, slider_y, slider_length, slider_width );
	}

	auto slider_x = (GetAllocation().width - slider_width) / 2.f;
	auto slider_y = (GetAllocation().height - slider_length) * (1 - ((current_value - adjustment->GetLower()) / value_range));

	return sf::FloatRect( slider_x, slider_y, slider_width, slider_length );
}


std::unique_ptr<RenderQueue> Scale::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateScaleDrawable( std::dynamic_pointer_cast<const Scale>( shared_from_this() ) );
}

sf::Vector2f Scale::CalculateRequisition() {
	auto slider_length = Context::Get().GetEngine().GetProperty<float>( "SliderLength", shared_from_this() );
	auto slider_width = std::max( 3.f, ( GetOrientation() == Orientation::HORIZONTAL ) ? GetAllocation().height : GetAllocation().width );

	if( GetOrientation() == Orientation::HORIZONTAL ) {
		return sf::Vector2f( slider_length * 2.f, slider_width );
	}

	return sf::Vector2f( slider_width, slider_length * 2.f );
}

void Scale::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) {
	if( button != sf::Mouse::Left ) {
		return;
	}

	if( m_drag_offset ) {
		m_drag_offset.reset();
		m_dragging = false;
	}

	if( !GetAllocation().contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
		return;
	}

	if( press ) {
		if( !GetSliderRect().contains( static_cast<float>( x ) - GetAllocation().left, static_cast<float>( y ) - GetAllocation().top ) ) {
			Adjustment::Ptr adjustment( GetAdjustment() );

			auto minor_step = adjustment->GetMinorStep();
			auto range = adjustment->GetUpper() - adjustment->GetLower();
			auto steps = range / minor_step;
			auto needed_steps = 0.f;

			auto trough_position = 0.f;
			auto trough_length = 0.f;

			if( GetOrientation() == Orientation::HORIZONTAL ) {
				trough_position = static_cast<float>( x ) - ( GetAllocation().left + GetSliderRect().width / 2.f );
				trough_length = GetAllocation().width - GetSliderRect().width;
			}

			if( GetOrientation() == Orientation::VERTICAL ) {
				trough_position = static_cast<float>( y ) - ( GetAllocation().top + GetSliderRect().height / 2.f );
				trough_length = GetAllocation().height - GetSliderRect().height;
			}

			trough_position = std::min( trough_position, trough_length );

			auto trough_ratio = trough_position / trough_length;

			for( ; needed_steps < steps; needed_steps += 1.f ) {
				if( ( 1.f / steps ) * needed_steps > trough_ratio ) {
					break;
				}
			}

			needed_steps = std::max( needed_steps - 1.f, 0.f );

			adjustment->SetValue( needed_steps * minor_step );
		}

		m_dragging = true;
		m_drag_offset.reset( new sf::Vector2f(
				static_cast<float>( x ) - ( GetAllocation().left + GetSliderRect().left + GetSliderRect().width / 2.f ),
				static_cast<float>( y ) - ( GetAllocation().top + GetSliderRect().top + GetSliderRect().height / 2.f )
		) );
	}
}

void Scale::HandleMouseMoveEvent( int x, int y ) {
	if( !m_dragging || ( x == std::numeric_limits<int>::min() ) || ( y == std::numeric_limits<int>::min() ) ) {
		return;
	}

	Adjustment::Ptr adjustment( GetAdjustment() );
	auto slider_rect = GetSliderRect();

	auto value_range = std::max( adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize(), adjustment->GetMinorStep() / 2.f );
	auto steps = value_range / adjustment->GetMinorStep();

	if( GetOrientation() == Orientation::HORIZONTAL ) {
		auto slider_center_x = GetAllocation().left + slider_rect.left + slider_rect.width / 2.0f;
		auto step_distance = (GetAllocation().width - slider_rect.width) / steps;
		auto delta = static_cast<float>( x ) - slider_center_x - m_drag_offset->x;

		while( delta < (-step_distance / 2) ) {
			adjustment->Decrement();
			delta += step_distance;
		}

		while( delta > (step_distance / 2) ) {
			adjustment->Increment();
			delta -= step_distance;
		}
	}
	else {
		auto slider_center_y = GetAllocation().top + slider_rect.top + slider_rect.height / 2.0f;
		auto step_distance = (GetAllocation().height - slider_rect.height) / steps;
		auto delta = static_cast<float>( y ) - slider_center_y - m_drag_offset->y;

		while( delta < (-step_distance / 2) ) {
			adjustment->Increment();
			delta += step_distance;
		}

		while( delta > (step_distance / 2) ) {
			adjustment->Decrement();
			delta -= step_distance;
		}
	}
}

const std::string& Scale::GetName() const {
	static const std::string name( "Scale" );
	return name;
}

}
