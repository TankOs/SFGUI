#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System/String.hpp>
#include <vector>

namespace sfg {

/** Combobox.
 */
class SFGUI_API ComboBox : public Bin {
	public:
		typedef SharedPtr<ComboBox> Ptr; ///< Shared pointer.
		typedef SharedPtr<const ComboBox> PtrConst; ///< Shared pointer to const.
		typedef std::size_t IndexType; ///< Type for item indices.

		static const IndexType NONE; ///< Value for specifying an invalid index/no item selected.

		/** Create combo box.
		 * @return Combo box.
		 */
		static Ptr Create();

		/** Dtor.
		 */
		virtual ~ComboBox();

		virtual const std::string& GetName() const;

		/** Get selected item.
		 * @return Active item or NONE if none selected.
		 */
		IndexType GetSelectedItem() const;

		/** Get highlighted item.
		 * @return Highlighted item or NONE if none highlighted.
		 */
		IndexType GetHighlightedItem() const;

		/** Select item.
		 * @param index Item index.
		 */
		void SelectItem( IndexType index );

		/** Append item.
		 * @param text Item text.
		 */
		void AppendItem( const sf::String& text );

		/** Insert item.
		 * @param index Item index.
		 * @param text Item text.
		 */
		void InsertItem( IndexType index, const sf::String& text );

		/** Prepend item.
		 * @param text Item text.
		 */
		void PrependItem( const sf::String& text );

		/** Change item.
		 * @param index Item index.
		 * @param text Item text.
		 */
		void ChangeItem( IndexType index, const sf::String& text );

		/** Remove item.
		 * @param index Item index.
		 */
		void RemoveItem( IndexType index );

		/** Get text of selected item.
		 * @return Text of selected item or empty if none selected.
		 */
		const sf::String& GetSelectedText() const;

		/** Get item count.
		 * @return Item count.
		 */
		IndexType GetItemCount() const;

		/** Get number of items that are displayed at once when popup is opened.
		 * @return Number of items that are displayed at once when popup is opened.
		 */
		IndexType GetDisplayedItems() const;

		/** Get index of the first item to be displayed in the popup.
		 * @return Get index of the first item to be displayed in the popup.
		 */
		IndexType GetStartItemIndex() const;

		/** Get text of specific item.
		 * @param index Item index.
		 * @return Item text or empty if index is invalid.
		 */
		const sf::String& GetItem( IndexType index ) const;

		/** Is the popup being shown?
		 * @return true if the popup is being shown.
		 */
		bool IsPoppedUp() const;

		// Signals.
		static Signal::SignalID OnSelect; //!< Fired when an entry is selected.
		static Signal::SignalID OnOpen; //!< Fired when the popup is opened.

	protected:
		/** Ctor.
		 */
		ComboBox();

		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		void HandleMouseEnter( int x, int y );
		void HandleMouseLeave( int x, int y );
		virtual void HandleMouseMoveEvent( int x, int y );
		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		virtual void HandleStateChange( State old_state );
		virtual void HandleUpdate( float seconds );
		void ChangeStartEntry();

		Scrollbar::Ptr m_scrollbar;

		bool m_active;
		IndexType m_active_item;
		IndexType m_highlighted_item;
		std::vector<sf::String> m_entries;
		IndexType m_start_entry;
};

}
