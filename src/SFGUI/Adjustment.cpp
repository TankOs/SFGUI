#include <SFGUI/Adjustment.hpp>

namespace sfg {

// Signals.
Signal::SignalID Adjustment::OnChange = 0;

Adjustment::Adjustment( float value, float lower, float upper, float minor_step, float major_step, float page_size ) :
	m_value( value ),
	m_lower( lower ),
	m_upper( upper ),
	m_minor_step( minor_step ),
	m_major_step( major_step ),
	m_page_size( page_size )
{

}

Adjustment::~Adjustment() {

}

Adjustment::Ptr Adjustment::Create( float value, float lower, float upper, float minor_step, float major_step, float page_size ) {
	Adjustment::Ptr ptr( new Adjustment( value, lower, upper, minor_step, major_step, page_size ) );
	return ptr;
}

Adjustment& Adjustment::operator=( const Adjustment& adjustment ) {
	SetLower( adjustment.m_lower );
	SetUpper( adjustment.m_upper );
	SetMinorStep( adjustment.m_minor_step );
	SetMajorStep( adjustment.m_major_step );
	SetPageSize( adjustment.m_page_size );
	SetValue( adjustment.m_value );

	return *this;
}

float Adjustment::GetValue() const {
 return m_value;
}

void Adjustment::SetValue( float new_value ) {
	float old_value = m_value;
	m_value = new_value;

	// Clamp value within limits
	if( m_value < m_lower ) {
		m_value = m_lower;
	}
	else if( m_value + m_page_size > m_upper ) {
		m_value = m_upper - m_page_size;
	}

	if( m_value != old_value ) {
		GetSignals().Emit( OnChange );
	}
}

float Adjustment::GetLower() const {
	return m_lower;
}

void Adjustment::SetLower( float new_lower ) {
	m_lower = new_lower;

	if( m_lower > m_upper ) {
		m_upper = m_lower;
	}

	SetValue( GetValue() );
}

float Adjustment::GetUpper() const {
	return m_upper;
}

void Adjustment::SetUpper( float new_upper ) {
	m_upper = new_upper;

	if( m_upper < m_lower ) {
		m_lower = m_upper;
	}

	SetValue( GetValue() );
}

float Adjustment::GetMinorStep() const {
	return m_minor_step;
}

void Adjustment::SetMinorStep( float new_minor_step ) {
	m_minor_step = new_minor_step;
}

float Adjustment::GetMajorStep() const {
	return m_major_step;
}

void Adjustment::SetMajorStep( float new_major_step ) {
	m_major_step = new_major_step;
}

float Adjustment::GetPageSize() const {
	return m_page_size;
}

void Adjustment::SetPageSize( float new_page_size ) {
	m_page_size = new_page_size;

	if( m_upper < m_lower + m_page_size ) {
		m_upper = m_lower + m_page_size;
	}

	SetValue( GetValue() );
}

void Adjustment::Configure( float new_value, float new_lower, float new_upper, float new_minor_step, float new_major_step, float new_page_size ) {
	SetValue( new_value );
	SetLower( new_lower );
	SetUpper( new_upper );
	SetMinorStep( new_minor_step );
	SetMajorStep( new_major_step );
	SetPageSize( new_page_size );
}

void Adjustment::Increment() {
	SetValue( GetValue() + GetMinorStep() );
}

void Adjustment::Decrement() {
	SetValue( GetValue() - GetMinorStep() );
}

void Adjustment::IncrementPage() {
	SetValue( GetValue() + GetMajorStep() );
}

void Adjustment::DecrementPage() {
	SetValue( GetValue() - GetMajorStep() );
}

}
