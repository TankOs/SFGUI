#include <SFGUI/Adjustment.hpp>

namespace sfg {

Adjustment::Adjustment( float lower, float upper, float value, float step, float page, float size ) :
	m_lower( lower ),
	m_upper( upper ),
	m_value( value ),
	m_step( step ),
	m_page( page ),
	m_size( size )
{

}

Adjustment::~Adjustment() {
}

}
