#include <SFGUI/Widget.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/Context.hpp>
#include <cmath>

namespace sfg {

Widget::Widget() :
	Object(),
	m_allocation( 0.f, 0.f, 0.f, 0.f ),
	m_requisition( 0.f, 0.f ),
	m_custom_requisition( 0 ),
	m_hierarchy_level( 0 ),
	m_drawable( 0 ),
	m_sensitive( true ),
	m_visible( true ),
	m_drawn( true ),
	m_state( NORMAL ),
	m_mouse_in( false ),
	m_mouse_button_down( -1 ),
	m_invalidated( true )
{
	m_viewport = Renderer::Get().GetDefaultViewport();
}

Widget::~Widget() {
	delete m_drawable;
	delete m_custom_requisition;
}

bool Widget::IsSensitive() const {
	return m_sensitive;
}

bool Widget::IsLocallyVisible() const {
	return m_visible;
}

bool Widget::IsGloballyVisible() const {
	// If not locally visible, also cannot be globally visible.
	if( !IsLocallyVisible() ) {
		return false;
	}

	// At this point we know the widget is locally visible.

	PtrConst parent( m_parent.lock() );

	// If locally visible and no parent, globally visible.
	if( !parent ) {
		return true;
	}

	// Return parent's global visibility.
	return parent->IsGloballyVisible();
}

void Widget::GrabFocus( Ptr widget ) {
	Container::Ptr parent( m_parent.lock() );

	if( !parent ) {
		// Notify old focused widget.
		if( m_focus_widget ) {
			m_focus_widget->OnLostFocus();
			m_focus_widget->HandleFocusChange( widget );
		}

		m_focus_widget = widget;

		if( m_focus_widget ) {
			m_focus_widget->OnGainFocus();
			m_focus_widget->HandleFocusChange( widget );
		}
	}
	else {
		parent->GrabFocus( widget );
	}
}

bool Widget::HasFocus( PtrConst widget ) const {
	Container::Ptr parent( m_parent.lock() );

	if( !parent ) {
		if( m_focus_widget == widget ) {
			return true;
		}

		return false;
	}

	return parent->HasFocus( widget );
}

void Widget::SetAllocation( const sf::FloatRect& rect ) {
	sf::FloatRect oldallocation( m_allocation );

	// Make sure allocation is pixel-aligned.
	m_allocation.left = std::floor( rect.left + .5f );
	m_allocation.top = std::floor( rect.top + .5f );
	m_allocation.width = std::floor( rect.width + .5f );
	m_allocation.height = std::floor( rect.height + .5f );

	if(
		oldallocation.top == m_allocation.top &&
		oldallocation.left == m_allocation.left &&
		oldallocation.width == m_allocation.width &&
		oldallocation.height == m_allocation.height
	) {
		// Nothing even changed. Save the hierarchy the trouble.
		return;
	}

	if( ( oldallocation.top != m_allocation.top ) || ( oldallocation.left != m_allocation.left ) ) {
	  HandlePositionChange();
	  HandleAbsolutePositionChange();
	}

	if( ( oldallocation.width != m_allocation.width ) || ( oldallocation.height != m_allocation.height ) ) {
	  HandleSizeChange();

	  Invalidate();

	  OnSizeAllocate();
	}
}

void Widget::RequestResize() {
	m_requisition = CalculateRequisition();

	if( m_custom_requisition ) {
		if( m_custom_requisition->x > 0.f ) {
			m_requisition.x = std::max( m_custom_requisition->x, m_requisition.x );
		}

		if( m_custom_requisition->y > 0.f ) {
			m_requisition.y = std::max( m_custom_requisition->y, m_requisition.y );
		}
	}

	HandleRequisitionChange();

	Container::Ptr parent = m_parent.lock();

	// Notify observers.
	OnSizeRequest();

	if( parent ) {
		parent->RequestResize();
	}
	else {
		sf::FloatRect allocation(
			GetAllocation().left,
			GetAllocation().top,
			std::max( GetAllocation().width, m_requisition.x ),
			std::max( GetAllocation().height, m_requisition.y )
		);

		SetAllocation( allocation );
	}
}


const sf::FloatRect& Widget::GetAllocation() const {
	return m_allocation;
}

void Widget::Update( float seconds ) {
	if( m_invalidated ) {
		m_invalidated = false;

		delete m_drawable;
		m_drawable = InvalidateImpl();

		if( m_drawable ) {
			m_drawable->SetPosition( GetAbsolutePosition() );
			m_drawable->SetLevel( m_hierarchy_level );
			m_drawable->Show( IsGloballyVisible() );
			m_drawable->SetViewport( m_viewport );
		}
	}

	HandleUpdate( seconds );
}

void Widget::Invalidate() const {
	if( m_invalidated ) {
		return;
	}

	m_invalidated = true;

	Container::PtrConst parent = m_parent.lock();

	if( parent ) {
		parent->HandleChildInvalidate( static_cast<Widget::PtrConst>( shared_from_this() ) );
	}
}

RenderQueue* Widget::InvalidateImpl() const {
	return 0;
}

void Widget::SetParent( const Widget::Ptr& parent ) {
	Container::Ptr cont( DynamicPointerCast<Container>( parent ) );

	if( !cont ) {
		return;
	}

	Container::Ptr oldparent = m_parent.lock();

	if( oldparent ) {
		oldparent->Remove( shared_from_this() );
	}

	m_parent = cont;

	SetHierarchyLevel( parent->GetHierarchyLevel() + 1 );
}

void Widget::SetPosition( const sf::Vector2f& position ) {
	sf::FloatRect allocation( GetAllocation() );

	// Make sure allocation is pixel-aligned.
	m_allocation.left = std::floor( position.x + .5f );
	m_allocation.top = std::floor( position.y + .5f );

	if( ( allocation.top != m_allocation.top ) || ( allocation.left != m_allocation.left ) ) {
	  HandlePositionChange();
	  HandleAbsolutePositionChange();
	}
}

void Widget::HandleEvent( const sf::Event& event ) {
	if( !IsGloballyVisible() ) {
		return;
	}

	// Ignore the event if another widget is active.
	if( !IsActiveWidget() && !IsActiveWidget( PtrConst() ) ) {
		return;
	}

	// Set widget active in context.
	Context::Get().SetActiveWidget( shared_from_this() );

	Container::Ptr parent( m_parent.lock() );

	switch( event.type ) {
		case sf::Event::MouseMoved:
			// Check if pointer inside of widget's allocation.
			if( GetAllocation().contains( static_cast<float>( event.mouseMove.x ), static_cast<float>( event.mouseMove.y ) ) ) {
				// Check for enter event.
				if( m_mouse_in == false ) {
					m_mouse_in = true;
					OnMouseEnter();
					HandleMouseEnter( event.mouseMove.x, event.mouseMove.y );
				}

				OnMouseMove();
			}
			else if( m_mouse_in == true ) { // Check for leave event.
				m_mouse_in = false;
				OnMouseLeave();
				HandleMouseLeave( event.mouseMove.x, event.mouseMove.y );
			}

			HandleMouseMoveEvent( event.mouseMove.x, event.mouseMove.y );
			break;

		case sf::Event::MouseButtonPressed:
			if( ( m_mouse_button_down == -1 ) && m_mouse_in ) {
				m_mouse_button_down = event.mouseButton.button;
			}

			HandleMouseButtonEvent( event.mouseButton.button, true, event.mouseButton.x, event.mouseButton.y );

			if( event.mouseButton.button == sf::Mouse::Left ) {
				OnMouseLeftPress();
			}
			else if( event.mouseButton.button == sf::Mouse::Right ) {
				OnMouseRightPress();
			}

			break;

		case sf::Event::MouseButtonReleased:
			// Only process as a click when mouse button has been pressed inside the widget before.
			if( m_mouse_button_down == event.mouseButton.button ) {
				m_mouse_button_down = -1;

				// When released inside the widget, the event can be considered a click.
				if( m_mouse_in ) {
					HandleMouseClick( event.mouseButton.button, event.mouseButton.x, event.mouseButton.y );

					if( event.mouseButton.button == sf::Mouse::Left ) {
						OnLeftClick();
					}
					else if( event.mouseButton.button == sf::Mouse::Right ) {
						OnRightClick();
					}
				}
			}

			HandleMouseButtonEvent( event.mouseButton.button, false, event.mouseButton.x, event.mouseButton.y );

			if( event.mouseButton.button == sf::Mouse::Left ) {
				OnMouseLeftRelease();
			}
			else if( event.mouseButton.button == sf::Mouse::Right ) {
				OnMouseRightRelease();
			}

			break;

		case sf::Event::KeyPressed:
			if( HasFocus() ) {
				// TODO: Delegate event too when widget's not active?
				HandleKeyEvent( event.key.code, true );
				OnKeyPress();
			}

			break;

		case sf::Event::KeyReleased:
			if( HasFocus() ) {
				// TODO: Delegate event too when widget's not active?
				HandleKeyEvent( event.key.code, false );
				OnKeyRelease();
			}
			break;

		case sf::Event::TextEntered:
			if( HasFocus() ) {
				// TODO: Delegate event too when widget's not active?
				HandleTextEvent( event.text.unicode );
				OnText();
			}
			break;

		default:
			break;
	}
}

void Widget::SetState( State state ) {
	// Do nothing if state wouldn't change.
	if( m_state == state ) {
		return;
	}

	unsigned char old_state( m_state );
	m_state = state;

	// If HandleStateChange() changed the state, do not call observer, will be
	// done from there too.
	if( m_state != old_state ) {
		HandleStateChange( static_cast<State>( old_state ) );
		OnStateChange();
	}

	if( state == ACTIVE ) {
		GrabFocus( shared_from_this() );
		SetActiveWidget( shared_from_this() );
	}
	else if( old_state == ACTIVE ) {
		SetActiveWidget( Ptr() );
	}
}

Widget::State Widget::GetState() const {
	return static_cast<State>( m_state );
}

Container::Ptr Widget::GetParent() {
	return m_parent.lock();
}

Container::PtrConst Widget::GetParent() const {
	return m_parent.lock();
}

void Widget::GrabFocus() {
	GrabFocus( shared_from_this() );
}

bool Widget::HasFocus() const {
	return HasFocus( shared_from_this() );
}

bool Widget::IsMouseInWidget() const {
	return m_mouse_in;
}

void Widget::Show( bool show ) {
	if( show == m_visible ) {
		return;
	}

	bool old_global_visibility = IsGloballyVisible();

	m_visible = show;

	HandleLocalVisibilityChange();

	if( old_global_visibility != IsGloballyVisible() ) {
		HandleGlobalVisibilityChange();
	}

	RequestResize();
}

const sf::Vector2f& Widget::GetRequisition() const {
	return m_requisition;
}

void Widget::SetRequisition( const sf::Vector2f& requisition ) {
	if( requisition.x > 0.f || requisition.y > 0.f ) {
		delete m_custom_requisition;
		m_custom_requisition = new sf::Vector2f( requisition );
	}
	else {
		delete m_custom_requisition;
		m_custom_requisition = 0;
	}

	RequestResize();
}

sf::Vector2f Widget::GetAbsolutePosition() const {
	// If no parent, allocation's position is absolute position.
	PtrConst parent( m_parent.lock() );

	if( !parent ) {
		return sf::Vector2f( GetAllocation().left, GetAllocation().top );
	}

	// Get parent's absolute position and add own rel. position to it.
	sf::Vector2f parent_position( parent->GetAbsolutePosition() );

	return sf::Vector2f(
		parent_position.x + GetAllocation().left,
		parent_position.y + GetAllocation().top
	);
}

void Widget::UpdateDrawablePosition() const {
	if( m_drawable ) {
		m_drawable->SetPosition( GetAbsolutePosition() );
	}
}

void Widget::SetId( const std::string& id ) {
	m_id = id;
}

const std::string& Widget::GetId() const {
	return m_id;
}

void Widget::SetClass( const std::string& cls ) {
	m_class = cls;
}

const std::string& Widget::GetClass() const {
	return m_class;
}

void Widget::HandleMouseMoveEvent( int /*x*/, int /*y*/ ) {
}

void Widget::HandleMouseButtonEvent( sf::Mouse::Button /*button*/, bool /*press*/, int /*x*/, int /*y*/ ) {
}

void Widget::HandleKeyEvent( sf::Keyboard::Key /*key*/, bool /*press*/ ) {
}

void Widget::HandlePositionChange() {
}

void Widget::HandleSizeChange() {
}

void Widget::HandleStateChange( State /*old_state*/ ) {
	Invalidate();
}

void Widget::HandleTextEvent( sf::Uint32 /*character*/ ) {
}

void Widget::HandleMouseEnter( int /*x*/, int /*y*/ ) {
}

void Widget::HandleMouseLeave( int /*x*/, int /*y*/ ) {
}

void Widget::HandleMouseClick( sf::Mouse::Button /*button*/, int /*x*/, int /*y*/ ) {
}

void Widget::HandleFocusChange( const Widget::Ptr& focused_widget ) {
	if( focused_widget != shared_from_this() ) {
		SetState( NORMAL );
	}
}

void Widget::HandleLocalVisibilityChange() {
}

void Widget::HandleGlobalVisibilityChange() {
	if( m_state == PRELIGHT ) {
		SetState( NORMAL );
	}

	if( m_drawable ) {
		m_drawable->Show( IsGloballyVisible() );
	}
}

void Widget::HandleAbsolutePositionChange() {
	UpdateDrawablePosition();
}

void Widget::Refresh() {
	RequestResize();

	Invalidate();
}

void Widget::HandleRequisitionChange() {
}

void Widget::HandleUpdate( float /*seconds*/ ) {
}

void Widget::HandleSetHierarchyLevel() {
	if( m_drawable ) {
		m_drawable->SetLevel( m_hierarchy_level );
	}
}

void Widget::SetHierarchyLevel( int level ) {
	m_hierarchy_level = level;

	HandleSetHierarchyLevel();
}

int Widget::GetHierarchyLevel() const {
	return m_hierarchy_level;
}

void Widget::SetViewport( const RendererViewport::Ptr& viewport ) {
	m_viewport = viewport;

	HandleViewportUpdate();
}

const RendererViewport::Ptr& Widget::GetViewport() const {
	return m_viewport;
}

void Widget::HandleViewportUpdate() {
	if( m_drawable ) {
		m_drawable->SetViewport( m_viewport );
	}
}

void Widget::SetActiveWidget() {
	GrabFocus( shared_from_this() );
}

void Widget::SetActiveWidget( Ptr widget ) {
	Container::Ptr parent( m_parent.lock() );

	if( !parent ) {
		m_active_widget = widget;
	}
	else {
		parent->SetActiveWidget( widget );
	}
}

bool Widget::IsActiveWidget() const {
	return HasFocus( shared_from_this() );
}

bool Widget::IsActiveWidget( PtrConst widget ) const {
	Container::Ptr parent( m_parent.lock() );

	if( !parent ) {
		if( m_active_widget == widget ) {
			return true;
		}

		return false;
	}

	return parent->IsActiveWidget( widget );
}

}
