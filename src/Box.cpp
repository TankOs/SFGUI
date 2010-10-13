#include <SFGUI/Box.hpp>
#include <iostream>

namespace sfg {

Box::Box( Orientation orientation ) :
	Container(),
	m_orientation( orientation )
{
	OnAdd.Connect( &Box::HandleAdd, this );
	OnRemove.Connect( &Box::HandleRemove, this );
}

Box::Ptr Box::Create( Orientation orientation ) {
	Box::Ptr  ptr( new Box( orientation ) );
	return ptr;
}

void Box::Pack( Widget::Ptr widget, bool expand, bool fill ) {
	if( IsChild( widget ) ) {
		return;
	}

	// It's important to create the ChildInfo object first, so that the
	// HandleAdd() method recognized the widget as a correctly packed one.
	m_children.insert( ChildInfo( widget, expand, fill ) );
	Add( widget );
}

void Box::HandleAdd( Widget::Ptr /*widget*/, Widget::Ptr child ) {
	// If there's no ChildInfo present for the widget, the user added the widget
	// manually, which is not allowed for this class.
	ChildrenCont::const_iterator  iter( m_children.find( child ) );

	// TODO: Add ChildInfo with default settings.
	if( iter == m_children.end() ) {
		std::cerr << "SFGUI warning: Child must be added via Pack() for sfg::Box widgets." << std::endl;
		Remove( child );
		return;
	}
}

void Box::HandleRemove( Widget::Ptr /*widget*/, Widget::Ptr child ) {
	m_children.erase( child );
}

Box::ChildInfo::ChildInfo( Widget::Ptr widget_, bool expand_, bool fill_ ) :
	widget( widget_ ),
	expand( expand_ ),
	fill( fill_ )
{
}

bool Box::ChildInfo::operator<( const ChildInfo& rhs ) const {
	return widget < rhs.widget;
}

}
