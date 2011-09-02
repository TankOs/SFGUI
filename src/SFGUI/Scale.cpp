#include <SFGUI/Scale.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

Scale::Scale( Orientation orientation ) :
	Range(),
	m_orientation( orientation ),
	m_dragging( false )
{
  OnMouseButtonPress.Connect( &Scale::HandleMouseButtonPress, this );
	OnMouseButtonRelease.Connect( &Scale::HandleMouseButtonRelease, this );
	OnMouseMove.Connect( &Scale::HandleMouseMove, this );
}

Scale::Ptr Scale::Create( Orientation orientation ) {
	Scale::Ptr  ptr( new Scale( orientation ) );
	return ptr;
}

Scale::Ptr Scale::Create( float min, float max, float step, Orientation orientation ) {
	Scale::Ptr  ptr( new Scale( orientation ) );
	ptr->SetRange( min, max );
	ptr->SetIncrements( step, 0.f );
	return ptr;
}

const Scale::Orientation Scale::GetOrientation() const {
	return m_orientation;
}

const sf::FloatRect Scale::GetSliderRect() const {
	float slider_length( Context::Get().GetEngine().GetProperty<float>( "Scale.Slider.Length", shared_from_this() ) );
	float slider_width( Context::Get().GetEngine().GetProperty<float>( "Scale.Slider.Width", shared_from_this() ) );

	Adjustment::Ptr adjustment( GetAdjustment() );

	float current_value = adjustment->GetValue();
	float value_range = adjustment->GetUpper() - adjustment->GetLower();

	if( m_orientation == Horizontal ) {
		float slider_x = ( GetAllocation().Width - slider_length ) * current_value / value_range;
		float slider_y = ( GetAllocation().Height - slider_width ) / 2.f;

		return sf::FloatRect( slider_x, slider_y, slider_length, slider_width );
	}
	else {
		float slider_x = ( GetAllocation().Width - slider_width ) / 2.f;
		float slider_y = ( GetAllocation().Height - slider_length ) * ( 1 - ( current_value / value_range ) );

		return sf::FloatRect( slider_x, slider_y, slider_width, slider_length );
	}
}

sf::Drawable* Scale::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetEngine().CreateScaleDrawable( boost::shared_dynamic_cast<Scale>( shared_from_this() ), target );
}

sf::Vector2f Scale::GetRequisitionImpl() const {
	float slider_width( Context::Get().GetEngine().GetProperty<float>( "Scale.Slider.Width", shared_from_this() ) );

	if( m_orientation == Horizontal ) {
		return sf::Vector2f( 0.f, slider_width );
	}
	else {
		return sf::Vector2f( slider_width, 0.f );
	}
}

bool Scale::HandleMouseButtonPress( Widget::Ptr /*widget*/, int x, int y, sf::Mouse::Button button ) {
	if( button != sf::Mouse::Left ) {
		return false;
	}

	sf::FloatRect slider_rect = GetSliderRect();
	slider_rect.Left += GetAllocation().Left;
	slider_rect.Top += GetAllocation().Top;

	if( !slider_rect.Contains( (float)x, (float)y ) ) {
		m_dragging = false;
		return false;
	}

	m_dragging = true;
	return true;
}

bool Scale::HandleMouseButtonRelease( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button button ) {
	if( button != sf::Mouse::Left ) {
		return false;
	}

	m_dragging = false;

	return true;
}

void Scale::HandleMouseMove( Widget::Ptr /*widget*/, int x, int y ) {
	if( !m_dragging ) {
		return;
	}

	Adjustment::Ptr adjustment( GetAdjustment() );
	sf::FloatRect slider_rect = GetSliderRect();

	float value_range = adjustment->GetUpper() - adjustment->GetLower();
	float steps = value_range / adjustment->GetMinorStep();

	if( m_orientation == Horizontal ) {
		float slider_center_x = GetAllocation().Left + slider_rect.Left + slider_rect.Width / 2.0f;
		float step_distance = ( GetAllocation().Width - slider_rect.Width ) / steps;

		float delta = static_cast<float>( x ) - slider_center_x;

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
		float step_distance = ( GetAllocation().Height - slider_rect.Height ) / steps;

		float delta = static_cast<float>( y ) - slider_center_y;

		while( delta < ( -step_distance / 2 ) ) {
			adjustment->Increment();
			delta += step_distance;
		}

		while( delta > ( step_distance / 2 ) ) {
			adjustment->Decrement();
			delta -= step_distance;
		}
	}
}

}
