#include <SFGUI/Range.hpp>
#include <SFGUI/Adjustment.hpp>

namespace sfg {

Range::Range( Orientation orientation ) :
	m_change_connection( 0 ),
	m_orientation( orientation )
{
}

Adjustment::Ptr Range::GetAdjustment() const {
	return m_adjustment;
}

void Range::SetAdjustment( Adjustment::Ptr adjustment ) {
	// Disconnect the previous connection.
	if( m_adjustment ) {
		m_adjustment->GetSignal( Adjustment::OnChange ).Disconnect( m_change_connection );
	}

	m_adjustment = adjustment;

	auto weak_this = std::weak_ptr<Widget>( shared_from_this() );

	// Connect change signal.
	m_change_connection = m_adjustment->GetSignal( Adjustment::OnChange ).Connect( [weak_this] {
		auto shared_this = weak_this.lock();

		if( !shared_this ) {
			return;
		}

		auto range = std::dynamic_pointer_cast<Range>( shared_this );

		if( !range ) {
			return;
		}

		range->HandleAdjustmentChange();
	} );
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

void Range::HandleAdjustmentChange() {
	Invalidate();
}

Range::Orientation Range::GetOrientation() const {
	return static_cast<Orientation>( m_orientation );
}

}
