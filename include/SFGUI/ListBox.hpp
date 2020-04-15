#pragma once

#include <SFGUI/Container.hpp>
#include <SFGUI/Scrollbar.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/String.hpp>

#include <initializer_list>
#include <memory>
#include <set>
#include <vector>

namespace sfg {

class SFGUI_API ListBox : public Container {
	public:

		struct Item {
			sf::String text;
			sf::Image image;
		};

		typedef std::shared_ptr<ListBox> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const ListBox> PtrConst; //!< Shared pointer.
		typedef int IndexType;

		static const IndexType NONE;

		enum class SelectionMode : char {
			NO_SELECTION,
			SINGLE_SELECTION,
			MULTI_SELECTION,
			DEFAULT = SINGLE_SELECTION
		};

		enum class ScrollbarPolicy : char {
			VERTICAL_ALWAYS,
			VERTICAL_AUTOMATIC,
			VERTICAL_NEVER,
			DEFAULT = VERTICAL_AUTOMATIC
		};

		enum class ItemTextPolicy : char {
			RESIZE_LISTBOX,
			SHRINK,
			DEFAULT = RESIZE_LISTBOX
		};

		/** Create listbox.
		 * @return ListBox.
		 */
		static Ptr Create(  );

		const std::string& GetName() const override;

		void AppendItem( const sf::String& str, const sf::Image& image = sf::Image() );
		void InsertItem( IndexType index, const sf::String& str, const sf::Image& image = sf::Image() );
		void PrependItem( const sf::String& str, const sf::Image& image = sf::Image() );
		void ChangeItemText( IndexType index, const sf::String& str );
		void ChangeItemImage( IndexType index, const sf::Image& image );
		void RemoveItem( IndexType index );
		void Clear();

		IndexType GetItemsCount() const;
		const sf::String& GetItemText( IndexType index ) const;
		const sf::String& GetDisplayedItemText( IndexType index ) const;
		const sf::Image& GetItemImage( IndexType index ) const;

		IndexType GetHighlightedItem() const;

		void SetSelection( IndexType index );
		void SetSelection( std::initializer_list<IndexType> indices );
		void AppendToSelection( IndexType index );
		void RemoveFromSelection( IndexType index );
		void ClearSelection();

		bool IsItemSelected( IndexType index ) const;
        IndexType GetSelectedItemsCount() const;
		IndexType GetSelectedItemIndex( IndexType index = 0 ) const;
		const sf::String& GetSelectedItemText( IndexType index = 0 ) const;

		IndexType GetFirstDisplayedItemIndex() const;
		IndexType GetDisplayedItemsCount() const;
		IndexType GetMaxDisplayedItemsCount() const;

		SelectionMode GetSelectionMode() const;
		void SetSelectionMode( SelectionMode mode );

		ScrollbarPolicy GetScrollbarPolicy() const;
		void SetScrollbarPolicy( ScrollbarPolicy policy );

		ItemTextPolicy GetItemTextPolicy() const;
		void SetItemTextPolicy( ItemTextPolicy policy );

		sf::Vector2f GetImagesSize() const;
		void SetImagesSize(sf::Vector2f size);

		float GetItemHeight() const;

        // Signals.
		static Signal::SignalID OnSelect; //!< Fired when an entry is selected.

	protected:
		/** Ctor.
		 */
		ListBox();

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
        void HandleMouseEnter( int x, int y ) override;
		void HandleMouseLeave( int x, int y ) override;
		void HandleMouseMoveEvent( int x, int y ) override;
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void HandleSizeChange() override;
		bool HandleAdd( Widget::Ptr ) override;
		void HandleRemove( Widget::Ptr ) override;

		IndexType GetItemAt( float y ) const;

		bool IsScrollbarVisible() const;

		void UpdateDisplayedItems();
		void UpdateScrollbarAdjustment();
		void UpdateScrollbarAllocation();

		void UpdateDisplayedItemsText();

		void OnScrollbarChanged();

		std::vector<Item> m_items;

		SelectionMode m_selection_mode;
		std::set<IndexType> m_selected_items;

		IndexType m_highlighted_item;

		IndexType m_first_displayed_item;
		IndexType m_max_displayed_items_count;

		Scrollbar::Ptr m_vertical_scrollbar;
		ScrollbarPolicy m_scrollbar_policy;

		ItemTextPolicy m_item_text_policy;
		std::vector<sf::String> m_displayed_items_texts;

		sf::Vector2f m_images_size;
};

}
