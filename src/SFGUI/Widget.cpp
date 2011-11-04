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

	HandleSizeAllocate( oldallocation );

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

	HandleSizeAllocate( oldallocation );

	if( m_drawable ) {
		m_drawable->SetPosition( GetAbsolutePosition() );
	}

	OnSizeAllocate();
}

void Widget::HandleEvent( const sf::Event& event ) {
	if( !IsVisible() ) {
		return;
	}

	Container::Ptr parent( m_parent.lock() );

	switch( event.Type ) {
		case sf::Event::MouseMoved:
			// Drag operations.
			if( m_mouse_button_down == sf::Mouse::Left && HasFlag( Draggable ) ) {
				if( !m_drag_info ) {
					// Ask to begin dragging.
					m_drag_info.reset( new DragInfo( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) );
					HandleDragOperation( DragInfo::Start, *m_drag_info );
				}
				else {
					m_drag_info->Update( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) );

					HandleDragOperation( DragInfo::Move, *m_drag_info );
					OnDragMove();
				}
			}

			// Check if pointer inside of widget's allocation.
			if( GetAllocation().Contains( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) {
				// Check for enter event.
				if( m_mouse_in == false ) {
					m_mouse_in = true;
					OnMouseEnter();
					HandleMouseEnter( event.MouseMove.X, event.MouseMove.Y );
				}

				OnMouseMove();
			}
			else if( m_mouse_in == true ) { // Check for leave event.
				m_mouse_in = false;
				OnMouseLeave();
				HandleMouseLeave( event.MouseMove.X, event.MouseMove.Y );
			}

			HandleMouseMoveEvent( event.MouseMove.X, event.MouseMove.Y );
			break;

		case sf::Event::MouseButtonPressed:
			// If a mouse button has already been pressed for this widget, drop further
			// presses. This maybe needs changing, but up to now, I can't think of any
			// cases where it would be useful to have such a functionality.
			if( m_mouse_button_down == -1 ) {
				if( m_mouse_in ) {
					m_mouse_button_down = event.MouseButton.Button;

					HandleMouseButtonEvent( event.MouseButton.Button, true, event.MouseButton.X, event.MouseButton.Y );
					OnMouseButtonPress();
				}
			}

			break;

		case sf::Event::MouseButtonReleased:
			// Only process when mouse button has been clicked inside the widget before.
			if( m_mouse_button_down == event.MouseButton.Button ) {
				m_mouse_button_down = -1;

				// Dragged?
				if( m_drag_info ) {
					m_drag_info.reset( 0 );
					HandleDragOperation( DragInfo::End, *m_drag_info );

					if( !m_drag_info ) {
						OnDragEnd();
					}
				}

				// When released inside the widget, the event can be considered a click.
				if( m_mouse_in ) {
					HandleMouseClick( event.MouseButton.X, event.MouseButton.Y );
				}

				OnMouseButtonRelease();
				HandleMouseButtonEvent( event.MouseButton.Button, false, event.MouseButton.X, event.MouseButton.Y );
			}

			HandleMouseButtonEvent( event.MouseButton.Button, false, event.MouseButton.X, event.MouseButton.Y );
			break;

		case sf::Event::KeyPressed:
			if( GetState() == Active ) {
				// TODO: Delegate event too when widget's not active?
				HandleKeyEvent( event.Key.Code, true );
				OnKeyPress();
			}

			break;

		case sf::Event::KeyReleased:
			if( GetState() == Active ) {
				// TODO: Delegate event too when widget's not active?
				HandleKeyEvent( event.Key.Code, false );
				OnKeyRelease();
			}
			break;

		case sf::Event::TextEntered:
			if( GetState() == Active ) {
				// TODO: Delegate event too when widget's not active?
				HandleTextEvent( event.Text.Unicode );
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

	State old_state( m_state );
	m_state = state;

	HandleStateChange( old_state );

	// If HandleStateChange() changed the state, do not call observer, will be
	// done from there too.
	if( m_state != state ) {
		OnStateChange();
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

void Widget::HandleMouseMoveEvent( int /*x*/, int /*y*/ ) {
}

void Widget::HandleMouseButtonEvent( sf::Mouse::Button /*button*/, bool /*press*/, int /*x*/, int /*y*/ ) {
}

void Widget::HandleKeyEvent( sf::Keyboard::Key /*key*/, bool /*press*/ ) {
}

void Widget::HandleSizeAllocate( const sf::FloatRect& /*new_allocation*/ ) {
}

void Widget::HandleExpose( sf::RenderTarget& /*target*/ ) {
}

void Widget::HandleDragOperation( DragInfo::State /*state*/, const DragInfo& /*drag_info*/ ) {
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

void Widget::HandleMouseClick( int /*x*/, int /*y*/ ) {
}

void Widget::HandleFocusChange( Widget::Ptr /*focused_widget*/ ) {
}

}
