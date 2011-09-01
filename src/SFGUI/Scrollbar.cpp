#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

Scrollbar::Scrollbar( Adjustment::Ptr adjustment, Orientation orientation ) :
	Range(),
	m_orientation( orientation ),
	m_length( .0f ),
	m_width( .0f ),
	m_dragging( false ),
	m_decrease_pressed( false ),
	m_increase_pressed( false ),
	m_page_decreasing( false ),
	m_page_increasing( false )
{
	OnMouseButtonPress.Connect( &Scrollbar::HandleMouseButtonPress, this );
	OnMouseButtonRelease.Connect( &Scrollbar::HandleMouseButtonRelease, this );
	OnMouseMove.Connect( &Scrollbar::HandleMouseMove, this );
	OnExpose.Connect( &Scrollbar::HandleExpose, this );

	SetAdjustment( adjustment );
}

Scrollbar::Ptr Scrollbar::Create( Orientation orientation ) {
	Scrollbar::Ptr  ptr( new Scrollbar( Adjustment::Ptr(), orientation ) );
	return ptr;
}

Scrollbar::Ptr Scrollbar::Create( Adjustment::Ptr adjustment, Orientation orientation ) {
	Scrollbar::Ptr  ptr( new Scrollbar( adjustment, orientation ) );
	return ptr;
}

float Scrollbar::GetLength() const {
	return m_length;
}

float Scrollbar::GetWidth() const {
	return m_width;
}

void Scrollbar::SetLength( float new_length ) {
	m_length = new_length;
	Invalidate();
}

void Scrollbar::SetWidth( float new_width ) {
	m_width = new_width;
	Invalidate();
}

const Scrollbar::Orientation Scrollbar::GetOrientation() const {
	return m_orientation;
}

const sf::FloatRect Scrollbar::GetSliderRect() const {
	float mimimum_slider_length( Context::Get().GetEngine().GetProperty<float>( "Scrollbar.Slider.MinimumLength", shared_from_this() ) );
	float stepper_length( Context::Get().GetEngine().GetProperty<float>( "Scrollbar.Stepper.Length", shared_from_this() ) );

	Adjustment::Ptr adjustment( GetAdjustment() );

	float current_value = adjustment->GetValue();
	float value_range = adjustment->GetUpper() - adjustment->GetLower();
	float pages = value_range / adjustment->GetPageSize() + 1.f;

	if( m_orientation == Horizontal ) {
		float trough_length = GetAllocation().Width - 2.f * stepper_length;
		float slider_length = std::max( mimimum_slider_length, trough_length / pages );

		float slider_x = stepper_length + ( trough_length - slider_length ) * current_value / value_range;
		float slider_y = 0.f;

		return sf::FloatRect( slider_x, slider_y, slider_length, GetWidth() );
	}
	else {
		float trough_length = GetAllocation().Height - 2.f * stepper_length;
		float slider_length = std::max( mimimum_slider_length, trough_length / pages );

		float slider_x = 0.f;
		float slider_y = stepper_length + ( trough_length - slider_length ) * current_value / value_range;

		return sf::FloatRect( slider_x, slider_y, GetWidth(), slider_length );
	}
}

bool Scrollbar::IsDecreaseStepperPressed() const {
	return m_decrease_pressed;
}

bool Scrollbar::IsIncreaseStepperPressed() const {
	return m_increase_pressed;
}

sf::Drawable* Scrollbar::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetEngine().CreateScrollbarDrawable( boost::shared_dynamic_cast<Scrollbar>( shared_from_this() ), target );
}

sf::Vector2f Scrollbar::GetRequisitionImpl() const {
	if( m_orientation == Horizontal ) {
		return sf::Vector2f( m_length, m_width );
	}

	return sf::Vector2f( m_width, m_length );
}

bool Scrollbar::HandleMouseButtonPress( Widget::Ptr /*widget*/, int x, int y, sf::Mouse::Button button ) {
	float stepper_length( Context::Get().GetEngine().GetProperty<float>( "Scrollbar.Stepper.Length", shared_from_this() ) );

	if( button != sf::Mouse::Left ) {
		return false;
	}

	sf::FloatRect slider_rect = GetSliderRect();
	slider_rect.Left += GetAllocation().Left;
	slider_rect.Top += GetAllocation().Top;

	if( slider_rect.Contains( (float)x, (float)y ) ) {
		m_dragging = true;
		return true;
	}

	if( m_orientation == Horizontal ) {
		sf::FloatRect decrease_stepper_rect( GetAllocation().Left, GetAllocation().Top, stepper_length, GetWidth() );
		sf::FloatRect increase_stepper_rect( GetAllocation().Left + GetAllocation().Width - stepper_length, GetAllocation().Top, stepper_length, GetWidth() );

		if( decrease_stepper_rect.Contains( (float)x, (float)y ) ) {
			m_decrease_pressed = true;
			GetAdjustment()->Decrement();
			m_change_timer.Reset();
			Invalidate();
			return true;
		}

		if( increase_stepper_rect.Contains( (float)x, (float)y ) ) {
			m_increase_pressed = true;
			GetAdjustment()->Increment();
			m_change_timer.Reset();
			Invalidate();
			return true;
		}
	}
	else {
		sf::FloatRect decrease_stepper_rect( GetAllocation().Left, GetAllocation().Top, GetWidth(), stepper_length );
		sf::FloatRect increase_stepper_rect( GetAllocation().Left, GetAllocation().Top + GetAllocation().Height - stepper_length, GetWidth(), stepper_length );

		if( decrease_stepper_rect.Contains( (float)x, (float)y ) ) {
			m_decrease_pressed = true;
			GetAdjustment()->Decrement();
			m_change_timer.Reset();
			Invalidate();
			return true;
		}

		if( increase_stepper_rect.Contains( (float)x, (float)y ) ) {
			m_increase_pressed = true;
			GetAdjustment()->Increment();
			m_change_timer.Reset();
			Invalidate();
			return true;
		}
	}

	float slider_center_x = slider_rect.Left + slider_rect.Width / 2.f;
	float slider_center_y = slider_rect.Top + slider_rect.Height / 2.f;

	sf::FloatRect allocation = GetAllocation();

	if( m_orientation == Horizontal ) {
		allocation.Height = GetWidth();

		if( allocation.Contains( (float)x, (float)y ) ) {
			if( (float)x < slider_center_x ) {
				m_page_decreasing = true;
				GetAdjustment()->DecrementPage();
				m_change_timer.Reset();
				Invalidate();
				return true;
			}
			else {
				m_page_increasing = true;
				GetAdjustment()->IncrementPage();
				m_change_timer.Reset();
				Invalidate();
				return true;
			}
		}
	}
	else {
		allocation.Width = GetWidth();

		if( allocation.Contains( (float)x, (float)y ) ) {
			if( (float)y < slider_center_y ) {
				m_page_decreasing = true;
				GetAdjustment()->DecrementPage();
				m_change_timer.Reset();
				Invalidate();
				return true;
			}
			else {
				m_page_increasing = true;
				GetAdjustment()->IncrementPage();
				m_change_timer.Reset();
				Invalidate();
				return true;
			}
		}
	}

	return false;
}

bool Scrollbar::HandleMouseButtonRelease( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button button ) {
	if( button != sf::Mouse::Left ) {
		return false;
	}

	m_dragging = false;
	m_decrease_pressed = false;
	m_increase_pressed = false;
	m_page_decreasing = false;
	m_page_increasing = false;

	Invalidate();

	return true;
}

void Scrollbar::HandleMouseMove( Widget::Ptr /*widget*/, int x, int y ) {
	float stepper_length( Context::Get().GetEngine().GetProperty<float>( "Scrollbar.Stepper.Length", shared_from_this() ) );

	if( !m_dragging ) {
		return;
	}

	Adjustment::Ptr adjustment( GetAdjustment() );
	sf::FloatRect slider_rect = GetSliderRect();

	float value_range = adjustment->GetUpper() - adjustment->GetLower();
	float steps = value_range / adjustment->GetMinorStep();

	if( m_orientation == Horizontal ) {
		float slider_center_x = GetAllocation().Left + slider_rect.Left + slider_rect.Width / 2.0f;
		float step_distance = ( GetAllocation().Width - 2.f * stepper_length ) / steps;

		float delta = x - slider_center_x;

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
		float slider_center_y = GetAllocation().Top + slider_rect.Top + slider_rect.Height / 2.0f;
		float step_distance = ( GetAllocation().Height - 2.f * stepper_length ) / steps;

		float delta = y - slider_center_y;

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

void Scrollbar::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& /*target*/ ) {
	// Increment / Decrement value every 200ms while one of the steppers is pressed
	if( m_decrease_pressed && m_change_timer.GetElapsedTime() > 200 ) {
		GetAdjustment()->Decrement();
		m_change_timer.Reset();
	}

	if( m_increase_pressed && m_change_timer.GetElapsedTime() > 200 ) {
		GetAdjustment()->Increment();
		m_change_timer.Reset();
	}

	// Increment / Decrement page every 200ms while mouse is pressed on the trough
	if( m_page_decreasing && m_change_timer.GetElapsedTime() > 200 ) {
		GetAdjustment()->DecrementPage();
		m_change_timer.Reset();
	}

	if( m_page_increasing && m_change_timer.GetElapsedTime() > 200 ) {
		GetAdjustment()->IncrementPage();
		m_change_timer.Reset();
	}
}

}
