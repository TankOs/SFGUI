#pragma once

#include <SFGUI/Bin.hpp>

#include <SFML/System/String.hpp>
#include <vector>
#include <memory>

namespace sfg {

class Scrollbar;

/** Combobox.
 */
class SFGUI_API ComboBox : public Bin {
	public:
		typedef std::shared_ptr<ComboBox> Ptr; ///< Shared pointer.
		typedef std::shared_ptr<const ComboBox> PtrConst; ///< Shared pointer to const.
		typedef int IndexType; ///< Type for item indices.
		typedef std::vector<sf::String>::const_iterator ConstIterator; ///< Iterator type

		static const IndexType NONE; ///< Value for specifying an invalid index/no item selected.

		/** Create combo box.
		 * @return Combo box.
		 */
		static Ptr Create();

		const std::string& GetName() const override;

		/** Get selected item.
		 * Indices lie in the range [0, GetItemCount() - 1].
		 * @return Active item or NONE if none selected.
		 */
		IndexType GetSelectedItem() const;

		/** Get highlighted item.
		 * Indices lie in the range [0, GetItemCount() - 1].
		 * @return Highlighted item or NONE if none highlighted.
		 */
		IndexType GetHighlightedItem() const;

		/** Select item.
		 * Indices lie in the range [0, GetItemCount() - 1].
		 * @param index Item index.
		 */
		void SelectItem( IndexType index );

		/** Append item.
		 * @param text Item text.
		 */
		void AppendItem( const sf::String& text );

		/** Insert item.
		 * Indices lie in the range [0, GetItemCount() - 1].
		 * @param index Item index.
		 * @param text Item text.
		 */
		void InsertItem( IndexType index, const sf::String& text );

		/** Prepend item.
		 * @param text Item text.
		 */
		void PrependItem( const sf::String& text );

		/** Change item.
		 * Indices lie in the range [0, GetItemCount() - 1].
		 * @param index Item index.
		 * @param text Item text.
		 */
		void ChangeItem( IndexType index, const sf::String& text );

		/** Remove item.
		 * Indices lie in the range [0, GetItemCount() - 1].
		 * @param index Item index.
		 */
		void RemoveItem( IndexType index );

		/** Clear all items.
		 */
		void Clear();

		/** Get a const iterator pointing to the first item.
		 * This will be equal to End() if the ComboBox contains no items.
		 */
		ConstIterator Begin() const;

		/** Get a const iterator pointing to one past the last item.
		 * This will be equal to Begin() if the ComboBox contains no items.
		 */
		ConstIterator End() const;

		/** Get text of selected item.
		 * @return Text of selected item or empty if none selected.
		 */
		const sf::String& GetSelectedText() const;

		/** Get the number of items in this ComboBox.
		 * @return Item count.
		 */
		IndexType GetItemCount() const;

		/** Get the number of items that are simultaneously displayed in the drop-down.
		 * This takes scrolling into consideration when not all items can be displayed on the screen.
		 * @return Number of items that are simultaneously displayed in the drop-down.
		 */
		IndexType GetDisplayedItemCount() const;

		/** Get index of the first item to be displayed in the drop-down.
		 * @return Get index of the first item to be displayed in the drop-down.
		 */
		IndexType GetDisplayedItemStart() const;

		/** Get text of specific item.
		 * @param index Item index.
		 * @return Item text or empty if index is invalid.
		 */
		const sf::String& GetItem( IndexType index ) const;

		/** Is the drop-down being shown?
		 * @return true if the drop-down is being shown.
		 */
		bool IsDropDownDisplayed() const;

		// Signals.
		static Signal::SignalID OnSelect; //!< Fired when an entry is selected.
		static Signal::SignalID OnOpen; //!< Fired when the drop-down is opened.

	protected:
		ComboBox();

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		void HandleMouseEnter( int x, int y ) override;
		void HandleMouseLeave( int x, int y ) override;
		void HandleMouseMoveEvent( int x, int y ) override;
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void HandleStateChange( State old_state ) override;
		void HandleUpdate( float seconds ) override;
		void ChangeStartEntry();

		std::shared_ptr<Scrollbar> m_scrollbar;

		IndexType m_active_item;
		IndexType m_highlighted_item;
		std::vector<sf::String> m_entries;
		IndexType m_start_entry;
};

/** Get a const iterator pointing to the first item of the ComboBox.
 * This will be equal to end() if the ComboBox contains no items.
 */
ComboBox::ConstIterator begin( const ComboBox& combo_box );

/** Get a const iterator pointing to one past the last item of the ComboBox.
 * This will be equal to begin() if the ComboBox contains no items.
 */
ComboBox::ConstIterator end( const ComboBox& combo_box );

}
