#pragma once
#include <SFGUI/Container.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <deque>

namespace sfg {

/** Notebook.
 */
class SFGUI_API Notebook : public Container {
	public:
		typedef SharedPtr<Notebook> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Notebook> PtrConst; //!< Shared pointer.
		typedef int IndexType; ///< Type for tab indices.

		/** Tab Position
		 */
		enum TabPosition {
			TOP = 1 << 0,
			BOTTOM = 1 << 1,
			LEFT = 1 << 2,
			RIGHT = 1 << 3
		};

		/** Create notebook.
		 * @return Notebook.
		 */
		static Ptr Create();

		virtual const std::string& GetName() const;

		/** Appends a new page at the end of the Notebook.
		 * @param child Widget in the new page.
		 * @param tab_label Widget used as label on the page tab.
		 * @return Index of the new page.
		 */
		IndexType AppendPage( const Widget::Ptr& child, const Widget::Ptr& tab_label );

		/** Prepends a new page at the beginning of the Notebook.
		 * @param child Widget in the new page.
		 * @param tab_label Widget used as label on the page tab.
		 * @return Index of the new page.
		 */
		IndexType PrependPage( const Widget::Ptr& child, const Widget::Ptr& tab_label );

		/** Inserts a new page at given position into the Notebook.
		 * @param child Widget in the new page.
		 * @param tab_label Widget used as label on the page tab.
		 * @param position Position to insert at. If negative or larger than Notebook size it appends the child.
		 * @return Index of the new page.
		 */
		IndexType InsertPage( const Widget::Ptr& child, const Widget::Ptr& tab_label, IndexType position );

		/** Removes the page with given number from the Notebook.
		 * @param page_number Number of the page to be removed. If negative or larger than Notebook, removes last page.
		 */
		void RemovePage( IndexType page_number );

		/** Gets the number of the first page containing given widget.
		 * @param widget Widget to search for.
		 * @return Number of the first page containing widget or -1 if widget is not in Notebook.
		 */
		IndexType GetPageOf( const Widget::Ptr& widget ) const;

		/** Switches current page to next page if possible.
		 */
		void NextPage();

		/** Switches current page to previous page if possible.
		 */
		void PreviousPage();

		/** Reorders child to be in a page at given position.
		 * @param child Child to reorder.
		 * @param position Position to reorder to.
		 */
		void ReorderChild( const Widget::Ptr& child, IndexType position );

		/** Gets the position of the page tabs.
		 * @return Position of the page tabs.
		 */
		TabPosition GetTabPosition() const;

		/** Sets the position of the page tabs.
		 * @param tab_position Position of the page tabs.
		 */
		void SetTabPosition( TabPosition tab_position );

		/** Gets the number of the currently displayed page.
		 * @return Number of the currently displayed page.
		 */
		IndexType GetCurrentPage() const;

		/** Gets the child widget in the page with given number.
		 * @return Child widget in the page with given number or Widget::Ptr() if page number is out of bounds.
		 */
		const Widget::Ptr GetNthPage( IndexType page_number ) const;

		/** Gets the tab label for the page with given number.
		 * @return Tab label for the page with given number or Widget::Ptr() if page number is out of bounds.
		 */
		const Widget::Ptr GetNthTabLabel( IndexType page_number ) const;

		/** Get the total number of pages in this Notebook.
		 * @return Total number of pages in this Notebook.
		 */
		IndexType GetPageCount() const;

		/** Gets the tab label for the page with containing the given child.
		 * @return Tab label for the page containing the given child or Widget::Ptr() if page number is out of bounds.
		 */
		const Widget::Ptr GetTabLabel( const Widget::Ptr& child ) const;

		/** Sets the tab label for the page with containing the given child.
		 * @param child Child whose page label is to be set.
		 * @param tab_label New tab label for the page containing the given child.
		 */
		void SetTabLabel( const Widget::Ptr& child, const Widget::Ptr& tab_label );

		/** Sets the current page of the Notebook.
		 * @param page_number Page to set as current for this Notebook. Sets last page as current if page number is out of bounds.
		 */
		void SetCurrentPage( IndexType page_number );

		/** Get the tab currently under the mouse cursor.
		 * @return Number of the tab currently unser the mouse cursor or -1 if none.
		 */
		IndexType GetPrelightTab() const;

		/** Set whether the tabs of this Notebook are scrollable.
		 * @param scrollable True to set the tabs of this Notebook as scrollable.
		 */
		void SetScrollable( bool scrollable );

		/** Check whether the tabs of this Notebook scrollable.
		 * @return true when the tabs of this Notebook are scrollable.
		 */
		bool GetScrollable() const;

		/** Get the index of the first tab currently displayed on the tab bar.
		 * @return Index of the first tab currently displayed on the tab bar.
		 */
		IndexType GetFirstDisplayedTab() const;

		/** Get the total number of tabs currently displayed on the tab bar.
		 * @return Total number of tabs currently displayed on the tab bar.
		 */
		IndexType GetDisplayedTabCount() const;

		/** Are the tabs being scrolled forward.
		 * @return true if the tabs are being scrolled forward.
		 */
		bool IsScrollingForward() const;

		/** Are the tabs being scrolled backward.
		 * @return true if the tabs are being scrolled backward.
		 */
		bool IsScrollingBackward() const;

		/** Is the forward scrolling button in prelight state.
		 * @return true if the forward scrolling button is in prelight state.
		 */
		bool IsForwardScrollPrelight() const;

		/** Is the backward scrolling button in prelight state.
		 * @return true if the backward scrolling button is in prelight state.
		 */
		bool IsBackwardScrollPrelight() const;

	protected:
		/** Ctor.
		 */
		Notebook();

		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		struct ChildLabelPair {
			Widget::Ptr child;
			Widget::Ptr tab_label;

			ChildLabelPair( const Widget::Ptr& child_, const Widget::Ptr& tab_label_ );
			bool operator==( const ChildLabelPair& rhs ) const;
		};

		typedef std::deque<ChildLabelPair> ChildrenList;

		void HandleMouseMoveEvent( int x, int y );
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		void HandleAdd( const Widget::Ptr& child );
		void HandleRemove( const Widget::Ptr& child );
		void HandleSizeChange();
		void HandleUpdate( float seconds );
		void HandleGlobalVisibilityChange();

		void RecalculateSize();

		sf::Vector2f m_tab_requisition;
		sf::Vector2f m_child_requisition;

		ChildrenList m_children;

		IndexType m_current_page;
		IndexType m_prelight_tab;
		IndexType m_first_tab;
		IndexType m_num_displayed_tabs;

		unsigned char m_tab_position;

		float m_elapsed_time;

		bool m_scrollable;

		bool m_scrolling_forward;
		bool m_scrolling_backward;

		bool m_forward_scroll_prelight;
		bool m_backward_scroll_prelight;
};

}
