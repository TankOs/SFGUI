#include <SFGUI/Range.hpp>

namespace sfg {

Range::Range() :
	Widget()
{
	m_adjustment = Adjustment::Create();

	m_adjustment->OnChange.Connect( &Range::HandleAdjustmentChange, this );
}

Adjustment::Ptr Range::GetAdjustment() const {
	return m_adjustment;
}

void Range::SetAdjustment( Adjustment::Ptr adjustment ) {
	m_adjustment = adjustment;
}

float Range::GetValue() const {
	return m_adjustment->GetValue();
}

void Range::SetValue( float new_value ) {
	m_adjustment->SetValue( new_value );
}

void Range::SetIncrements( float minor_step, float major_step ) {
	m_adjustment->SetMinorStep( minor_step );
	m_adjustment->SetMajorStep( major_step );
}

void Range::SetRange( float min, float max ) {
	m_adjustment->SetLower( min );
	m_adjustment->SetUpper( max );
}

void Range::HandleAdjustmentChange( Adjustment::Ptr /*adjustment*/ ) {
	Invalidate();
}

}
