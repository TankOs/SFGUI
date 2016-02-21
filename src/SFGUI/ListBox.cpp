#include <SFGUI/ListBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Widgets.hpp>

#include <algorithm>

#include <SFML/Graphics.hpp>

namespace sfg {

const ListBox::IndexType ListBox::NONE = -1;
static const sf::String EMPTY = "";

Signal::SignalID ListBox::OnSelect = 0;

ListBox::Ptr ListBox::Create(  ) {
	auto ptr = Ptr( new ListBox );
	static_cast<Container::Ptr>( ptr )->Add( ptr->m_vertical_scrollbar );
	return ptr;
}

ListBox::ListBox() :
	Container(),
	m_items(),
	m_selection_mode(SelectionMode::DEFAULT),
	m_selected_items(),
	m_highlighted_item(NONE),
	m_first_displayed_item(0),
	m_max_displayed_items_count(0),
	m_vertical_scrollbar(nullptr),
	m_scrollbar_policy(ScrollbarPolicy::DEFAULT),
	m_item_text_policy(ItemTextPolicy::DEFAULT),
	m_displayed_items_texts()
{
	m_vertical_scrollbar = Scrollbar::Create(Scrollbar::Orientation::VERTICAL);
	m_vertical_scrollbar->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&ListBox::OnScrollbarChanged, this));
}

std::unique_ptr<sfg::RenderQueue> ListBox::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateListBoxDrawable( std::dynamic_pointer_cast<const ListBox>( shared_from_this() ) );
}

sf::Vector2f ListBox::CalculateRequisition() {
	auto text_padding = Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() );
	auto border_width = Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() );
	const auto& font_name = Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() );
	const auto& font = Context::Get().GetEngine().GetResourceManager().GetFont( font_name );
	auto font_size = Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() );
	auto dots_width = Context::Get().GetEngine().GetTextStringMetrics("...", *font, font_size).x;

	// Calculate the max width of items
	float items_max_width = 0.f;
	for( const sf::String& item : m_items ) {
		items_max_width = std::max(items_max_width, Context::Get().GetEngine().GetTextStringMetrics(item, *font, font_size).x);
	}

	return sf::Vector2f(
		border_width * 2 + text_padding * 2
		 	+ ( m_item_text_policy == ItemTextPolicy::RESIZE_LISTBOX ? items_max_width : dots_width )
			+ ( IsScrollbarVisible() ? m_vertical_scrollbar->GetRequisition().x : 0 ),
		std::max(m_vertical_scrollbar->GetRequisition().y, 50.f)
	);
}

const std::string& ListBox::GetName() const {
	static const std::string name( "ListBox" );
	return name;
}

void ListBox::AppendItem( const sf::String& str ) {
    m_items.push_back( str );

	UpdateDisplayedItems();
    RequestResize();
	HandleSizeChange();
    Invalidate();
}

void ListBox::InsertItem( IndexType index, const sf::String& str ) {
	m_items.insert( m_items.begin() + index, str );

	// Update next selected indexes (decrement them).
	std::set<IndexType> new_selected_items;
	std::transform(
		m_selected_items.cbegin(),
		m_selected_items.cend(),
		std::inserter(new_selected_items, new_selected_items.end()),
		[&](IndexType i){
			if( i < index )
			 	return i;
			else
				return ++i;
		}
	);
	m_selected_items = std::move(new_selected_items);

	UpdateDisplayedItems();
    RequestResize();
	HandleSizeChange();
    Invalidate();
}

void ListBox::PrependItem( const sf::String& str ) {
    m_items.insert( m_items.begin(), str );

	// Update selected items indexes.
	std::set<IndexType> new_selected_items;
	std::transform(
		m_selected_items.cbegin(),
		m_selected_items.cend(),
		std::inserter(new_selected_items, new_selected_items.end()),
		[&](IndexType i){
			return ++i;
		}
	);
	m_selected_items = std::move(new_selected_items);

	UpdateDisplayedItems();
	RequestResize();
	HandleSizeChange();
    Invalidate();
}

void ListBox::ChangeItem( IndexType index, const sf::String& str ) {
	if( index >= static_cast<IndexType>( m_items.size() ) || index < 0 ) {
		return;
	}

	m_items[ static_cast<std::size_t>( index ) ] = str;

	UpdateDisplayedItems();
	Invalidate();
}

void ListBox::RemoveItem( IndexType index ) {
	if( index >= static_cast<IndexType>( m_items.size() ) || index < 0 ) {
		return;
	}

    m_items.erase( m_items.begin() + index );

	// Remove it from the selected indexes.
	m_selected_items.erase( index );

	// Update next selected indexes (decrement them).
	std::set<IndexType> new_selected_items;
	std::transform(
		m_selected_items.cbegin(),
		m_selected_items.cend(),
		std::inserter(new_selected_items, new_selected_items.end()),
		[&](IndexType i){
			if( i > index )
			 	return --i;
			else
				return i;
		}
	);
	m_selected_items = std::move(new_selected_items);

	UpdateDisplayedItems();
	RequestResize();
	HandleSizeChange();
    Invalidate();
}

void ListBox::Clear() {
    m_items.clear();
	m_selected_items.clear();

	UpdateDisplayedItems();
	RequestResize();
	HandleSizeChange();
	Invalidate();
}

ListBox::IndexType ListBox::GetItemsCount() const {
	return static_cast<IndexType>( m_items.size() );
}

const sf::String& ListBox::GetItemText( IndexType index ) const {
	if( index >= static_cast<IndexType>( m_items.size() ) || index < 0 ) {
		return EMPTY;
	}

	return m_items[ static_cast<std::size_t>( index )];
}

const sf::String& ListBox::GetDisplayedItemText( IndexType index ) const {
	if( index >= static_cast<IndexType>( m_items.size() ) || index < 0 ) {
		return EMPTY;
	}

	if(m_item_text_policy == ItemTextPolicy::RESIZE_LISTBOX) {
		return GetItemText( index );
	} else {
		return m_displayed_items_texts[ static_cast<std::size_t>( index ) ];
	}
}

ListBox::IndexType ListBox::GetHighlightedItem() const {
	return m_highlighted_item;
}

void ListBox::SetSelection( IndexType index ) {
	if( m_selection_mode == SelectionMode::NO_SELECTION ) {
		return;
	}

	m_selected_items.clear();
	if( index != NONE ) {
		m_selected_items.insert( index );
	}

	Invalidate();
}

void ListBox::SetSelection( std::initializer_list<IndexType> indices ) {
	if( m_selection_mode == SelectionMode::MULTI_SELECTION ) {
		m_selected_items = std::set<IndexType>( indices.begin(), indices.end() );

		Invalidate();
	} else if( m_selection_mode == SelectionMode::SINGLE_SELECTION ) {
		if( indices.size() > 0) {
			SetSelection( *( indices.begin() ) );
		} else {
			SetSelection( NONE );
		}

		Invalidate();
	}
}

void ListBox::AppendToSelection( IndexType index ) {
	if( m_selection_mode == SelectionMode::NO_SELECTION ) {
		return;
	}

	if( m_selected_items.size() == 0 || m_selection_mode == SelectionMode::MULTI_SELECTION ) {
		m_selected_items.insert( index );
	}

	Invalidate();
}

void ListBox::RemoveFromSelection( IndexType index ) {
	m_selected_items.erase( index );

	Invalidate();
}

void ListBox::ClearSelection() {
	m_selected_items.clear();

	Invalidate();
}

bool ListBox::IsItemSelected(IndexType index) const {
	return m_selected_items.find( index ) != m_selected_items.end();
}

ListBox::IndexType ListBox::GetSelectedItemsCount() const {
    return static_cast<IndexType>( m_selected_items.size() );
}

ListBox::IndexType ListBox::GetSelectedItemIndex( IndexType index ) const {
	if( index >= static_cast<IndexType>( m_selected_items.size() ) || index < 0 ) {
		return NONE;
	}

	auto it = m_selected_items.cbegin();
	std::advance( it, index );
	return *it;
}

const sf::String& ListBox::GetSelectedItemText( IndexType index ) const {
	return m_items[ static_cast<std::size_t>( GetSelectedItemIndex( index ) ) ];
}

ListBox::IndexType ListBox::GetFirstDisplayedItemIndex() const {
	return m_first_displayed_item;
}

ListBox::IndexType ListBox::GetDisplayedItemsCount() const {
	return std::min(m_max_displayed_items_count, static_cast<IndexType>( m_items.size() ) - m_first_displayed_item);
}

ListBox::IndexType ListBox::GetMaxDisplayedItemsCount() const {
	return m_max_displayed_items_count;
}

ListBox::SelectionMode ListBox::GetSelectionMode() const {
	return m_selection_mode;
}

void ListBox::SetSelectionMode( SelectionMode mode ) {
	m_selection_mode = mode;

	if( m_selection_mode == SelectionMode::NO_SELECTION ) {
		m_selected_items.clear();
		Invalidate();
	} else if( m_selection_mode == SelectionMode::SINGLE_SELECTION && m_selected_items.size() > 1 ) {
		auto it = m_selected_items.begin();
		std::advance( it, 1 );
		m_selected_items.erase( it, m_selected_items.end() );
	}
}

ListBox::ScrollbarPolicy ListBox::GetScrollbarPolicy() const {
	return m_scrollbar_policy;
}

void ListBox::SetScrollbarPolicy( ScrollbarPolicy policy ) {
	m_scrollbar_policy = policy;

	RequestResize();
	Invalidate();
}

ListBox::ItemTextPolicy ListBox::GetItemTextPolicy() const {
	return m_item_text_policy;
}

void ListBox::SetItemTextPolicy( ItemTextPolicy policy ) {
	m_item_text_policy = policy;

	RequestResize();
	Invalidate();
}

void ListBox::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( !HasFocus() ) {
		SetState( State::PRELIGHT );
	}
}

void ListBox::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( !HasFocus() ) {
		SetState( State::NORMAL );
	}
}

void ListBox::HandleMouseMoveEvent( int x, int y ) {
	if( ( x == std::numeric_limits<int>::min() ) || ( y == std::numeric_limits<int>::min() ) ) {
		return;
	}

	float realX = static_cast<float>( x ) - GetAllocation().left;
	float realY = static_cast<float>( y ) - GetAllocation().top;

	if( IsMouseInWidget() && ( !IsScrollbarVisible() || ( IsScrollbarVisible() && realX < m_vertical_scrollbar->GetAllocation().left ) ) ) {
		//Highlight the item under the mouse
		IndexType hovered_item_index = GetItemAt( realY );
		m_highlighted_item = hovered_item_index;

		Invalidate();
	} else if( m_highlighted_item != NONE ) {
		m_highlighted_item = NONE;

		Invalidate();
	}
}

void ListBox::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) {
	if( ( x == std::numeric_limits<int>::min() ) || ( y == std::numeric_limits<int>::min() ) ) {
		return;
	}

	float realX = static_cast<float>( x ) - GetAllocation().left;
	float realY = static_cast<float>( y ) - GetAllocation().top;

	if( IsMouseInWidget() ) {
		if( button == sf::Mouse::Left && press ) {
			if( !IsScrollbarVisible() || ( IsScrollbarVisible() && realX < m_vertical_scrollbar->GetAllocation().left ) ) {
				if( m_selection_mode != SelectionMode::NO_SELECTION ) {
					// Find out and select which item has been clicked.
					IndexType clicked_item_index = GetItemAt( realY );

					if( clicked_item_index != NONE ) { // Only change the selection if the user has clicked on an item.
						// Determine whether the clicked item is new to the selection.
						bool selection_changed = false;
						if( m_selection_mode == SelectionMode::SINGLE_SELECTION || ( !sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) && !sf::Keyboard::isKeyPressed( sf::Keyboard::RControl ) ) ) {
							// In SINGLE_SELECTION mode or when Ctrl is not pressed, if the clicked item was not in the selected items list, the selection has changed.
							selection_changed = std::find( m_selected_items.begin(), m_selected_items.end(), clicked_item_index ) == m_selected_items.end();

							// Clear the selection and add the item to the selection.
							m_selected_items.clear();
							m_selected_items.insert( clicked_item_index );
						} else {
							// In MULTI_SELECTION and when Ctrl is pressed, the selection has changed (as if the clicked item is already selected, it is removed from the selection).
							selection_changed = true;

							// Add or remove the clicked item, depending on if it was in the selection or not.
							if(m_selected_items.find( clicked_item_index ) == m_selected_items.end() )
								m_selected_items.insert( clicked_item_index );
							else
								m_selected_items.erase( clicked_item_index );
						}

						if( selection_changed ) // Only emit the OnSelect signal if the selection changed.
							GetSignals().Emit( OnSelect );
					}
				}

				GrabFocus();
				Invalidate();
			}
		}
	}
}

ListBox::IndexType ListBox::GetItemAt( float y ) const {
	auto text_padding = Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() );
	auto border_width = Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() );
	const auto& font_name = Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() );
	const auto& font = Context::Get().GetEngine().GetResourceManager().GetFont( font_name );
	auto font_size = Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() );
	auto line_height = Context::Get().GetEngine().GetFontLineHeight( *font, font_size );

	IndexType item_index = 0;
	while(y > border_width + static_cast<float>( item_index ) * ( line_height + text_padding * 2 ) ) {
		++item_index;
	}

	if( item_index == 0 )
		return 0 + m_first_displayed_item;
	else if( item_index > GetDisplayedItemsCount() )
		return NONE;
	else
		return item_index - 1 + m_first_displayed_item;
}

bool ListBox::IsScrollbarVisible() const {
	return ( m_scrollbar_policy == ScrollbarPolicy::VERTICAL_ALWAYS || ( m_scrollbar_policy == ScrollbarPolicy::VERTICAL_AUTOMATIC && GetMaxDisplayedItemsCount() < GetItemsCount() ) );
}

void ListBox::HandleSizeChange() {
	UpdateDisplayedItems();
	UpdateScrollbarAdjustment();
	UpdateScrollbarAllocation();

	UpdateDisplayedItemsText();

	Invalidate();
}

bool ListBox::HandleAdd( Widget::Ptr widget ) {
	// The user can't add widgets to the ListBox.

	if( widget == m_vertical_scrollbar && GetChildren().size() == 0 ) { // The scrollbar is an exception (added while creating the widget).
		Container::HandleAdd( widget );
		return true;
	}

	#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: No widgets can be added to a ListBox.\n";
	#endif

	return false;
}

void ListBox::HandleRemove( Widget::Ptr /*widget*/ ){
	std::cerr << "SFGUI warning: No widgets can be removed from a ListBox.\n";
}

void ListBox::UpdateDisplayedItems() {
	auto text_padding = Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() );
	auto border_width = Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() );
	const auto& font_name = Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() );
	const auto& font = Context::Get().GetEngine().GetResourceManager().GetFont( font_name );
	auto font_size = Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() );
	auto line_height = Context::Get().GetEngine().GetFontLineHeight( *font, font_size );

	// Update the displayed items count.
	float items_height = 0.f;
	m_max_displayed_items_count = 0;
	while(items_height < GetAllocation().height - border_width * 2.f + text_padding ) {
		items_height += line_height + text_padding * 2.f;
		++m_max_displayed_items_count;
	}
	if(m_max_displayed_items_count > 0)
		--m_max_displayed_items_count;

	// If there aren't enough items from m_first_displayed_item to
	// m_first_displayed_item + m_max_displayed_items_count, decrement
	// m_first_displayed_item.
	if(m_first_displayed_item + m_max_displayed_items_count > static_cast<IndexType>( m_items.size() ) ) {
		m_first_displayed_item = std::max( static_cast<IndexType>( m_items.size() ) - m_max_displayed_items_count, static_cast<IndexType>( 0 ) );
	}
}

void ListBox::UpdateScrollbarAdjustment() {
	m_vertical_scrollbar->GetAdjustment()->Configure(
		static_cast<float>( m_first_displayed_item ),
		0.f,
		static_cast<float>( m_items.size() ),
		1.f,
		static_cast<float>( m_max_displayed_items_count ),
		static_cast<float>( m_max_displayed_items_count )
	);
}

void ListBox::UpdateScrollbarAllocation() {
	auto border_width = Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() );
	m_vertical_scrollbar->SetAllocation( sf::FloatRect(
		GetAllocation().width - border_width - m_vertical_scrollbar->GetRequisition().x,
		border_width,
		m_vertical_scrollbar->GetRequisition().x,
		GetAllocation().height - border_width * 2.f
	) );

	m_vertical_scrollbar->RequestResize();
	m_vertical_scrollbar->Invalidate();
	m_vertical_scrollbar->Show( IsScrollbarVisible() );

	Invalidate();
}

void ListBox::UpdateDisplayedItemsText() {
	m_displayed_items_texts.clear();
	if(m_item_text_policy == ItemTextPolicy::RESIZE_LISTBOX)
		return;

	auto text_padding = Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() );
	auto border_width = Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() );
	const auto& font_name = Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() );
	const auto& font = Context::Get().GetEngine().GetResourceManager().GetFont( font_name );
	auto font_size = Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() );
	auto dots_width = Context::Get().GetEngine().GetTextStringMetrics("...", *font, font_size).x;

	float max_width = GetAllocation().width - border_width * 2 - text_padding * 2 - ( IsScrollbarVisible() ? m_vertical_scrollbar->GetAllocation().width : 0 );

	for(auto& itemText : m_items) {
		if( Context::Get().GetEngine().GetTextStringMetrics( itemText, *font, font_size ).x < max_width ) {
			// The item's text is fully displayable in the listbox's width.
			m_displayed_items_texts.push_back( itemText );
		} else {
			// We need to shrink the text so that it will fit inside the listbox (and don't forget some width for "...").
			sf::String displayableText;

			while( displayableText.getSize() < itemText.getSize() &&
				Context::Get().GetEngine().GetTextStringMetrics( displayableText, *font, font_size ).x <= max_width - dots_width ) {
				displayableText += itemText[ displayableText.getSize() ];
			}

			if(displayableText.getSize() > 0)
				displayableText.erase( displayableText.getSize() - 1 ); // Removes the last character as it is going outside of the available space.

			displayableText += "...";
			m_displayed_items_texts.push_back( displayableText );
		}
	}

	Invalidate();
}

void ListBox::OnScrollbarChanged() {
	m_first_displayed_item = static_cast<IndexType>( m_vertical_scrollbar->GetAdjustment()->GetValue() );
	UpdateDisplayedItems();

	Invalidate();
}

}
