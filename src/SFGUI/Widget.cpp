#include <SFGUI/Widget.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/Context.hpp>

namespace sfg {

Widget::Widget() :
	Object(),
	m_sensitive( true ),
	m_visible( true ),
	m_state( Normal ),
	m_mouse_in( false ),
	m_mouse_button_down( -1 ),
	m_allocation( 0, 0, 0, 0 ),
	m_requisition( 0, 0 ),
	m_invalidated( true ),
	m_recalc_requisition( true ),
	m_flags( NoFlags )
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

void Widget::GrabFocus( Ptr widget ) {
	Container::Ptr parent( m_parent.lock() );

	if( !parent ) {
		// Notify old focused widget.
		if( m_focus_widget ) {
			m_focus_widget->OnFocusChange();
			m_focus_widget->HandleFocusChange( widget );
		}

		m_focus_widget = widget;
		m_focus_widget->OnFocusChange();
		m_focus_widget->HandleFocusChange( widget );
	}
	else {
		parent->GrabFocus( widget );
	}
}

void Widget::AllocateSize( const sf::FloatRect& rect ) {
	sf::FloatRect  oldallocation( m_allocation );

	// Make sure allocation is pixel-aligned.
	m_allocation.Left = std::floor( rect.Left + .5f );
	m_allocation.Top = std::floor( rect.Top + .5f );
	m_allocation.Width = std::floor( rect.Width + .5f );
	m_allocation.Height = std::floor( rect.Height + .5f );

	if( !HandleSizeAllocate( oldallocation ) ) {
		m_allocation = oldallocation;
		return;
	}

	OnSizeAllocate();

	Invalidate();
}

void Widget::RequestSize() {
	m_recalc_requisition = true;
	Container::Ptr parent = m_parent.lock();

	// Notify observers.
	OnSizeRequest();

	if( parent ) {
		parent->RequestSize();
	}
	else {
		sf::Vector2f  requisition( GetRequisition() );

		sf::FloatRect  allocation(
			GetAllocation().Left,
			GetAllocation().Top,
			std::max( GetAllocation().Width, requisition.x ),
			std::max( GetAllocation().Height, requisition.y )
		);

		AllocateSize( allocation );
	}
}


const sf::FloatRect& Widget::GetAllocation() const {
	return m_allocation;
}

void Widget::Expose( sf::RenderTarget& target ) {
	if( m_invalidated ) {
		m_invalidated = false;

		m_drawable.reset( InvalidateImpl() );

		if( m_drawable ) {
			m_drawable->SetPosition( GetAbsolutePosition() );
		}
	}

	if( IsVisible() ) {
		if( m_drawable ) {
			target.Draw( *m_drawable );
		}

		HandleExpose( target );
		OnExpose();
	}
}

void Widget::Invalidate() {
	m_invalidated = true;
}

sf::Drawable* Widget::InvalidateImpl() {
	return 0;
}

void Widget::SetParent( Widget::Ptr parent ) {
	Container::Ptr  cont( std::dynamic_pointer_cast<Container>( parent ) );

	if( !cont ) {
		return;
	}

	Container::Ptr oldparent = m_parent.lock();

	if( oldparent ) {
		oldparent->Remove( shared_from_this() );
	}

	m_parent = cont;
}

void Widget::SetPosition( const sf::Vector2f& position ) {
	sf::FloatRect  oldallocation( GetAllocation() );

	// Make sure allocation is pixel-aligned.
	m_allocation.Left = std::floor( position.x + .5f );
	m_allocation.Top = std::floor( position.y + .5f );

	if( !HandleSizeAllocate( oldallocation ) ) {
		m_allocation = oldallocation;
		return;
	}

	if( m_drawable ) {
		m_drawable->SetPosition( GetAbsolutePosition() );
	}

	OnSizeAllocate();
}

Widget::HandleEventResult Widget::HandleEvent( const sf::Event& event ) {
	if( !IsVisible() ) {
		return IgnoreEvent;
	}

	Container::Ptr parent = m_parent.lock();
	HandleEventResult  result( PassEvent );

	switch( event.Type ) {
		case sf::Event::MouseMoved:
			// Drag operations.
			if( m_mouse_button_down == sf::Mouse::Left && HasFlag( Draggable ) ) {
				if( !m_drag_info ) {
					// Ask to begin dragging.
					m_drag_info.reset( new DragInfo( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) );

					if( !HandleDragOperation( DragInfo::Start, *m_drag_info ) ) {
						m_drag_info.reset();
					}
				}
				else {
					m_drag_info->Update( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) );

					if( !HandleDragOperation( DragInfo::Move, *m_drag_info ) ) {
						m_drag_info.reset();
					}
					else {
						OnDragMove();
					}
				}
			}

			// Check if pointer inside of widget's allocation.
			if( GetAllocation().Contains( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) {
				// Check for enter event.
				if( m_mouse_in == false ) {
					m_mouse_in = true;
					OnMouseEnter();
					HandleMouseEnter( event.MouseMove.X, event.MouseMove.Y );

					// Register hook to get notified when mouse leaves the widget.
					if( parent ) {
						parent->RegisterEventHook( sf::Event::MouseMoved, shared_from_this() );
					}
				}
				OnMouseMove();
			}
			else if( m_mouse_in == true ) { // Check for leave event.
				m_mouse_in = false;
				OnMouseLeave();
				HandleMouseLeave( event.MouseMove.X, event.MouseMove.Y );

				if( !m_drag_info && parent ) {
					parent->UnregisterEventHook( sf::Event::MouseMoved, shared_from_this() );
				}

				// Mouse left the widget's region, so don't continue to pass the event to
				// children. The event only reached the widget because the event got
				// hooked.
				result = IgnoreEvent;
			}
			else {
				result = IgnoreEvent;
			}

			if( HandleMouseMoveEvent( event.MouseMove.X, event.MouseMove.Y ) ) {
				result = EatEvent;
			}

			break;

		case sf::Event::MouseButtonPressed:
			// If a mouse button has already been pressed for this widget, drop further
			// presses. This maybe needs changing, but up to now, I can't think of any
			// cases where it would be useful to have such a functionality.
			if( m_mouse_button_down == -1 ) {
				if( m_mouse_in ) {
					m_mouse_button_down = event.MouseButton.Button;

					if( parent ) {
						parent->RegisterEventHook( sf::Event::MouseButtonReleased, shared_from_this() );
					}

					OnMouseButtonPress();
					if( HandleMouseButtonEvent( event.MouseButton.Button, true, event.MouseButton.X, event.MouseButton.Y ) ) {
						result = EatEvent;
						GrabFocus();
					}
				}
				else {
					result = IgnoreEvent;
				}
			}

			break;

		case sf::Event::MouseButtonReleased:
			// Only process when mouse button has been clicked inside the widget before.
			if( m_mouse_button_down == event.MouseButton.Button ) {
				m_mouse_button_down = -1;

				if( parent ) {
					parent->UnregisterEventHook( sf::Event::MouseButtonReleased, shared_from_this() );
				}

				// Dragged?
				if( m_drag_info ) {
					if( HandleDragOperation( DragInfo::End, *m_drag_info ) ) {
						OnDragEnd();
						m_drag_info.reset( 0 );

						if( parent ) {
							parent->UnregisterEventHook( sf::Event::MouseMoved, shared_from_this() );
						}
					}
				}

				// When released inside the widget, the event can be considered a click.
				if( m_mouse_in ) {
					HandleMouseClick( event.MouseButton.X, event.MouseButton.Y );
				}

				OnMouseButtonRelease();
				if( HandleMouseButtonEvent( event.MouseButton.Button, false, event.MouseButton.X, event.MouseButton.Y ) ) {
					result = EatEvent;
				}
			}
			else if( !m_mouse_in ) {
				result = IgnoreEvent;
			}

			if( HandleMouseButtonEvent( event.MouseButton.Button, false, event.MouseButton.X, event.MouseButton.Y ) ) {
				result = EatEvent;
			}

			break;

		case sf::Event::KeyPressed:
			if( GetState() == Active ) {
				// TODO: Delegate event too when widget's not active?
				if( HandleKeyEvent( event.Key.Code, true ) ) {
					result = EatEvent;
				}

				OnKeyPress();
			}

			break;

		case sf::Event::KeyReleased:
			if( GetState() == Active ) {
				// TODO: Delegate event too when widget's not active?
				if( HandleKeyEvent( event.Key.Code, false ) ) {
					result = EatEvent;
				}

				OnKeyRelease();
			}
			break;

		case sf::Event::TextEntered:
			if( GetState() == Active ) {
				// TODO: Delegate event too when widget's not active?
				if( HandleTextEvent( event.Text.Unicode ) ) {
					result = EatEvent;
				}

				OnText();
			}
			break;

		default:
			break;
	}

	return result;
}

void Widget::SetState( State state ) {
	State old_state( m_state );

	m_state = state;

	if( !HandleStateChange( old_state ) ) {
		m_state = old_state;
		return;
	}

	OnStateChange();
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

bool Widget::IsMouseInWidget() const {
	return m_mouse_in;
}

void Widget::Show( bool show ) {
	m_visible = show;
}

void Widget::SetFlags( int flags ) {
	m_flags = flags;
}

bool Widget::HasFlag( Flags flag ) const {
	return (m_flags & flag) == flag;
}

const sf::Vector2f& Widget::GetRequisition() const {
	// Check if we need to recalculate the requisition.
	if( m_recalc_requisition ) {
		m_requisition = GetRequisitionImpl();
		m_recalc_requisition = false;

		// Check if custom requsition set for width or height.
		if( m_custom_requisition ) {
			if( m_custom_requisition->x > 0.f ) {
				m_requisition.x = m_custom_requisition->x;
			}

			if( m_custom_requisition->y > 0.f ) {
				m_requisition.y = m_custom_requisition->y;
			}
		}
	}

	return m_requisition;
}

void Widget::SetRequisition( const sf::Vector2f& requisition ) {
	if( requisition.x > 0.f || requisition.y >= 0.f ) {
		m_custom_requisition.reset( new sf::Vector2f( requisition ) );
	}
	else {
		m_custom_requisition.reset();
	}

	// Set flag to recalculate requisition and request new size.
	m_recalc_requisition = true;
	RequestSize();
}

sf::Vector2f Widget::GetAbsolutePosition() const {
	// If no parent, allocation's position is absolute position.
	PtrConst parent( m_parent.lock() );

	if( !parent ) {
		return sf::Vector2f( GetAllocation().Left, GetAllocation().Top );
	}

	// Get parent's absolute position and add own rel. position to it.
	sf::Vector2f parent_position( parent->GetAbsolutePosition() );

	return sf::Vector2f(
		parent_position.x + GetAllocation().Left,
		parent_position.y + GetAllocation().Top
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

bool Widget::HandleMouseMoveEvent( int /*x*/, int /*y*/ ) {
	return false;
}

bool Widget::HandleMouseButtonEvent( sf::Mouse::Button /*button*/, bool /*press*/, int /*x*/, int /*y*/ ) {
	return false;
}

bool Widget::HandleKeyEvent( sf::Keyboard::Key /*key*/, bool /*press*/ ) {
	return false;
}

bool Widget::HandleSizeAllocate( const sf::FloatRect& /*new_allocation*/ ) {
	return true;
}

void Widget::HandleExpose( sf::RenderTarget& /*target*/ ) {
}

bool Widget::HandleDragOperation( DragInfo::State /*state*/, const DragInfo& /*drag_info*/ ) {
	return true;
}

bool Widget::HandleStateChange( State /*old_state*/ ) {
	Invalidate();
	return true;
}

bool Widget::HandleTextEvent( sf::Uint32 /*character*/ ) {
	return false;
}

void Widget::HandleMouseEnter( int /*x*/, int /*y*/ ) {
}

void Widget::HandleMouseLeave( int /*x*/, int /*y*/ ) {
}

void Widget::HandleMouseClick( int /*x*/, int /*y*/ ) {
}

void Widget::HandleFocusChange( Widget::Ptr /*focused_widget*/ ) {
}

}
