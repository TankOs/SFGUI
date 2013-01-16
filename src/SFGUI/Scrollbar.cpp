#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <limits>

namespace sfg {

Scrollbar::Scrollbar( const Adjustment::Ptr& adjustment, Orientation orientation ) :
	Range( orientation ),
	m_elapsed_time( 0.f ),
	m_slider_click_offset( 0.f ),
	m_page_decreasing( 0 ),
	m_page_increasing( 0 ),
	m_dragging( false ),
	m_decrease_pressed( false ),
	m_increase_pressed( false ),
	m_repeat_wait( true )
{
	if( adjustment ) {
		SetAdjustment( adjustment );
	}
}

Scrollbar::Ptr Scrollbar::Create( Orientation orientation ) {
	Scrollbar::Ptr ptr( new Scrollbar( Adjustment::Ptr(), orientation ) );
	return ptr;
}

Scrollbar::Ptr Scrollbar::Create( const Adjustment::Ptr& adjustment, Orientation orientation ) {
	Scrollbar::Ptr ptr( new Scrollbar( adjustment, orientation ) );
	return ptr;
}

const sf::FloatRect Scrollbar::GetSliderRect() const {
	float mimimum_slider_length( Context::Get().GetEngine().GetProperty<float>( "SliderMinimumLength", shared_from_this() ) );

	Adjustment::Ptr adjustment( GetAdjustment() );

	float current_value = adjustment->GetValue();
	float value_range = std::max( adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize(), .0f );
	float pages = value_range / adjustment->GetPageSize() + 1.f;

	if( GetOrientation() == HORIZONTAL ) {
		float stepper_length = GetAllocation().height;
		float trough_length = GetAllocation().width - 2.f * stepper_length;
		float slider_length = std::max( mimimum_slider_length, trough_length / pages );
		if( adjustment->GetPageSize() == .0f ) {
			slider_length = mimimum_slider_length;
		}

		float slider_x = stepper_length + ( trough_length - slider_length ) * ( current_value - adjustment->GetLower() ) / value_range;
		float slider_y = 0.f;

		if( value_range == .0f ) {
			slider_x = stepper_length;
		}

		return sf::FloatRect( slider_x, slider_y, slider_length, GetAllocation().height );
	}
	else {
		float stepper_length = GetAllocation().width;
		float trough_length = GetAllocation().height - 2.f * stepper_length;
		float slider_length = std::max( mimimum_slider_length, trough_length / pages );
		if( adjustment->GetPageSize() == .0f ) {
			slider_length = mimimum_slider_length;
		}

		float slider_x = 0.f;
		float slider_y = stepper_length + ( trough_length - slider_length ) * ( current_value - adjustment->GetLower() ) / value_range;

		if( value_range == .0f ) {
			slider_y = stepper_length;
		}

		return sf::FloatRect( slider_x, slider_y, GetAllocation().width, slider_length );
	}
}

bool Scrollbar::IsDecreaseStepperPressed() const {
	return m_decrease_pressed;
}

bool Scrollbar::IsIncreaseStepperPressed() const {
	return m_increase_pressed;
}


RenderQueue* Scrollbar::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateScrollbarDrawable( DynamicPointerCast<const Scrollbar>( shared_from_this() ) );
}

sf::Vector2f Scrollbar::CalculateRequisition() {
	float mimimum_slider_length( Context::Get().GetEngine().GetProperty<float>( "SliderMinimumLength", shared_from_this() ) );

	// Scrollbars should always have a custom requisition set for it's shorter side.
	// If the dev forgets to set one show him where the scrollbar slider is so
	// it is easier for him to fix.
	return sf::Vector2f( mimimum_slider_length, mimimum_slider_length );
}

void Scrollbar::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) {
	if( button != sf::Mouse::Left ) {
		return;
	}

	if( press ) {
		sf::FloatRect slider_rect = GetSliderRect();
		slider_rect.left += GetAllocation().left;
		slider_rect.top += GetAllocation().top;

		if( slider_rect.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			m_dragging = true;

			if( GetOrientation() == HORIZONTAL ) {
				float slider_mid = slider_rect.left + slider_rect.width / 2.f;
				m_slider_click_offset = static_cast<float>( x ) + GetAllocation().left - slider_mid;
			}
			else {
				float slider_mid = slider_rect.top + slider_rect.height / 2.f;
				m_slider_click_offset = static_cast<float>( y ) + GetAllocation().top - slider_mid;
			}

			return;
		}

		if( GetOrientation() == HORIZONTAL ) {
			float stepper_length = GetAllocation().height;

			sf::FloatRect decrease_stepper_rect( GetAllocation().left, GetAllocation().top, stepper_length, GetAllocation().height );
			sf::FloatRect increase_stepper_rect( GetAllocation().left + GetAllocation().width - stepper_length, GetAllocation().top, stepper_length, GetAllocation().height );

			if( decrease_stepper_rect.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				m_decrease_pressed = true;
				GetAdjustment()->Decrement();
				m_elapsed_time = 0.f;
				m_repeat_wait = true;
				Invalidate();
				return;
			}

			if( increase_stepper_rect.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				m_increase_pressed = true;
				GetAdjustment()->Increment();
				m_elapsed_time = 0.f;
				m_repeat_wait = true;
				Invalidate();
				return;
			}
		}
		else {
			float stepper_length = GetAllocation().width;

			sf::FloatRect decrease_stepper_rect( GetAllocation().left, GetAllocation().top, GetAllocation().width, stepper_length );
			sf::FloatRect increase_stepper_rect( GetAllocation().left, GetAllocation().top + GetAllocation().height - stepper_length, GetAllocation().width, stepper_length );

			if( decrease_stepper_rect.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				m_decrease_pressed = true;
				GetAdjustment()->Decrement();
				m_elapsed_time = 0.f;
				m_repeat_wait = true;
				Invalidate();
				return;
			}

			if( increase_stepper_rect.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				m_increase_pressed = true;
				GetAdjustment()->Increment();
				m_elapsed_time = 0.f;
				m_repeat_wait = true;
				Invalidate();
				return;
			}
		}

		float slider_center_x = slider_rect.left + slider_rect.width / 2.f;
		float slider_center_y = slider_rect.top + slider_rect.height / 2.f;

		if( GetOrientation() == HORIZONTAL ) {
			if( GetAllocation().contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				if( static_cast<float>( x ) < slider_center_x ) {
					m_page_decreasing = x;
					GetAdjustment()->DecrementPage();
					m_elapsed_time = 0.f;
					m_repeat_wait = true;
					Invalidate();
					return;
				}
				else {
					m_page_increasing = x;
					GetAdjustment()->IncrementPage();
					m_elapsed_time = 0.f;
					m_repeat_wait = true;
					Invalidate();
					return;
				}
			}
		}
		else {
			if( GetAllocation().contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				if( static_cast<float>( y ) < slider_center_y ) {
					m_page_decreasing = y;
					GetAdjustment()->DecrementPage();
					m_elapsed_time = 0.f;
					m_repeat_wait = true;
					Invalidate();
					return;
				}
				else {
					m_page_increasing = y;
					GetAdjustment()->IncrementPage();
					m_elapsed_time = 0.f;
					m_repeat_wait = true;
					Invalidate();
					return;
				}
			}
		}
	}
	else {
		m_dragging = false;
		m_decrease_pressed = false;
		m_increase_pressed = false;
		m_page_decreasing = 0;
		m_page_increasing = 0;

		m_slider_click_offset = 0.f;

		Invalidate();
		return;
	}
}

void Scrollbar::HandleMouseMoveEvent( int x, int y ) {
	if( !m_dragging || ( x == std::numeric_limits<int>::min() ) || ( y == std::numeric_limits<int>::min() ) ) {
		return;
	}

	Adjustment::Ptr adjustment( GetAdjustment() );
	sf::FloatRect slider_rect = GetSliderRect();

	float value_range = std::max( adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize(), adjustment->GetMinorStep() / 2.f );
	float steps = value_range / adjustment->GetMinorStep();

	if( GetOrientation() == HORIZONTAL ) {
		float stepper_length = GetAllocation().height;

		float slider_center_x = slider_rect.left + slider_rect.width / 2.0f;
		float step_distance = ( GetAllocation().width - 2.f * stepper_length ) / steps;

		float delta = static_cast<float>( x ) - ( slider_center_x + m_slider_click_offset );

		while( delta < ( -step_distance / 2 ) ) {
			adjustment->Decrement();
			delta += step_distance;
		}

		while( delta > ( step_distance / 2 ) ) {
			adjustment->Increment();
			delta -= step_distance;
		}
	}
	else {
		float stepper_length = GetAllocation().width;

		float slider_center_y = slider_rect.top + slider_rect.height / 2.0f;
		float step_distance = ( GetAllocation().height - 2.f * stepper_length ) / steps;

		float delta = static_cast<float>( y ) - ( slider_center_y + m_slider_click_offset );

		while( delta < ( -step_distance / 2 ) ) {
			adjustment->Decrement();
			delta += step_distance;
		}

		while( delta > ( step_distance / 2 ) ) {
			adjustment->Increment();
			delta -= step_distance;
		}
	}
}

void Scrollbar::HandleUpdate( float seconds ) {
	float stepper_speed( Context::Get().GetEngine().GetProperty<float>( "StepperSpeed", shared_from_this() ) );

	m_elapsed_time += seconds;

	if( m_elapsed_time < ( 1.f / stepper_speed ) ) {
		return;
	}

	if( m_repeat_wait ) {
		sf::Uint32 stepper_repeat_delay( Context::Get().GetEngine().GetProperty<sf::Uint32>( "StepperRepeatDelay", shared_from_this() ) );

		if( m_elapsed_time < ( static_cast<float>( stepper_repeat_delay ) / 1000.f ) ) {
			return;
		}

		m_repeat_wait = false;
	}

	m_elapsed_time = 0.f;

	// Increment / Decrement value while one of the steppers is pressed
	if( m_decrease_pressed ) {
		GetAdjustment()->Decrement();
		Invalidate();
		return;
	}
	else if( m_increase_pressed ) {
		GetAdjustment()->Increment();
		Invalidate();
		return;
	}

	sf::FloatRect slider_rect = GetSliderRect();
	slider_rect.left += GetAllocation().left;
	slider_rect.top += GetAllocation().top;

	// Increment / Decrement page while mouse is pressed on the trough
	if( m_page_decreasing ) {
		GetAdjustment()->DecrementPage();

		if( GetOrientation() == HORIZONTAL ) {
			if( slider_rect.left + slider_rect.width < static_cast<float>( m_page_decreasing ) ) {
				m_page_decreasing = 0;
			}
		}
		else {
			if( slider_rect.top + slider_rect.height < static_cast<float>( m_page_decreasing ) ) {
				m_page_decreasing = 0;
			}
		}

		Invalidate();
		return;
	}
	else if( m_page_increasing ) {
		GetAdjustment()->IncrementPage();

		if( GetOrientation() == HORIZONTAL ) {
			if( slider_rect.left + slider_rect.width > static_cast<float>( m_page_increasing ) ) {
				m_page_increasing = 0;
			}
		}
		else {
			if( slider_rect.top + slider_rect.height > static_cast<float>( m_page_increasing ) ) {
				m_page_increasing = 0;
			}
		}

		Invalidate();
		return;
	}
}

const std::string& Scrollbar::GetName() const {
	static const std::string name( "Scrollbar" );
	return name;
}

}
