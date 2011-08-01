#ifndef SFGUI_LISTBOX_HPP
#define SFGUI_LISTBOX_HPP

#include <deque>
#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Clock.hpp>

namespace sfg {

/** List box
 */
class ListBox : public Widget {
	public:
		typedef boost::shared_ptr<ListBox>  Ptr; //!< Shared pointer.
		typedef std::basic_string<sf::Uint32>  u32string; //!< UTF-32 string.

		/** Create list box.
		 * @param num_entries Entries.
		 * @param width Maximum displayed entry width.
		 * @return ListBox.
		 */
		static Ptr Create( std::size_t num_entries = 1, float width = 40.f );

    /** Get entries.
		 * @return std::deque of UTF-32 entry strings.
		 */
		std::deque<u32string> const& GetEntries() const { return m_entries; }

		/** Get total number of entries.
		 * @return Total number of entries.
		 */
		std::size_t GetNumEntries() const { return m_entries.size(); }

		/** Get maximum number of displayed entries.
		 * @return Maximum number of displayed entries.
		 */
		std::size_t GetMaxDisplayedEntries() const { return m_num_entries; }

    /** Get displayed entry string by index.
		 * @param index Entry index.
		 * @return Displayed entry string.
		 */
		u32string GetDisplayedEntry( std::size_t index );

		/** Get entry string by index.
		 * @param index Entry index.
		 * @return Entry string.
		 */
		u32string GetEntry( std::size_t index );

		/** Add entry.
		 * @param entry Entry string.
		 */
		void AddEntry( const sf::String& entry );

		/** Remove entry by index.
		 * @param index Entry index.
		 */
		void RemoveEntry( std::size_t index );

		/** Check if entry at displayed index is under mouse cursor.
		 * @param index Displayed entry index.
		 * @return true when item is under mouse cursor.
		 */
		bool IsHovered( std::size_t index ) const;

		/** Check if entry at displayed index is selected.
		 * @param index Displayed entry index.
		 * @return true when item is selected.
		 */
		bool IsSelected( std::size_t index ) const;

    /** Get index of entry currently under mouse cursor.
		 * @return Index of entry currently under mouse cursor.
		 */
    std::size_t GetHovered() const { return m_hover_index; }

		/** Get index of currently selected entry.
		 * @return Index of currently selected entry.
		 */
    std::size_t GetSelected() const { return m_active_index; }

    /** Get index of the first displayed entry.
		 * @return Index of the first displayed entry.
		 */
    std::size_t GetDisplayStart() const { return m_display_start; }

    /** Is up scroll button currently pressed.
		 * @return true if it is pressed.
		 */
    bool IsUpPressed() const { return m_up_pressed; }

    /** Is down scroll button currently pressed.
		 * @return true if it is pressed.
		 */
    bool IsDownPressed() const { return m_down_pressed; }

		Signal<void( Widget::Ptr )>  OnSelectionChanged; //!< Fired when the selection changes.

	protected:
		sf::Drawable* InvalidateImpl( const sf::RenderTarget& target );
		sf::Vector2f GetRequisitionImpl() const;

	private:
		/** Ctor.
		 * @param entries Entries.
		 * @param width Maximum displayed entry width.
		 */
		ListBox( std::size_t num_entries = 1, float width = 40.f );

		void HandleStateChange( Widget::Ptr widget, State oldstate );
		void HandleMouseMove( Widget::Ptr widget, int x, int y );
		void HandleMouseLeave( Widget::Ptr widget, int x, int y );
		bool HandleMouseButtonPress( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		bool HandleMouseButtonRelease( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleExpose( Widget::Ptr widget, sf::RenderTarget& target );
		void HandleFocusChange( Widget::Ptr widget );

    std::size_t m_num_entries;
    float m_width;
		std::deque<u32string> m_entries;
		std::size_t m_display_start;

		int m_active_index;
		int m_hover_index;

		bool m_up_pressed;
		bool m_down_pressed;
		bool m_sliding;
		sf::Clock m_scroll_timer;
		int m_slide_last_y;
};

}

#endif // SFGUI_LISTBOX_HPP
