#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Object.hpp>
#include <SFGUI/Signal.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <map>
#include <string>

namespace sfg {

class Container;

/** Base class for widgets.
 */
class SFGUI_API Widget : public Object, public EnableSharedFromThis<Widget> {
	public:
		typedef SharedPtr<Widget> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Widget> PtrConst; //!< Shared pointer.

		/** Widget state.
		 */
		enum State {
			NORMAL = 0, /*!< Normal. */
			ACTIVE, /*!< Active, e.g. when a button is pressed. */
			PRELIGHT, /*!< Prelight, e.g. when the mouse moves over a widget. */
			SELECTED, /*!< Selected, e.g. when a list item in a list is selected. */
			INSENSITIVE /*!< Insensitive, disabled widget. */
		};

		/** Destructor.
		 */
		virtual ~Widget();

		/** Check if widget is sensitive (enabled).
		 * @return true when sensitive.
		 */
		bool IsSensitive() const;

		/** Check if widget is locally visible, i.e. is rendered when allowed to.
		 * This DOES NOT take into account the global visibility of it's parents.
		 * @return true when locally visible.
		 */
		bool IsLocallyVisible() const;

		/** Check if widget is globaly visible, i.e. is currently rendered.
		 * This takes into account the global visibility of it's parents.
		 * @return true when globally visible.
		 */
		bool IsGloballyVisible() const;

		/** Show (or hide) widget.
		 * @param show true to show, false to hide.
		 */
		void Show( bool show = true );

		/** Get name of widget.
		 * @return Name.
		 */
		virtual const std::string& GetName() const = 0;

		/** Grab focus.
		 */
		void GrabFocus();

		/** Check if widget has focus.
		 * @return true if widget has focus.
		 */
		bool HasFocus() const;

		/** Set this widget to be active.
		 */
		void SetActiveWidget();

		/** Check if widget is active widget.
		 * @return true if widget is active widget.
		 */
		bool IsActiveWidget() const;

		/** Allocate size.
		 * @param rect Rect.
		 */
		void SetAllocation( const sf::FloatRect& rect );

		/** Request new allocation at parent.
		 */
		void RequestResize();

		/** Get allocated size (position and size).
		 * @return Rect.
		 */
		const sf::FloatRect& GetAllocation() const;

		/** Get requested size (requisition).
		 * @return Size.
		 */
		const sf::Vector2f& GetRequisition() const;

		/** Set a custom requisition.
		 * This can be compared to setting a minimum widget size. Mostly setting it
		 * is not needed because sizers take care of proper widget dimensions. You
		 * can specify 0 for width or height to enable calculating the requisition
		 * for each axis. If you specify 0 both for width and height, normal
		 * requisition calculation is re-enabled.
		 * @param requisition Custom requisition (skip argument to disable custom requisition).
		 */
		void SetRequisition( const sf::Vector2f& requisition = sf::Vector2f( 0.f, 0.f ) );

		/** Set position.
		 * @param position Position.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Update
		 * @param seconds Elapsed time in seconds.
		 */
		void Update( float seconds );

		/** Invalidate widget (prepare internal sf::Drawable).
		 * Implement InvalidateImpl() for your own code.
		 */
		void Invalidate() const;

		/** Set parent widget.
		 * Note that the parent must be a subclass of sfg::Container. You mostly
		 * don't want to call this method directly.
		 * @param parent Parent.
		 */
		void SetParent( const Widget::Ptr& parent );

		/** Get parent.
		 * @return Parent.
		 */
		SharedPtr<Container> GetParent();

		/** Get parent.
		 * @return Parent.
		 */
		SharedPtr<const Container> GetParent() const;

		/** Set widget's state.
		 * @param state State.
		 */
		void SetState( State state );

		/** Get widget's state.
		 * @return State.
		 */
		State GetState() const;

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals. Normally reimplemented by
		 * containers only.
		 * @param event SFML event.
		 */
		virtual void HandleEvent( const sf::Event& event );

		/** Get absolute position on screen.
		 * @return Absolute position.
		 */
		virtual sf::Vector2f GetAbsolutePosition() const;

		/** Handle changing of absolute position
		 */
		virtual void HandleAbsolutePositionChange();

		/** Handle global visibility change.
		 */
		virtual void HandleGlobalVisibilityChange();

		/** Update position of drawable.
		 */
		virtual void UpdateDrawablePosition() const;

		/** Set ID.
		 * @param id ID.
		 */
		void SetId( const std::string& id );

		/** Get ID.
		 * @return ID or empty.
		 */
		const std::string& GetId() const;

		/** Set class.
		 * @param cls Class.
		 */
		void SetClass( const std::string& cls );

		/** Get class.
		 * @return Class or empty.
		 */
		const std::string& GetClass() const;

		/** Refresh.
		 * Invalidates the widget and re-requests size
		 */
		virtual void Refresh();

		/** Set hierarchy level of this widget.
		 * @param level New hierarchy level of this widget..
		 */
		void SetHierarchyLevel( int level );

		/** Get hierarchy level of this widget.
		 * @return Hierarchy level of this widget..
		 */
		int GetHierarchyLevel() const;

		/** Set viewport of this widget.
		 * @param viewport Viewport of this widget..
		 */
		void SetViewport( const SharedPtr<RendererViewport>& viewport );

		/** Get viewport of this widget.
		 * @return Viewport of this widget..
		 */
		const SharedPtr<RendererViewport>& GetViewport() const;

		// Signals.
		Signal OnStateChange; //!< Fired when state changed.
		Signal OnGainFocus; //!< Fired when focus gained.
		Signal OnLostFocus; //!< Fired when focus lost.

		Signal OnExpose; //!< Fired when widget is being rendered.

		Signal OnSizeAllocate; //!< Fired when widget's allocation changed.
		Signal OnSizeRequest; //!< Fired when size was requested.

		Signal OnMouseEnter; //!< Fired when mouse entered widget.
		Signal OnMouseLeave; //!< Fired when mouse left widget.
		Signal OnMouseMove; //!< Fired when mouse moved over widget.
		Signal OnMouseLeftPress; //!< Fired when left button pressed.
		Signal OnMouseRightPress; //!< Fired when right button pressed.
		Signal OnMouseLeftRelease; //!< Fired when left button released.
		Signal OnMouseRightRelease; //!< Fired when right button released.

		Signal OnLeftClick; //!< Fired when left button clicked.
		Signal OnRightClick; //!< Fired when left button clicked.

		Signal OnKeyPress; //!< Fired when a key is pressed while State == Active.
		Signal OnKeyRelease; //!< Fired when a key is released while State == Active.
		Signal OnText; //!< Fired when text is entered while State == Active.

	protected:
		/** Constructor.
		 */
		Widget();

		/** Invalidate implementation (redraw internally).
		 * Gets called whenever the widget needs to be redrawn, e.g. due to a call
		 * to Invalidate().
		 * @return Pointer to new drawable -- ownership is taken by caller.
		 */
		virtual RenderQueue* InvalidateImpl() const;

		/** Requisition implementation (recalculate requisition).
		 * @return New requisition.
		 */
		virtual sf::Vector2f CalculateRequisition() = 0;

		/** Check if mouse is inside widget.
		 * @return true if mouse is inside.
		 */
		bool IsMouseInWidget() const;

		// Internal handling methods.

		/** Handle mouse move event.
		 * @param x Mouse X position.
		 * @param y Mouse Y position.
		 */
		virtual void HandleMouseMoveEvent( int x, int y );

		/** Handle mouse button event.
		 * @param button Mouse button.
		 * @param press true if button was pressed, false if released.
		 * @param x Mouse X position.
		 * @param y Mouse Y position.
		 */
		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );

		/** Handle key event.
		 * @param key Key.
		 * @param press true if button was pressed, false if released.
		 */
		virtual void HandleKeyEvent( sf::Keyboard::Key key, bool press );

		/** Handle widget (relative) position changes.
		 */
		virtual void HandlePositionChange();

		/** Handle widget size changes.
		 */
		virtual void HandleSizeChange();

		/** Handle requisition changes.
		 * Called after new requisition is set.
		 */
		virtual void HandleRequisitionChange();

		/** Handle state changes.
		 * The default behaviour is to accept any state change and invalidate the
		 * widget.
		 * @param old_state Old state.
		 */
		virtual void HandleStateChange( State old_state );

		/** Handle text event.
		 * @param character Character.
		 */
		virtual void HandleTextEvent( sf::Uint32 character );

		/** Handle mouse enter.
		 * @param x Mouse X position.
		 * @param y Mouse Y position.
		 */
		virtual void HandleMouseEnter( int x, int y );

		/** Handle mouse leave.
		 * @param x Mouse X position.
		 * @param y Mouse Y position.
		 */
		virtual void HandleMouseLeave( int x, int y );

		/** Handle mouse click.
		 * @param button Button.
		 * @param x Mouse X position.
		 * @param y Mouse Y position.
		 */
		virtual void HandleMouseClick( sf::Mouse::Button button, int x, int y );

		/** Handle focus change.
		 * @param focused_widget Widget currently being focused.
		 */
		virtual void HandleFocusChange( const Widget::Ptr& focused_widget );

		/** Handle local visibility change.
		 */
		virtual void HandleLocalVisibilityChange();

		/** Handle update.
		 */
		virtual void HandleUpdate( float seconds );

		/** Handle hierarchy level change.
		 */
		virtual void HandleSetHierarchyLevel();

		/** Handle viewport change.
		 */
		virtual void HandleViewportUpdate();

		/** Set the focused widget.
		 * @param widget Focused widget.
		 */
		void GrabFocus( Ptr widget );

		/** Check if a widget has focus.
		 * @param widget Checked widget.
		 * @return true if widget has focus.
		 */
		bool HasFocus( PtrConst widget ) const;

	private:
		void SetActiveWidget( Ptr widget );
		bool IsActiveWidget( PtrConst widget ) const;

		sf::FloatRect m_allocation;
		sf::Vector2f m_requisition;
		sf::Vector2f* m_custom_requisition;

		SharedPtr<RendererViewport> m_viewport;

		WeakPtr<Container> m_parent;

		Ptr m_focus_widget;
		Ptr m_active_widget;

		std::string m_id;
		std::string m_class;

		int m_hierarchy_level;

		mutable RenderQueue* m_drawable;

		bool m_sensitive;
		bool m_visible;
		bool m_drawn;

		unsigned char m_state;
		bool m_mouse_in;
		char m_mouse_button_down;

		mutable bool m_invalidated;
};

}
