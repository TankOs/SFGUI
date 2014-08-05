#include <SFGUI/Container.hpp>

#include <SFML/Window/Event.hpp>

namespace sfg {

void Container::Add( Widget::Ptr widget ) {
	if( HandleAdd( widget ) ) {
		widget->SetParent( shared_from_this() );
		RequestResize();
	}
}

void Container::Remove( Widget::Ptr widget ) {
	WidgetsList::iterator iter( std::find( m_children.begin(), m_children.end(), widget ) );

	if( iter != m_children.end() ) {
		m_children.erase( iter );
		widget->SetParent( Widget::Ptr() );
		HandleRemove( widget );

		RequestResize();
	}
}

void Container::RemoveAll() {
	while( !m_children.empty() ) {
		auto widget = m_children.back();

		m_children.pop_back();
		widget->SetParent( Widget::Ptr() );
		HandleRemove( widget );
	}

	RequestResize();
}

bool Container::IsChild( Widget::Ptr widget ) const {
	return std::find( m_children.begin(), m_children.end(), widget ) != m_children.end();
}

const Container::WidgetsList& Container::GetChildren() const {
	return m_children;
}

void Container::Refresh() {
	for( const auto& child : m_children ) {
		child->Refresh();
	}

	Widget::Refresh();
}

void Container::HandleEvent( const sf::Event& event ) {
	// Ignore event when widget is not visible.
	if( !IsGloballyVisible() ) {
		return;
	}

	// Create a copy of the event and transform mouse coordinates to local
	// coordinates if event is a mouse event.
	sf::Event local_event( event );

	if( local_event.type == sf::Event::MouseMoved ) {
		local_event.mouseMove.x -= static_cast<int>( GetAllocation().left );
		local_event.mouseMove.y -= static_cast<int>( GetAllocation().top );
	}

	if(
		local_event.type == sf::Event::MouseButtonPressed ||
		local_event.type == sf::Event::MouseButtonReleased
	) {
		local_event.mouseButton.x -= static_cast<int>( GetAllocation().left );
		local_event.mouseButton.y -= static_cast<int>( GetAllocation().top );
	}

	// Pass event to children.
	for( const auto& child : m_children ) {
		child->HandleEvent( local_event );
	}

	// Process event for own widget.
	Widget::HandleEvent( event );
}

bool Container::HandleAdd( Widget::Ptr child ) {
	if( IsChild( child ) ) {
		return false;
	}

	m_children.push_back( child );

	child->SetViewport( GetViewport() );

	return true;
}

void Container::HandleRemove( Widget::Ptr /*child*/ ) {
}

void Container::HandleChildInvalidate( Widget::PtrConst child ) const {
	auto parent = GetParent();

	if( parent ) {
		parent->HandleChildInvalidate( child );
	}
}

void Container::HandleAbsolutePositionChange() {
	// Update children's drawable positions.
	for( const auto& child : m_children ) {
		child->HandleAbsolutePositionChange();
	}

	// Update own drawable position.
	Widget::HandleAbsolutePositionChange();
}

void Container::HandleGlobalVisibilityChange() {
	Widget::HandleGlobalVisibilityChange();

	for( const auto& child : m_children ) {
		child->HandleGlobalVisibilityChange();
	}
}

void Container::HandleUpdate( float seconds ) {
	Widget::HandleUpdate( seconds );

	for( const auto& child : m_children ) {
		child->Update( seconds );
	}
}

void Container::HandleSetHierarchyLevel() {
	Widget::HandleSetHierarchyLevel();

	for( const auto& child : m_children ) {
		child->SetHierarchyLevel( GetHierarchyLevel() + 1 );
	}
}

void Container::HandleViewportUpdate() {
	for( const auto& child : m_children ) {
		child->SetViewport( GetViewport() );
	}

	Widget::HandleViewportUpdate();
}

}
