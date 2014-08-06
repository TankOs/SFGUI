#include <SFGUI/Widget.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Primitive.hpp>

#include <SFML/Window/Event.hpp>
#include <cmath>
#include <limits>

namespace {

std::weak_ptr<sfg::Widget> focus_widget;
std::weak_ptr<sfg::Widget> active_widget;
std::weak_ptr<sfg::Widget> modal_widget;

std::vector<sfg::Widget*> root_widgets;

}

namespace sfg {

// Signals.
Signal::SignalID Widget::OnStateChange = 0;
Signal::SignalID Widget::OnGainFocus = 0;
Signal::SignalID Widget::OnLostFocus = 0;

Signal::SignalID Widget::OnExpose = 0;

Signal::SignalID Widget::OnSizeAllocate = 0;
Signal::SignalID Widget::OnSizeRequest = 0;

Signal::SignalID Widget::OnMouseEnter = 0;
Signal::SignalID Widget::OnMouseLeave = 0;
Signal::SignalID Widget::OnMouseMove = 0;
Signal::SignalID Widget::OnMouseLeftPress = 0;
Signal::SignalID Widget::OnMouseRightPress = 0;
Signal::SignalID Widget::OnMouseLeftRelease = 0;
Signal::SignalID Widget::OnMouseRightRelease = 0;

Signal::SignalID Widget::OnLeftClick = 0;
Signal::SignalID Widget::OnRightClick = 0;

Signal::SignalID Widget::OnKeyPress = 0;
Signal::SignalID Widget::OnKeyRelease = 0;
Signal::SignalID Widget::OnText = 0;

Widget::Widget() :
	m_hierarchy_level( 0 ),
	m_z_order( 0 ),
	m_invalidated( true ),
	m_parent_notified( false ),
	m_state( State::NORMAL ),
	m_mouse_button_down( false ),
	m_mouse_in( false ),
	m_visible( true )
{
	m_viewport = Renderer::Get().GetDefaultViewport();

	// Register this as a root widget initially.
	root_widgets.push_back( this );
}

Widget::~Widget() {
	if( !m_parent.lock() ) {
		// If this widget is an orphan, we assume it is
		// a root widget and try to de-register it.
		std::vector<Widget*>::iterator iter( std::find( root_widgets.begin(), root_widgets.end(), this ) );

		if( iter != root_widgets.end() ) {
			root_widgets.erase( iter );
		}
	}
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
	// Notify old focused widget.
	if( focus_widget.lock() ) {
		focus_widget.lock()->GetSignals().Emit( OnLostFocus );
		focus_widget.lock()->HandleFocusChange( widget );
	}

	focus_widget = widget;

	if( focus_widget.lock() ) {
		focus_widget.lock()->GetSignals().Emit( OnGainFocus );
		focus_widget.lock()->HandleFocusChange( widget );
	}
}

bool Widget::HasFocus( PtrConst widget ) {
	if( focus_widget.lock() == widget ) {
		return true;
	}

	return false;
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

	  GetSignals().Emit( OnSizeAllocate );
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

	auto parent = m_parent.lock();

	// Notify observers.
	GetSignals().Emit( OnSizeRequest );

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
		m_parent_notified = false;

		m_drawable = InvalidateImpl();

		if( m_drawable ) {
			m_drawable->SetPosition( GetAbsolutePosition() );
			m_drawable->SetLevel( m_hierarchy_level );
			m_drawable->SetZOrder( m_z_order );
			m_drawable->Show( IsGloballyVisible() );

			// We don't want to propagate container viewports for Canvases,
			// they have their own special viewport for drawing.
			if( m_drawable->GetPrimitives().empty() || !m_drawable->GetPrimitives()[0]->GetCustomDrawCallback() ) {
				m_drawable->SetViewport( m_viewport );
			}
		}
	}

	HandleUpdate( seconds );
}

void Widget::Invalidate() const {
	if( !m_invalidated ) {
		m_invalidated = true;
	}

	if( !m_parent_notified ) {
		auto parent = m_parent.lock();

		if( parent ) {
			m_parent_notified = true;

			parent->HandleChildInvalidate( static_cast<Widget::PtrConst>( shared_from_this() ) );
		}
	}
}

std::unique_ptr<RenderQueue> Widget::InvalidateImpl() const {
	return nullptr;
}

void Widget::SetParent( Widget::Ptr parent ) {
	auto cont = std::dynamic_pointer_cast<Container>( parent );
	auto oldparent = m_parent.lock();

	if( cont == oldparent ) {
		return;
	}

	if( oldparent ) {
		oldparent->Remove( shared_from_this() );
	}

	m_parent = cont;

	auto iter = std::find( root_widgets.begin(), root_widgets.end(), this );

	if( parent ) {
		// If this widget has a parent, it is no longer a root widget.
		if( iter != root_widgets.end() ) {
			root_widgets.erase( iter );
		}

		SetHierarchyLevel( parent->GetHierarchyLevel() + 1 );
	}
	else {
		// If this widget does not have a parent, it becomes a root widget.
		if( iter == root_widgets.end() ) {
			root_widgets.push_back( this );
		}

		SetHierarchyLevel( 0 );
	}

	HandleAbsolutePositionChange();
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

	// Ignore the event if widget is insensitive
	if ( GetState() == State::INSENSITIVE ) {
		return;
	}

	// Ignore the event if another widget is active.
	if( !IsActiveWidget() && !IsActiveWidget( PtrConst() ) ) {
		return;
	}

	// Ignore the event if another widget is modal.
	if( HasModal() && !IsModal() ) {
		return;
	}

	// Set widget active in context.
	Context::Get().SetActiveWidget( shared_from_this() );

	auto parent = m_parent.lock();

	auto emit_leave = false;
	auto emit_enter = false;
	auto emit_move = false;
	auto emit_left_click = false;
	auto emit_right_click = false;

	try {
		switch( event.type ) {
			case sf::Event::MouseLeft:
				if( IsMouseInWidget() ) {
					SetMouseInWidget( false );

					HandleMouseLeave( std::numeric_limits<int>::min(), std::numeric_limits<int>::min() );

					emit_leave = true;
				}

				HandleMouseMoveEvent( std::numeric_limits<int>::min(), std::numeric_limits<int>::min() );

				SetMouseButtonDown();
				HandleMouseButtonEvent( sf::Mouse::Left, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::min() );
				HandleMouseButtonEvent( sf::Mouse::Right, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::min() );

				if( emit_leave ) {
					GetSignals().Emit( OnMouseLeave );
				}

				break;

			case sf::Event::MouseMoved:
				// Check if pointer inside of widget's allocation.
				if( GetAllocation().contains( static_cast<float>( event.mouseMove.x ), static_cast<float>( event.mouseMove.y ) ) ) {
					// Check for enter event.
					if( !IsMouseInWidget() ) {
						SetMouseInWidget( true );

						emit_enter = true;

						HandleMouseEnter( event.mouseMove.x, event.mouseMove.y );
					}

					emit_move = true;
				}
				else if( IsMouseInWidget() ) { // Check for leave event.
					SetMouseInWidget( false );

					emit_leave = true;

					HandleMouseLeave( event.mouseMove.x, event.mouseMove.y );
				}

				HandleMouseMoveEvent( event.mouseMove.x, event.mouseMove.y );

				if( emit_move ) {
					if( emit_enter ) {
						GetSignals().Emit( OnMouseEnter );
					}

					GetSignals().Emit( OnMouseMove );
				}
				else if( emit_leave ) {
					GetSignals().Emit( OnMouseLeave );
				}

				break;

			case sf::Event::MouseButtonPressed:
				if( !IsMouseButtonDown() && IsMouseInWidget() ) {
					SetMouseButtonDown( event.mouseButton.button );
				}

				HandleMouseButtonEvent( event.mouseButton.button, true, event.mouseButton.x, event.mouseButton.y );

				if( IsMouseInWidget() ) {
					if( event.mouseButton.button == sf::Mouse::Left ) {
						GetSignals().Emit( OnMouseLeftPress );
					}
					else if( event.mouseButton.button == sf::Mouse::Right ) {
						GetSignals().Emit( OnMouseRightPress );
					}
				}

				break;

			case sf::Event::MouseButtonReleased:
				// Only process as a click when mouse button has been pressed inside the widget before.
				if( IsMouseButtonDown( event.mouseButton.button ) ) {
					SetMouseButtonDown();

					// When released inside the widget, the event can be considered a click.
					if( IsMouseInWidget() ) {
						HandleMouseClick( event.mouseButton.button, event.mouseButton.x, event.mouseButton.y );

						if( event.mouseButton.button == sf::Mouse::Left ) {
							emit_left_click = true;
						}
						else if( event.mouseButton.button == sf::Mouse::Right ) {
							emit_right_click = true;
						}
					}
				}

				HandleMouseButtonEvent( event.mouseButton.button, false, event.mouseButton.x, event.mouseButton.y );

				if( emit_left_click ) {
					GetSignals().Emit( OnLeftClick );
				}
				else if( emit_right_click ) {
					GetSignals().Emit( OnRightClick );
				}

				if( IsMouseInWidget() ) {
					if( event.mouseButton.button == sf::Mouse::Left ) {
						GetSignals().Emit( OnMouseLeftRelease );
					}
					else if( event.mouseButton.button == sf::Mouse::Right ) {
						GetSignals().Emit( OnMouseRightRelease );
					}
				}

				break;

			case sf::Event::KeyPressed:
				if( HasFocus() ) {
					// TODO: Delegate event too when widget's not active?
					HandleKeyEvent( event.key.code, true );
					GetSignals().Emit( OnKeyPress );
				}

				break;

			case sf::Event::KeyReleased:
				if( HasFocus() ) {
					// TODO: Delegate event too when widget's not active?
					HandleKeyEvent( event.key.code, false );
					GetSignals().Emit( OnKeyRelease );
				}
				break;

			case sf::Event::TextEntered:
				if( HasFocus() ) {
					// TODO: Delegate event too when widget's not active?
					HandleTextEvent( event.text.unicode );
					GetSignals().Emit( OnText );
				}
				break;

			default:
				break;
		}
	}
	catch( ... ) {
		SetState( State::NORMAL );
		throw;
	}
}

void Widget::SetState( State state ) {
	// Do nothing if state wouldn't change.
	if( GetState() == state ) {
		return;
	}

	auto old_state = GetState();

	// Store the new state.
	m_state = state;

	auto emit_state_change = false;

	// If HandleStateChange() changed the state, do not call observer, will be
	// done from there too.
	if( GetState() != old_state ) {
		HandleStateChange( static_cast<State>( old_state ) );
		emit_state_change = true;
	}

	if( state == State::ACTIVE ) {
		GrabFocus( shared_from_this() );
		SetActiveWidget( shared_from_this() );
	}
	else if( old_state == State::ACTIVE ) {
		SetActiveWidget( Ptr() );
	}

	if( emit_state_change ) {
		GetSignals().Emit( OnStateChange );
	}
}

Widget::State Widget::GetState() const {
	return m_state;
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

void Widget::SetMouseInWidget( bool in_widget ) {
	m_mouse_in = in_widget;
}

bool Widget::IsMouseButtonDown( sf::Mouse::Button button ) const {
	// Check if any button is down if requested.
	if( button == sf::Mouse::ButtonCount ) {
		return m_mouse_button_down != sf::Mouse::ButtonCount;
	}

	// Check if requested button is down.
	return m_mouse_button_down == button;
}

void Widget::SetMouseButtonDown( sf::Mouse::Button button ) {
	m_mouse_button_down = static_cast<unsigned char>( button & 0x3f ); // 6 bits
}

void Widget::Show( bool show ) {
	if( show == IsLocallyVisible() ) {
		return;
	}

	auto old_global_visibility = IsGloballyVisible();

	// Flip the visible bit since we know show != IsLocallyVisible()
	m_visible = !m_visible;

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
		m_custom_requisition.reset( new sf::Vector2f( requisition ) );
	}
	else {
		m_custom_requisition.reset();
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
	if( id.empty() ) {
		return;
	}

	if( !m_class_id ) {
		m_class_id.reset( new ClassId );
	}

	m_class_id->id = id;

	Refresh();
}

std::string Widget::GetId() const {
	if( !m_class_id ) {
		return "";
	}

	return m_class_id->id;
}

void Widget::SetClass( const std::string& cls ) {
	if( cls.empty() ) {
		return;
	}

	if( !m_class_id ) {
		m_class_id.reset( new ClassId );
	}

	m_class_id->class_ = cls;

	Refresh();
}

std::string Widget::GetClass() const {
	if( !m_class_id ) {
		return "";
	}

	return m_class_id->class_;
}

Widget::Ptr SearchContainerForId( Container::PtrConst container, const std::string& id ) {
	if( !container ) {
		return Widget::Ptr();
	}

	for( const auto& child : container->GetChildren() ) {
		if( child->GetId() == id ) {
			return child;
		}

		auto child_container = std::dynamic_pointer_cast<Container>( child );

		if( child_container ) {
			auto widget = SearchContainerForId( child_container, id );

			if( widget ) {
				return widget;
			}
		}
	}

	return Widget::Ptr();
}

Widget::Ptr Widget::GetWidgetById( const std::string& id ) {
	for( const auto& root_widget : root_widgets ) {
		if( root_widget->GetId() == id ) {
			return root_widget->shared_from_this();
		}

		auto container = std::dynamic_pointer_cast<Container>(
			root_widget->shared_from_this()
		);

		if( container ) {
			auto widget = SearchContainerForId( container, id );

			if( widget ) {
				return widget;
			}
		}
	}

	return Widget::Ptr();
}

Widget::WidgetsList SearchContainerForClass( Container::PtrConst container, const std::string& class_name ) {
	Widget::WidgetsList result;

	if( !container ) {
		return result;
	}

	for( const auto& child : container->GetChildren() ) {
		if( child->GetClass() == class_name ) {
			result.push_back( child );
		}

		auto child_container = std::dynamic_pointer_cast<Container>( child );

		if( child_container ) {
			auto child_result = SearchContainerForClass( child_container, class_name );

			// Splice the 2 vectors.
			if( !child_result.empty() ) {
				result.reserve( child_result.size() );
				result.insert( result.end(), child_result.begin(), child_result.end() );
			}
		}
	}

	return result;
}

Widget::WidgetsList Widget::GetWidgetsByClass( const std::string& class_name ) {
	WidgetsList result;

	for( const auto& root_widget : root_widgets ) {
		if( root_widget->GetClass() == class_name ) {
			result.push_back( root_widget->shared_from_this() );
		}

		auto container = std::dynamic_pointer_cast<Container>(
			root_widget->shared_from_this()
		);

		if( container ) {
			auto container_result = SearchContainerForClass( container, class_name );

			// Splice the 2 vectors.
			if( !container_result.empty() ) {
				result.reserve( container_result.size() );
				result.insert( result.end(), container_result.begin(), container_result.end() );
			}
		}
	}

	return result;
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

void Widget::HandleFocusChange( Widget::Ptr focused_widget ) {
	if( ( focused_widget != shared_from_this() ) && ( GetState() == State::ACTIVE ) ) {
		SetState( State::NORMAL );
	}
}

void Widget::HandleLocalVisibilityChange() {
}

void Widget::HandleGlobalVisibilityChange() {
	auto state = GetState();

	if( ( state == State::PRELIGHT ) || ( state == State::ACTIVE ) ) {
		SetState( State::NORMAL );
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

void Widget::RefreshAll() {
	for( const auto& root_widget : root_widgets ) {
		root_widget->Refresh();
	}
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

void Widget::SetViewport( RendererViewport::Ptr viewport ) {
	m_viewport = viewport;

	HandleViewportUpdate();
}

RendererViewport::Ptr Widget::GetViewport() const {
	return m_viewport;
}

int Widget::GetZOrder() const {
	return m_z_order;
}

void Widget::SetZOrder( int z_order ) {
	m_z_order = z_order;

	if( m_drawable ) {
		m_drawable->SetZOrder( z_order );
	}
}

void Widget::HandleViewportUpdate() {
	if( m_drawable ) {
		m_drawable->SetViewport( m_viewport );
	}
}

void Widget::SetActiveWidget() {
	SetActiveWidget( shared_from_this() );
}

void Widget::SetActiveWidget( Ptr widget ) {
	active_widget = widget;
}

bool Widget::IsActiveWidget() const {
	return IsActiveWidget( shared_from_this() );
}

bool Widget::IsActiveWidget( PtrConst widget ) {
	if( active_widget.lock() == widget ) {
		return true;
	}

	return false;
}

void Widget::GrabModal() {
	if( modal_widget.lock() ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Tried to grab modal while existing widget has it.\n";
#endif

		return;
	}

	modal_widget = shared_from_this();
}

void Widget::ReleaseModal() {
	if( modal_widget.lock() == shared_from_this() ) {
		modal_widget.reset();

		return;
	}

#if defined( SFGUI_DEBUG )
	std::cerr << "SFGUI warning: Tried to release modal although current widget not modal.\n";
#endif
}

bool Widget::IsModal() const {
	if( modal_widget.lock() == shared_from_this() ) {
		return true;
	}

	return false;
}

bool Widget::HasModal() {
	return !modal_widget.expired();
}

const std::vector<Widget*>& Widget::GetRootWidgets() {
	return root_widgets;
}

}
