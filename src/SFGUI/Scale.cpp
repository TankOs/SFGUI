#include <SFGUI/Scale.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

Scale::Scale( Orientation orientation ) :
	Range( orientation ),
	m_drag_offset( 0 ),
	m_dragging( false )
{
}

Scale::~Scale() {
	delete m_drag_offset;
}

Scale::Ptr Scale::Create( Orientation orientation ) {
	Scale::Ptr ptr( new Scale( orientation ) );
	return ptr;
}

Scale::Ptr Scale::Create( float min, float max, float step, Orientation orientation ) {
	Scale::Ptr ptr( new Scale( orientation ) );
	ptr->SetRange( min, max );
	ptr->SetIncrements( step, 0.f );
	return ptr;
}

const sf::FloatRect Scale::GetSliderRect() const {
	float slider_length( Context::Get().GetEngine().GetProperty<float>( "SliderLength", shared_from_this() ) );

	float slider_width( ( GetOrientation() == HORIZONTAL ) ? GetAllocation().height : GetAllocation().width );

	Adjustment::Ptr adjustment( GetAdjustment() );

	float current_value = adjustment->GetValue();
	float value_range = adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize();

	if( GetOrientation() == HORIZONTAL ) {
		float slider_x = ( GetAllocation().width - slider_length ) * ( current_value - adjustment->GetLower() ) / value_range;
		float slider_y = ( GetAllocation().height - slider_width ) / 2.f;

		return sf::FloatRect( slider_x, slider_y, slider_length, slider_width );
	}
	else {
		float slider_x = ( GetAllocation().width - slider_width ) / 2.f;
		float slider_y = ( GetAllocation().height - slider_length ) * ( 1 - ( ( current_value - adjustment->GetLower() ) / value_range ) );

		return sf::FloatRect( slider_x, slider_y, slider_width, slider_length );
	}
}


RenderQueue* Scale::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateScaleDrawable( DynamicPointerCast<const Scale>( shared_from_this() ) );
}

sf::Vector2f Scale::CalculateRequisition() {
	float slider_length( Context::Get().GetEngine().GetProperty<float>( "SliderLength", shared_from_this() ) );
	float slider_width( std::max( 3.f, ( GetOrientation() == HORIZONTAL ) ? GetAllocation().height : GetAllocation().width ) );

	if( GetOrientation() == HORIZONTAL ) {
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

	if( m_drag_offset ) {
		delete m_drag_offset;
		m_drag_offset = 0;
		m_dragging = false;
	}

	if( !GetAllocation().contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
		return;
	}

	if( press ) {
		if( !GetSliderRect().contains( static_cast<float>( x ) - GetAllocation().left, static_cast<float>( y ) - GetAllocation().top ) ) {
			Adjustment::Ptr adjustment( GetAdjustment() );

			float minor_step = adjustment->GetMinorStep();
			float range = adjustment->GetUpper() - adjustment->GetLower();
			float steps = range / minor_step;
			float needed_steps = 0.f;

			float trough_position = 0.f;
			float trough_length = 0.f;

			if( GetOrientation() == HORIZONTAL ) {
				trough_position = static_cast<float>( x ) - ( GetAllocation().left + GetSliderRect().width / 2.f );
				trough_length = GetAllocation().width - GetSliderRect().width;
			}

			if( GetOrientation() == VERTICAL ) {
				trough_position = static_cast<float>( y ) - ( GetAllocation().top + GetSliderRect().height / 2.f );
				trough_length = GetAllocation().height - GetSliderRect().height;
			}

			trough_position = std::min( trough_position, trough_length );

			float trough_ratio = trough_position / trough_length;

			for( ; needed_steps < steps; needed_steps += 1.f ) {
				if( ( 1.f / steps ) * needed_steps > trough_ratio ) {
					break;
				}
			}

			needed_steps = std::max( needed_steps - 1.f, 0.f );

			adjustment->SetValue( needed_steps * minor_step );
		}

		m_dragging = true;
		m_drag_offset = new sf::Vector2f(
			static_cast<float>( x ) - ( GetAllocation().left + GetSliderRect().left + GetSliderRect().width / 2.f ),
			static_cast<float>( y ) - ( GetAllocation().top + GetSliderRect().top + GetSliderRect().height / 2.f )
		);
	}
}

void Scale::HandleMouseMoveEvent( int x, int y ) {
	if( !m_dragging ) {
		return;
	}

	assert( m_drag_offset );

	Adjustment::Ptr adjustment( GetAdjustment() );
	sf::FloatRect slider_rect = GetSliderRect();

	float value_range = std::max( adjustment->GetUpper() - adjustment->GetLower() - adjustment->GetPageSize(), adjustment->GetMinorStep() / 2.f );
	float steps = value_range / adjustment->GetMinorStep();

	if( GetOrientation() == HORIZONTAL ) {
		float slider_center_x = GetAllocation().left + slider_rect.left + slider_rect.width / 2.0f;
		float step_distance = ( GetAllocation().width - slider_rect.width ) / steps;
		float delta = static_cast<float>( x ) - slider_center_x - m_drag_offset->x;

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
		float slider_center_y = GetAllocation().top + slider_rect.top + slider_rect.height / 2.0f;
		float step_distance = ( GetAllocation().height - slider_rect.height ) / steps;
		float delta = static_cast<float>( y ) - slider_center_y - m_drag_offset->y;

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
