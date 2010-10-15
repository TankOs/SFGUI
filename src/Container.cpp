#include <SFGUI/Container.hpp>

namespace sfg {

Container::Container() :
	Widget()
{
	OnExpose.Connect( &Container::HandleExpose, this );
	OnSizeAllocate.Connect( &Container::HandleSizeAllocate, this );
}

void Container::Add( Widget::Ptr widget ) {
	if( IsChild( widget ) ) {
		return;
	}

	m_children.insert( widget );
	widget->SetParent( shared_from_this() );
	widget->SetPosition( sf::Vector2f( GetAllocation().Left, GetAllocation().Top ) );

	OnAdd.Sig( shared_from_this(), widget );
}

void Container::Remove( Widget::Ptr widget ) {
	if( !IsChild( widget ) ) {
		return;
	}

	m_children.erase( widget );
	OnRemove.Sig( shared_from_this(), widget );
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

void Container::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& target ) {
	WidgetsSet::iterator  iter( m_children.begin() );
	WidgetsSet::iterator  iterend( m_children.end() );

	for( ; iter != iterend; ++iter ) {
		(*iter)->Expose( target );
	}
}

void Container::HandleSizeAllocate( Widget::Ptr /*widget*/, const sf::FloatRect& oldallocation ) {
	if( GetChildren().size() > 0 && (GetAllocation().Left != oldallocation.Left || GetAllocation().Top != oldallocation.Top) ) {
		WidgetsSet::iterator  iter( m_children.begin() );
		WidgetsSet::iterator  iterend( m_children.end() );
		sf::Vector2f  delta( GetAllocation().Left - oldallocation.Left, GetAllocation().Top - oldallocation.Top );

		// Move children accordingly.
		for( ; iter != iterend; ++iter ) {
			(*iter)->SetPosition(
				sf::Vector2f(
					(*iter)->GetAllocation().Left + delta.x,
					(*iter)->GetAllocation().Top + delta.x
				)
			);
		}
	}
}

}
