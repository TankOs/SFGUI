#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System/String.hpp>
#include <vector>

namespace sfg {

/** Combobox.
 */
class SFGUI_API ComboBox : public Bin {
	public:
		typedef SharedPtr<ComboBox>  Ptr; //!< Shared pointer.
		typedef SharedPtr<const ComboBox>  PtrConst; //!< Shared pointer.
		
		/** Create combo box.
		 * @return Combo box.
		 */
		static Ptr Create();
		
		/** Dtor.
		 */
		virtual ~ComboBox();
		
		virtual const std::string& GetName() const;
		
		/** Get active item.
		 * @return Active item index.
		 */
		int GetActive() const;
		
		/** Get highlighted item.
		 * @return The highlighted item.
		 */
		int GetHighlighted() const;
		
		/** Set active item.
		 * @param index The new active item's index.
		 */
		void SetActive( int index );
		
		/** Append a new entry.
		 * @param text The text to append.
		 */
		void AppendText( const sf::String& text );
		
		/** Insert a new entry.
		 * @param position The position to insert the text at.
		 * @param text The text to insert.
		 */
		void InsertText( int position, const sf::String& text );

		/** Prepend a new entry.
		 * @param text The text to insert.
		 */
		void PrependText( const sf::String& text );
		
		/** Change an entry.
		 * @param index The index to change.
		 * @param text The new text.
		 */
		void ChangeText( int index, const sf::String& text );
		
		/** Remove an entry.
		 * @param position The index of the entry to remove.
		 */
		void RemoveText( int index );
		
		/** Get the text of the active entry.
		 * @return The text of the active entry.
		 */
		const sf::String& GetActiveText() const;
		
		/** Get the number of entries.
		 * @return The number of entries.
		 */
		std::size_t GetNumberEntries() const;
		
		/** Get the text of a specific entry.
		 * @param index The index of the entry to get the text of.
		 * @return The text of the specified entry.
		 */
		const sf::String& GetEntryText( const int index ) const;
		
		/** Is the popup being shown?
		 * @return True if the popup is being shown.
		 */
		bool IsPoppedUp() const;
		
		Signal OnSelect; //!< Fired when an entry is selected.
		Signal OnOpen; //!< Fired when the popup is opened.
		
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
		
		std::vector<sf::String> m_entries;
		int m_active_index;
		int m_highlighted;
		bool m_active;
};

}
