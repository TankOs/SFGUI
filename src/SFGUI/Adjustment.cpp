#include <SFGUI/Adjustment.hpp>

namespace sfg {

Adjustment::Adjustment( float value, float lower, float upper, float step, float page, float size ) :
	m_value( value ),
	m_lower( lower ),
	m_upper( upper ),
	m_step( step ),
	m_page( page ),
	m_size( size )
{

}

Adjustment::~Adjustment() {
}

}
