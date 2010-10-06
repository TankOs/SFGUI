#include <SFGUI/Container.hpp>

namespace sfg {

Container::Container() :
	Widget()
{
}

void Container::Add( Widget::Ptr widget ) {
	if( IsChild( widget ) ) {
		return;
	}
}

void Container::Remove( Widget::Ptr widget ) {
	if( !IsChild( widget ) ) {
		return;
	}

	m_children.erase( widget );
}

bool Container::IsChild( Widget::Ptr widget ) const {
	WidgetsSet::const_iterator  iter( m_children.find( widget ) );
	return iter != m_children.end();
}

const Container::WidgetsSet& Container::GetChildren() const {
	return m_children;
}

Container::WidgetsSet& Container::GetChildren() {
	return m_children;
}

}
