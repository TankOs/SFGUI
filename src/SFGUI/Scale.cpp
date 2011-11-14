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

Scale::Orientation Scale::GetOrientation() const {
	return m_orientation;
}

const sf::FloatRect Scale::GetSliderRect() const {
	float slider_length( Context::Get().GetEngine().GetProperty<float>( "SliderLength", shared_from_this() ) );
	float slider_width( m_orientation == Horizontal ? GetAllocation().Height : GetAllocation().Width );

	Adjustment::Ptr adjustment( GetAdjustment() );

	float current_value = adjustment->GetValue();
	float value_range = adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize();

	if( m_orientation == Horizontal ) {
		float slider_x = ( GetAllocation().Width - slider_length ) * ( current_value - adjustment->GetLower() ) / value_range;
		float slider_y = ( GetAllocation().Height - slider_width ) / 2.f;

		return sf::FloatRect( slider_x, slider_y, slider_length, slider_width );
	}
	else {
		float slider_x = ( GetAllocation().Width - slider_width ) / 2.f;
		float slider_y = ( GetAllocation().Height - slider_length ) * ( 1 - ( ( current_value - adjustment->GetLower() ) / value_range ) );

		return sf::FloatRect( slider_x, slider_y, slider_width, slider_length );
	}
}


RenderQueue* Scale::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateScaleDrawable( std::dynamic_pointer_cast<const Scale>( shared_from_this() ) );
}

sf::Vector2f Scale::GetRequisitionImpl() const {
	float slider_length( Context::Get().GetEngine().GetProperty<float>( "SliderLength", shared_from_this() ) );
	float slider_width( m_orientation == Horizontal ? GetAllocation().Height : GetAllocation().Width );

	if( m_orientation == Horizontal ) {
		return sf::Vector2f( slider_length * 2.f, slider_width );
	}
	else {
		return sf::Vector2f( slider_width, slider_length * 2.f );
	}
}

void Scale::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) {
	if( button != sf::Mouse::Left ) {
		return;
	}

	if( press ) {
		sf::FloatRect slider_rect = GetSliderRect();
		slider_rect.Left += GetAllocation().Left;
		slider_rect.Top += GetAllocation().Top;

		if( !slider_rect.Contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			m_dragging = false;
			return;
		}

		m_dragging = true;
	}
	else {
		m_dragging = false;
	}
}

void Scale::HandleMouseMoveEvent( int x, int y ) {
	if( !m_dragging ) {
		return;
	}

	Adjustment::Ptr adjustment( GetAdjustment() );
	sf::FloatRect slider_rect = GetSliderRect();

	float value_range = std::max( adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize(), adjustment->GetMinorStep() / 2.f );
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

const std::string& Scale::GetName() const {
	static const std::string name( "Scale" );
	return name;
}

}
