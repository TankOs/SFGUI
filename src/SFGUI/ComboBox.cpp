#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <limits>

namespace sfg {

// Signals.
Signal::SignalID ComboBox::OnSelect = 0;
Signal::SignalID ComboBox::OnOpen = 0;

const ComboBox::IndexType ComboBox::NONE = std::numeric_limits<ComboBox::IndexType>::max();
static const sf::String EMPTY = "";

ComboBox::ComboBox() :
	Bin(),
	m_queue( 0 ),
	m_active( false ),
	m_active_item( NONE ),
	m_highlighted_item( NONE )
{
}

ComboBox::~ComboBox() {
}

ComboBox::Ptr ComboBox::Create() {
	ComboBox::Ptr ptr( new ComboBox );
	return ptr;
}

RenderQueue* ComboBox::InvalidateImpl() const {
	RenderQueue* queue = Context::Get().GetEngine().CreateComboBoxDrawable( DynamicPointerCast<const ComboBox>( shared_from_this() ) );

	m_queue = queue;

	if( GetState() == ACTIVE ) {
		// Set Z Layer to 1, above all "normal" widgets.
		m_queue->SetZOrder( 1 );
	}

	return queue;
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
	RequestResize();
}

void ComboBox::InsertItem( IndexType index, const sf::String& text ) {
	m_entries.insert( m_entries.begin() + index, text );

	if( m_active_item != NONE && m_active_item >= index ) {
		++m_active_item;
	}

	RequestResize();
}

void ComboBox::PrependItem( const sf::String& text ) {
	m_entries.insert( m_entries.begin(), text );

	if( m_active_item != NONE ) {
		++m_active_item;
	}

	RequestResize();
}

void ComboBox::ChangeItem( IndexType index, const sf::String& text ) {
	if( index >= m_entries.size() ) {
		return;
	}

	m_entries[index] = text;
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
					m_highlighted_item = line_y;
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

void ComboBox::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( !press || ( button != sf::Mouse::Left ) ) {
		return;
	}

	if( GetState() == ACTIVE ) {
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

	if( IsMouseInWidget() ) {
		m_active = true;
		m_highlighted_item = NONE;

		SetState( ACTIVE );

		GetSignals().Emit( OnOpen );

		if( m_queue ) {
			// Set Z Layer to 1, above all "normal" widgets.
			m_queue->SetZOrder( 1 );
		}

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

}
