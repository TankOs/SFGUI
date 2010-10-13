#include <SFGUI/Widget.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/RenderEngine.hpp>

namespace sfg {
Widget::Widget() :
	m_sensitive( true ),
	m_visible( true ),
	m_state( Normal ),
	m_allocation( 0, 0, 0, 0 ),
	m_requisition( 0, 0, 0, 0 ),
	m_renderengine( 0 )
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

void Widget::SetName( const std::string& name ) {
	m_name = name;
}

const std::string& Widget::GetName() const {
	return m_name;
}

void Widget::GrabFocus() {
	GrabFocus( shared_from_this() );
}

void Widget::GrabFocus( Ptr widget ) {
	if( !m_parent ) {
		// Notify old focused widget.
		if( m_focus_widget ) {
			m_focus_widget->OnFocusChange.Sig( m_focus_widget );
		}

		m_focus_widget = widget;
		m_focus_widget->OnFocusChange.Sig( m_focus_widget );
	}
	else {
		m_parent->GrabFocus( widget );
	}
}

void Widget::AllocateSize( const sf::FloatRect& rect ) {
	sf::FloatRect  oldallocation( m_allocation );

	m_allocation = rect;
	OnSizeAllocate.Sig( shared_from_this(), oldallocation );

	Invalidate();
}

void Widget::RequestSize( const sf::FloatRect& rect ) {
	sf::FloatRect  oldrequisition( m_requisition );

	m_requisition = rect;
	OnSizeRequest.Sig( shared_from_this(), oldrequisition );
}


const sf::FloatRect& Widget::GetAllocation() const {
	return m_allocation;
}

const sf::FloatRect& Widget::GetRequisition() const {
	return m_requisition;
}

void Widget::Expose( sf::RenderTarget& target ) {
	if( m_drawable ) {
		target.Draw( *m_drawable );
	}

	OnExpose.Sig( shared_from_this(), target );
}

const RenderEngine* Widget::GetRenderEngine() const {
	return m_renderengine;
}

void Widget::SetRenderEngine( const RenderEngine& engine ) {
	m_renderengine = &engine;
	Invalidate();
}

void Widget::Invalidate() {
	if( !m_renderengine ) {
		return;
	}

	m_drawable.reset( InvalidateImpl() );

	if( m_drawable ) {
		m_drawable->SetPosition( GetAllocation().Left, GetAllocation().Top );
	}
}

sf::Drawable* Widget::InvalidateImpl() {
	return 0;
}

void Widget::SetParent( Widget::Ptr parent ) {
	Container::Ptr  cont( boost::shared_dynamic_cast<Container>( parent ) );

	if( !cont ) {
		return;
	}

	if( m_parent ) {
		Container::Ptr  oldparent( boost::shared_dynamic_cast<Container>( m_parent ) );
		oldparent->Remove( shared_from_this() );
	}

	m_parent = cont;
}

}
