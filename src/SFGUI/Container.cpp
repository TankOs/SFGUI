#include <SFGUI/Container.hpp>

namespace sfg {

Container::Container() :
	Widget()
{
}

Container::~Container() {
}

void Container::Add( const Widget::Ptr& widget ) {
	if( IsChild( widget ) ) {
		return;
	}

	m_children.push_back( widget );
	HandleAdd( widget );

	// Check if HandleAdd still wants the little boy.
	if( IsChild( widget ) ) {
		widget->SetParent( shared_from_this() );
		RequestResize();
	}
}

void Container::Remove( const Widget::Ptr& widget ) {
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
		Widget::Ptr widget = m_children.back();

		m_children.pop_back();
		widget->SetParent( Widget::Ptr() );
		HandleRemove( widget );
	}

	RequestResize();
}

bool Container::IsChild( const Widget::Ptr& widget ) const {
	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		if( m_children[index] == widget ) {
			return true;
		}
	}

	return false;
}

const Container::WidgetsList& Container::GetChildren() const {
	return m_children;
}

void Container::Refresh() {
	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		m_children[index]->Refresh();
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

	std::size_t children_size = m_children.size();

	// Pass event to children.
	for( std::size_t index = 0; index < children_size; ++index ) {
		m_children[index]->HandleEvent( local_event );
	}

	// Process event for own widget.
	Widget::HandleEvent( event );
}

void Container::HandleAdd( const Widget::Ptr& child ) {
	child->SetViewport( GetViewport() );
}

void Container::HandleRemove( const Widget::Ptr& /*child*/ ) {
}

void Container::HandleChildInvalidate( const Widget::PtrConst& child ) const {
	Container::PtrConst parent = GetParent();

	if( parent ) {
		parent->HandleChildInvalidate( child );
	}
}

void Container::HandleAbsolutePositionChange() {
	std::size_t children_size = m_children.size();

	// Update children's drawable positions.
	for( std::size_t index = 0; index < children_size; ++index ) {
		m_children[index]->HandleAbsolutePositionChange();
	}

	// Update own drawable position.
	Widget::HandleAbsolutePositionChange();
}

void Container::HandleGlobalVisibilityChange() {
	Widget::HandleGlobalVisibilityChange();

	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		m_children[index]->HandleGlobalVisibilityChange();
	}
}

void Container::HandleUpdate( float seconds ) {
	Widget::HandleUpdate( seconds );

	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		m_children[index]->Update( seconds );
	}
}

void Container::HandleSetHierarchyLevel() {
	Widget::HandleSetHierarchyLevel();

	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		m_children[index]->SetHierarchyLevel( GetHierarchyLevel() + 1 );
	}
}

void Container::HandleViewportUpdate() {
	std::size_t children_size = m_children.size();

	for( std::size_t index = 0; index < children_size; ++index ) {
		m_children[index]->SetViewport( GetViewport() );
	}

	Widget::HandleViewportUpdate();
}

}
