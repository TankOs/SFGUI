#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Object.hpp>
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
		typedef std::vector<Widget::Ptr> WidgetsList;

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
		 * The name of a widget is a descriptive name of the widget itself. E.g.
		 * "CheckButton" for the CheckButton widget. The name is mainly used by the
		 * style parser.
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
		 * Only one widget at a time can be active. This often happens when a
		 * widget is gaining focus. You normally don't want to call this yourself.
		 */
		void SetActiveWidget();

		/** Check if widget is active widget.
		 * @return true if widget is active widget.
		 */
		bool IsActiveWidget() const;

		/** Set allocation (position + size).
		 * @param rect Rect.
		 */
		void SetAllocation( const sf::FloatRect& rect );

		/** Request a resize at the parent widget.
		 * When a widget's requisition changes, it requests a resize at the parent
		 * to actually get more space (if possible).
		 */
		void RequestResize();

		/** Get allocation (position and size).
		 * @return Allocation.
		 */
		const sf::FloatRect& GetAllocation() const;

		/** Get requisition (minimum size the widget is asking for).
		 * @return Requisition.
		 */
		const sf::Vector2f& GetRequisition() const;

		/** Set custom requisition.
		 * This can be compared to setting a minimum widget size. Mostly setting it
		 * is not needed because layouters take care of proper widget dimensions.
		 * You can specify 0 for width and/or height to enable calculating the
		 * requisition for each axis. If you specify 0 both for width and height,
		 * normal requisition calculation is re-enabled.
		 * @param requisition Custom requisition (skip argument to disable custom requisition).
		 */
		void SetRequisition( const sf::Vector2f& requisition = sf::Vector2f( 0.f, 0.f ) );

		/** Set position.
		 * Shortcut for SetAllocation(): Only the position part of the allocation
		 * is changed.
		 * @param position Position.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Update.
		 * Update the widget's state, i.e. invalidate graphics, process animations
		 * etc.
		 * @param seconds Elapsed time in seconds.
		 */
		void Update( float seconds );

		/** Invalidate widget.
		 * When a widget is about to invalidate it will recreate itself in a
		 * graphical manner. This happens automatically when code detects the
		 * visual representation needs to be regenerated, so you normally don't
		 * have to call it yourself.
		 *
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

		/** Handle absolute position changes.
		 */
		virtual void HandleAbsolutePositionChange();

		/** Handle global visibility changes.
		 */
		virtual void HandleGlobalVisibilityChange();

		/** Update position of drawable.
		 */
		virtual void UpdateDrawablePosition() const;

		/** Set ID.
		 * Mostly used for identification and by styles (#widget_id).
		 * @param id ID.
		 */
		void SetId( const std::string& id );

		/** Get ID.
		 * @return ID or empty.
		 */
		std::string GetId() const;

		/** Set class.
		 * Mostly used by style (.widget_class).
		 * @param cls Class.
		 */
		void SetClass( const std::string& cls );

		/** Get class.
		 * @return Class or empty.
		 */
		std::string GetClass() const;

		/** Get all widgets with the specified ID.
		 * @param id ID the widget should have.
		 * @return Widget::Ptr of the first found widget with the specified ID or Widget::Ptr() if none found.
		 */
		static Widget::Ptr GetWidgetById( const std::string& id );

		/** Get all widgets with the specified class.
		 * @param class_name Class the widget should have.
		 * @return sfg::Widget::WidgetsList of all found widgets with the specified class. Empty if none found.
		 */
		static WidgetsList GetWidgetsByClass( const std::string& class_name );

		/** Refresh.
		 * Invalidate the widget and request resize.
		 */
		virtual void Refresh();

		/** Refreshes all root widgets, which should refresh all widgets in the hierarchy.
		 */
		static void RefreshAll();

		/** Set hierarchy level of this widget.
		 * @param level New hierarchy level of this widget.
		 */
		void SetHierarchyLevel( int level );

		/** Get hierarchy level of this widget.
		 * @return Hierarchy level of this widget.
		 */
		int GetHierarchyLevel() const;

		/** Set viewport of this widget.
		 * @param viewport Viewport of this widget.
		 */
		void SetViewport( const SharedPtr<RendererViewport>& viewport );

		/** Get viewport of this widget.
		 * @return Viewport of this widget.
		 */
		const SharedPtr<RendererViewport>& GetViewport() const;

		/** Get the Z layer this widget should be rendered in.
		 * Larger values are rendered later. Default: 0.
		 * @return Z layer this widget should be rendered in.
		 */
		int GetZOrder() const;

		/** Set the Z layer this widget should be rendered in.
		 * Larger values are rendered later. Default: 0.
		 * @param z_order Z layer this widget should be rendered in.
		 */
		void SetZOrder( int z_order );

		// Signals.
		static Signal::SignalID OnStateChange; //!< Fired when state changed.
		static Signal::SignalID OnGainFocus; //!< Fired when focus gained.
		static Signal::SignalID OnLostFocus; //!< Fired when focus lost.

		static Signal::SignalID OnExpose; //!< Fired when widget is being rendered.

		static Signal::SignalID OnSizeAllocate; //!< Fired when widget's allocation changed.
		static Signal::SignalID OnSizeRequest; //!< Fired when size was requested.

		static Signal::SignalID OnMouseEnter; //!< Fired when mouse entered widget.
		static Signal::SignalID OnMouseLeave; //!< Fired when mouse left widget.
		static Signal::SignalID OnMouseMove; //!< Fired when mouse moved over widget.
		static Signal::SignalID OnMouseLeftPress; //!< Fired when left button pressed.
		static Signal::SignalID OnMouseRightPress; //!< Fired when right button pressed.
		static Signal::SignalID OnMouseLeftRelease; //!< Fired when left button released.
		static Signal::SignalID OnMouseRightRelease; //!< Fired when right button released.

		static Signal::SignalID OnLeftClick; //!< Fired when left button clicked.
		static Signal::SignalID OnRightClick; //!< Fired when left button clicked.

		static Signal::SignalID OnKeyPress; //!< Fired when a key is pressed while State == Active.
		static Signal::SignalID OnKeyRelease; //!< Fired when a key is released while State == Active.
		static Signal::SignalID OnText; //!< Fired when text is entered while State == Active.

	protected:
		/** Constructor.
		 */
		Widget();

		/** Invalidate implementation (redraw internally).
		 * Gets called whenever the widget needs to be redrawn, e.g. due to a call
		 * to Invalidate().
		 * @return Pointer to RenderQueue -- ownership is taken by caller.
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

		/** Set whether the mouse is in the widget or not.
		 * @param in_widget true if mouse should be in widget.
		 */
		void SetMouseInWidget( bool in_widget );

		/** Check if the given mouse button is down
		 * @param button Given mouse button. Defaults to check if ANY button is down.
		 * @return true if the given mouse button is down.
		 */
		bool IsMouseButtonDown( sf::Mouse::Button button = sf::Mouse::ButtonCount ) const;

		/** Set whether the given mouse button is down.
		 * @param button Given mouse button. Defaults to clear button down state for all buttons.
		 */
		void SetMouseButtonDown( sf::Mouse::Button button = sf::Mouse::ButtonCount );

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
		static void GrabFocus( Ptr widget );

		/** Check if a widget has focus.
		 * @param widget Checked widget.
		 * @return true if widget has focus.
		 */
		static bool HasFocus( PtrConst widget );

		/** Set this widget as the current modal widget.
		 * Consumes all events.
		 */
		void GrabModal();

		/** Unset this widget as the current modal widget.
		 * Restores normal event propagation.
		 */
		void ReleaseModal();

		/** Check if this widget is the modal widget.
		 * @return true if this widget is the modal widget.
		 */
		bool IsModal() const;

		/** Check if a widget is currently a modal widget.
		 * @return true if a widget is currently a modal widget.
		 */
		static bool HasModal();

	private:
		struct ClassId {
			std::string id;
			std::string class_;
		};

		static void SetActiveWidget( Ptr widget );
		static bool IsActiveWidget( PtrConst widget );

		static const std::vector<Widget*>& GetRootWidgets();

		sf::FloatRect m_allocation;
		sf::Vector2f m_requisition;
		sf::Vector2f* m_custom_requisition;

		SharedPtr<RendererViewport> m_viewport;

		WeakPtr<Container> m_parent;

		static WeakPtr<Widget> m_focus_widget;
		static WeakPtr<Widget> m_active_widget;
		static WeakPtr<Widget> m_modal_widget;

		static std::vector<Widget*> m_root_widgets;

		ClassId* m_class_id;

		int m_hierarchy_level;
		int m_z_order;

		mutable RenderQueue* m_drawable;

		/**
		 * Bit-field
		 * 7=6=5 4 3=2=1 0
		 * ^ ^ ^ ^ ^ ^ ^ ^
		 * | | | | | | | |
		 * | | | | | | | visible
		 * | | | | | | |
		 * | | | | state
		 * | | | |
		 * | | | mouse_in
		 * | | |
		 * mouse_button_down
		 */

		unsigned char m_bitfield;

		mutable bool m_invalidated;
};

}
