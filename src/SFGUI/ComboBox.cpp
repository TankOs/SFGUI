#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Renderer.hpp>

#include <limits>

namespace sfg {

// Signals.
Signal::SignalID ComboBox::OnSelect = 0;
Signal::SignalID ComboBox::OnOpen = 0;

const ComboBox::IndexType ComboBox::NONE = std::numeric_limits<ComboBox::IndexType>::max();
static const sf::String EMPTY = "";

ComboBox::ComboBox() :
	Bin(),
	m_active( false ),
	m_active_item( NONE ),
	m_highlighted_item( NONE ),
	m_start_entry( 0 )
{
}

ComboBox::~ComboBox() {
}

ComboBox::Ptr ComboBox::Create() {
	ComboBox::Ptr ptr( new ComboBox );
	return ptr;
}

RenderQueue* ComboBox::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateComboBoxDrawable( DynamicPointerCast<const ComboBox>( shared_from_this() ) );
}

ComboBox::IndexType ComboBox::GetSelectedItem() const {
	return m_active_item;
}

ComboBox::IndexType ComboBox::GetHighlightedItem() const {
	return m_highlighted_item;
}

void ComboBox::SelectItem( IndexType index ) {
	if( index >= m_entries.size() ) {
		return;
	}

	m_active_item = index;
	Invalidate();
}

void ComboBox::AppendItem( const sf::String& text ) {
	m_entries.push_back( text );

	if( IsMouseInWidget() ) {
		SetState( PRELIGHT );
	}
	else {
		SetState( NORMAL );
	}

	RequestResize();
}

void ComboBox::InsertItem( IndexType index, const sf::String& text ) {
	m_entries.insert( m_entries.begin() + index, text );

	if( m_active_item != NONE && m_active_item >= index ) {
		++m_active_item;
	}

	if( IsMouseInWidget() ) {
		SetState( PRELIGHT );
	}
	else {
		SetState( NORMAL );
	}

	RequestResize();
}

void ComboBox::PrependItem( const sf::String& text ) {
	m_entries.insert( m_entries.begin(), text );

	if( m_active_item != NONE ) {
		++m_active_item;
	}

	if( IsMouseInWidget() ) {
		SetState( PRELIGHT );
	}
	else {
		SetState( NORMAL );
	}

	RequestResize();
}

void ComboBox::ChangeItem( IndexType index, const sf::String& text ) {
	if( index >= m_entries.size() ) {
		return;
	}

	m_entries[index] = text;

	if( IsMouseInWidget() ) {
		SetState( PRELIGHT );
	}
	else {
		SetState( NORMAL );
	}

	RequestResize();
}

void ComboBox::RemoveItem( IndexType index ) {
	if( index >= m_entries.size() ) {
		return;
	}

	m_entries.erase( m_entries.begin() + index );

	// Make sure active item index keeps valid.
	if( m_active_item != NONE ) {
		if( m_active_item == index ) {
			m_active_item = NONE;
		}
		else if( m_active_item > index ) {
			m_active_item = m_entries.size() == 0 ? NONE : m_active_item - 1;
		}
	}

	if( IsMouseInWidget() ) {
		SetState( PRELIGHT );
	}
	else {
		SetState( NORMAL );
	}

	RequestResize();
}

const sf::String& ComboBox::GetSelectedText() const {
	if( m_active_item == NONE ) {
		return EMPTY;
	}

	return m_entries[m_active_item];
}

ComboBox::IndexType ComboBox::GetItemCount() const {
	return m_entries.size();
}

const sf::String& ComboBox::GetItem( IndexType index ) const {
	if( index >= m_entries.size() ) {
		return EMPTY;
	}

	return m_entries[index];
}

bool ComboBox::IsPoppedUp() const {
	return m_active;
}

void ComboBox::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( GetState() == NORMAL ) {
		SetState( PRELIGHT );
	}
}

void ComboBox::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( GetState() == PRELIGHT ) {
		SetState( NORMAL );
	}
}

void ComboBox::HandleMouseMoveEvent( int x, int y ) {
	if( m_active ) {
		if( m_scrollbar ) {
			sf::Event event;

			event.type = sf::Event::MouseMoved;
			event.mouseMove.x = x - static_cast<int>( GetAllocation().left );
			event.mouseMove.y = y - static_cast<int>( GetAllocation().top );

			ReleaseModal();
			m_scrollbar->SetActiveWidget();
			m_scrollbar->HandleEvent( event );
			SetActiveWidget();
			GrabModal();

			sf::FloatRect scrollbar_allocation = m_scrollbar->GetAllocation();
			scrollbar_allocation.left += GetAllocation().left;
			scrollbar_allocation.top += GetAllocation().top;

			if( scrollbar_allocation.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				m_highlighted_item = NONE;
				Invalidate();

				return;
			}
		}

		if( ( x > GetAllocation().left ) && ( x < GetAllocation().left + GetAllocation().width ) ) {
			float padding( Context::Get().GetEngine().GetProperty<float>( "ItemPadding", shared_from_this() ) );
			const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
			unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
			const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

			IndexType line_y = y;
			line_y -= static_cast<int>( GetAllocation().top + GetAllocation().height + padding );
			line_y /= static_cast<int>( Context::Get().GetEngine().GetFontLineHeight( font, font_size ) + 2 * padding );

			if( line_y < GetItemCount() ) {
				if( line_y != m_highlighted_item ) {
					Invalidate();
					m_highlighted_item = line_y + GetStartItemIndex();
				}
			}
			else {
				if( m_highlighted_item != NONE ) {
					m_highlighted_item = NONE;
					Invalidate();
				}
			}
		}
		else {
			if( m_highlighted_item != NONE ) {
				m_highlighted_item = NONE;
				Invalidate();
			}
		}
	}
}

void ComboBox::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) {
	if( GetState() == ACTIVE ) {
		if( m_scrollbar ) {
			sf::Event event;

			event.type = press ? sf::Event::MouseButtonPressed : sf::Event::MouseButtonReleased;
			event.mouseButton.button = button;
			event.mouseButton.x = x - static_cast<int>( GetAllocation().left );
			event.mouseButton.y = y - static_cast<int>( GetAllocation().top );

			ReleaseModal();
			m_scrollbar->SetActiveWidget();
			m_scrollbar->HandleEvent( event );
			SetActiveWidget();
			GrabModal();

			sf::FloatRect scrollbar_allocation = m_scrollbar->GetAllocation();
			scrollbar_allocation.left += GetAllocation().left;
			scrollbar_allocation.top += GetAllocation().top;

			if( scrollbar_allocation.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				return;
			}
		}

		if( !press || ( button != sf::Mouse::Left ) ) {
			return;
		}

		m_active = false;

		if( ( m_highlighted_item != NONE ) && ( m_active_item != m_highlighted_item ) ) {
			m_active_item = m_highlighted_item;
			GetSignals().Emit( OnSelect );
		}

		m_highlighted_item = NONE;

		if( IsMouseInWidget() ) {
			SetState( PRELIGHT );
		}
		else {
			SetState( NORMAL );
		}

		Invalidate();

		return;
	}

	if( press && ( button == sf::Mouse::Left ) && IsMouseInWidget() ) {
		m_active = true;
		m_highlighted_item = NONE;

		SetState( ACTIVE );

		GetSignals().Emit( OnOpen );

		Invalidate();
	}
}

sf::Vector2f ComboBox::CalculateRequisition() {
	float padding( Context::Get().GetEngine().GetProperty<float>( "ItemPadding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	// Determine highest needed width of all items.
	sf::Vector2f metrics( 0.f, 0.f );
	for ( IndexType item = 0; item < GetItemCount(); ++item ) {
		metrics.x = std::max( metrics.x, Context::Get().GetEngine().GetTextMetrics( m_entries.at( item ), font, font_size ).x );
	}

	metrics.y = Context::Get().GetEngine().GetFontLineHeight( font, font_size );

	// This is needed for the arrow.
	metrics.x += metrics.y;

	sf::Vector2f requisition(
		metrics.x + 3 * padding,
		metrics.y + 2 * padding
	);

	return requisition;
}

const std::string& ComboBox::GetName() const {
	static const std::string name( "ComboBox" );
	return name;
}

void ComboBox::HandleStateChange( State old_state ) {
	Bin::HandleStateChange( old_state );

	if( GetState() == ACTIVE ) {
		SetZOrder( 1 );

		GrabModal();

		m_start_entry = 0;

		float padding( Context::Get().GetEngine().GetProperty<float>( "ItemPadding", shared_from_this() ) );
		const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
		unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
		const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
		const float line_height( Context::Get().GetEngine().GetFontLineHeight( font, font_size ) );

		if( ( GetDisplayedItems() > 2 ) && ( GetDisplayedItems() < GetItemCount() ) ) {
			float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );


			const sf::Vector2f item_size(
				GetAllocation().width - 2 * border_width,
				line_height + 2 * padding
			);

			m_scrollbar = Scrollbar::Create( Scrollbar::VERTICAL );

			float offset = ( GetState() == ACTIVE ? border_width : 0.f ) + GetAllocation().width - padding - line_height;

			m_scrollbar->SetPosition( sf::Vector2f( offset, GetAllocation().height + border_width ) );
			m_scrollbar->SetRequisition( sf::Vector2f( GetAllocation().width - offset, static_cast<float>( GetDisplayedItems() ) * item_size.y - 2.f * border_width ) );

			m_scrollbar->GetAdjustment()->SetPageSize( static_cast<float>( GetDisplayedItems() ) );
			m_scrollbar->GetAdjustment()->SetLower( 0.f );
			m_scrollbar->GetAdjustment()->SetUpper( static_cast<float>( GetItemCount() ) );
			m_scrollbar->GetAdjustment()->SetMinorStep( 1.f );
			m_scrollbar->GetAdjustment()->SetMajorStep( 1.f );

			m_scrollbar->GetAdjustment()->GetSignal( Adjustment::OnChange ).Connect( &ComboBox::ChangeStartEntry, this );

			m_scrollbar->SetZOrder( 2 );

			Add( m_scrollbar );
		}
	}
	else {
		SetZOrder( 0 );

		if( m_scrollbar ) {
			Remove( m_scrollbar );

			m_scrollbar.reset();
		}
	}
}

ComboBox::IndexType ComboBox::GetDisplayedItems() const {
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float padding( Context::Get().GetEngine().GetProperty<float>( "ItemPadding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	const float line_height( Context::Get().GetEngine().GetFontLineHeight( font, font_size ) );

	const sf::Vector2f item_size(
		GetAllocation().width - 2 * border_width,
		line_height + 2 * padding
	);

	float available_space = static_cast<float>( Renderer::Get().GetWindowSize().y ) - ( GetAbsolutePosition().y + item_size.y );

	IndexType num_displayed_entries = static_cast<IndexType>( available_space / item_size.y );

	num_displayed_entries = ( GetItemCount() < num_displayed_entries ? GetItemCount() : num_displayed_entries );

	return num_displayed_entries;
}

ComboBox::IndexType ComboBox::GetStartItemIndex() const {
	return m_start_entry;
}

void ComboBox::HandleUpdate( float seconds ) {
	Bin::HandleUpdate( seconds );

	if( GetState() != ACTIVE ) {
		if( IsModal() ) {
			ReleaseModal();
		}
	}
}

void ComboBox::ChangeStartEntry() {
	if( m_scrollbar ) {
		m_start_entry = static_cast<IndexType>( m_scrollbar->GetAdjustment()->GetValue() + .5f );
	}

	Invalidate();
}

}
