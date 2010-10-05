#include <SFGUI/Widget.hpp>

namespace sfg {
Widget::Widget() :
	m_sensitive( true ),
	m_visible( true ),
	m_focus( false ),
	m_state( Normal )
{
}

Widget::~Widget() {
}

bool Widget::IsSensitive() const {
	return m_sensitive;
}

bool Widget::IsVisible() const {
	return m_visible;
}

bool Widget::HasFocus() const {
	return m_focus;
}

void Widget::SetName( const std::string& name ) {
	m_name = name;
}

const std::string& Widget::GetName() const {
	return m_name;
}

}
