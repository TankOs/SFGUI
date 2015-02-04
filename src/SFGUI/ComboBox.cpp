#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>

namespace sfg {

// Signals.
Signal::SignalID ComboBox::OnSelect = 0;
Signal::SignalID ComboBox::OnOpen = 0;

const ComboBox::IndexType ComboBox::NONE = -1;
static const sf::String EMPTY = "";

ComboBox::ConstIterator begin( const ComboBox& combo_box ) {
	return combo_box.Begin();
}

ComboBox::ConstIterator end( const ComboBox& combo_box ) {
	return combo_box.End();
}

ComboBox::ComboBox() :
	m_active_item( NONE ),
	m_highlighted_item( NONE ),
	m_start_entry( 0 )
{
}

ComboBox::Ptr ComboBox::Create() {
	return Ptr( new ComboBox );
}

std::unique_ptr<RenderQueue> ComboBox::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateComboBoxDrawable( std::dynamic_pointer_cast<const ComboBox>( shared_from_this() ) );
}

ComboBox::IndexType ComboBox::GetSelectedItem() const {
	return m_active_item;
}

ComboBox::IndexType ComboBox::GetHighlightedItem() const {
	return m_highlighted_item;
}

void ComboBox::SelectItem( IndexType index ) {
	if( index >= static_cast<IndexType>( m_entries.size() ) || index < 0 ) {
		return;
	}

	m_active_item = index;
	Invalidate();
}

void ComboBox::AppendItem( const sf::String& text ) {
	m_entries.push_back( text );

	if( IsMouseInWidget() ) {
		SetState( State::PRELIGHT );
	}
	else {
		SetState( State::NORMAL );
	}

	RequestResize();
}

void ComboBox::InsertItem( IndexType index, const sf::String& text ) {
	m_entries.insert( m_entries.begin() + index, text );

	if( m_active_item != NONE && m_active_item >= index ) {
		++m_active_item;
	}

	if( IsMouseInWidget() ) {
		SetState( State::PRELIGHT );
	}
	else {
		SetState( State::NORMAL );
	}

	RequestResize();
}

void ComboBox::PrependItem( const sf::String& text ) {
	m_entries.insert( m_entries.begin(), text );

	if( m_active_item != NONE ) {
		++m_active_item;
	}

	if( IsMouseInWidget() ) {
		SetState( State::PRELIGHT );
	}
	else {
		SetState( State::NORMAL );
	}

	RequestResize();
}

void ComboBox::ChangeItem( IndexType index, const sf::String& text ) {
	if( index >= static_cast<IndexType>( m_entries.size() ) || index < 0 ) {
		return;
	}

	m_entries[static_cast<std::size_t>( index )] = text;

	if( IsMouseInWidget() ) {
		SetState( State::PRELIGHT );
	}
	else {
		SetState( State::NORMAL );
	}

	Invalidate();
}

void ComboBox::RemoveItem( IndexType index ) {
	if( index >= static_cast<IndexType>( m_entries.size() ) || index < 0 ) {
		return;
	}

	m_entries.erase( m_entries.begin() + index );

	// Make sure active item index keeps valid.
	if( m_active_item != NONE ) {
		if( m_active_item == index ) {
			m_active_item = NONE;
		}
		else if( m_active_item > index ) {
			m_active_item = m_entries.empty() ? NONE : m_active_item - 1;
		}
	}

	if( IsMouseInWidget() ) {
		SetState( State::PRELIGHT );
	}
	else {
		SetState( State::NORMAL );
	}

	Invalidate();
}

void ComboBox::Clear() {
	if( m_entries.empty() ) {
		return;
	}

	m_entries.clear();

	m_active_item = NONE;

	SetState( State::NORMAL );

	Invalidate();
}

ComboBox::ConstIterator ComboBox::Begin() const {
	// TODO: No std::cbegin in C++11 yet.
	return std::begin( m_entries );
}

ComboBox::ConstIterator ComboBox::End() const {
	// TODO: No std::cend in C++11 yet.
	return std::end( m_entries );
}

const sf::String& ComboBox::GetSelectedText() const {
	if( m_active_item == NONE ) {
		return EMPTY;
	}

	return m_entries[static_cast<std::size_t>( m_active_item )];
}

ComboBox::IndexType ComboBox::GetItemCount() const {
	return static_cast<IndexType>( m_entries.size() );
}

const sf::String& ComboBox::GetItem( IndexType index ) const {
	if( index >= static_cast<IndexType>( m_entries.size() ) || index < 0 ) {
		return EMPTY;
	}

	return m_entries[static_cast<std::size_t>( index )];
}

bool ComboBox::IsDropDownDisplayed() const {
	return GetState() == State::ACTIVE;
}

void ComboBox::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( GetState() == State::NORMAL ) {
		SetState( State::PRELIGHT );
	}
}

void ComboBox::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( GetState() == State::PRELIGHT ) {
		SetState( State::NORMAL );
	}
}

void ComboBox::HandleMouseMoveEvent( int x, int y ) {
	if( ( x == std::numeric_limits<int>::min() ) || ( y == std::numeric_limits<int>::min() ) ) {
		return;
	}

	if( GetState() == State::ACTIVE ) {
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

			auto scrollbar_allocation = m_scrollbar->GetAllocation();
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

			auto line_y = y;
			line_y -= static_cast<int>( GetAllocation().top + GetAllocation().height + padding );
			line_y /= static_cast<int>( Context::Get().GetEngine().GetFontLineHeight( font, font_size ) + 2 * padding );

			if( ( line_y < static_cast<int>( GetItemCount() ) ) && ( line_y >= 0 ) ) {
				if( line_y != static_cast<int>( m_highlighted_item ) ) {
					Invalidate();
					m_highlighted_item = line_y + GetDisplayedItemStart();
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
	if( ( x == std::numeric_limits<int>::min() ) || ( y == std::numeric_limits<int>::min() ) ) {
		return;
	}

	if( GetState() == State::ACTIVE ) {
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

			auto scrollbar_allocation = m_scrollbar->GetAllocation();
			scrollbar_allocation.left += GetAllocation().left;
			scrollbar_allocation.top += GetAllocation().top;

			if( scrollbar_allocation.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
				return;
			}
		}

		if( !press || ( button != sf::Mouse::Left ) ) {
			return;
		}

		auto emit_select = false;

		if( ( m_highlighted_item != NONE ) && ( m_active_item != m_highlighted_item ) ) {
			m_active_item = m_highlighted_item;
			emit_select = true;
		}

		m_highlighted_item = NONE;

		if( IsMouseInWidget() ) {
			SetState( State::PRELIGHT );
		}
		else {
			SetState( State::NORMAL );
		}

		Invalidate();

		if( emit_select ) {
			GetSignals().Emit( OnSelect );
		}

		return;
	}

	if( press && ( button == sf::Mouse::Left ) && IsMouseInWidget() ) {
		m_highlighted_item = NONE;

		SetState( State::ACTIVE );

		Invalidate();
		GetSignals().Emit( OnOpen );
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
		metrics.x = std::max( metrics.x, Context::Get().GetEngine().GetTextStringMetrics( m_entries[static_cast<std::size_t>( item )], font, font_size ).x );
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

	if( GetState() == State::ACTIVE ) {
		SetZOrder( 1 );

		GrabModal();

		m_start_entry = 0;

		float padding( Context::Get().GetEngine().GetProperty<float>( "ItemPadding", shared_from_this() ) );
		const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
		unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
		const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
		const float line_height( Context::Get().GetEngine().GetFontLineHeight( font, font_size ) );

		if( ( GetDisplayedItemCount() > 2 ) && ( GetDisplayedItemCount() < GetItemCount() ) ) {
			float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );


			const sf::Vector2f item_size(
				GetAllocation().width - 2 * border_width,
				line_height + 2 * padding
			);

			m_scrollbar = Scrollbar::Create( Scrollbar::Orientation::VERTICAL );

			auto offset = ( GetState() == State::ACTIVE ? border_width : 0.f ) + GetAllocation().width - padding - line_height;

			m_scrollbar->SetPosition( sf::Vector2f( offset, GetAllocation().height + border_width ) );
			m_scrollbar->SetRequisition( sf::Vector2f( GetAllocation().width - offset, static_cast<float>( GetDisplayedItemCount() ) * item_size.y - 2.f * border_width ) );

			m_scrollbar->GetAdjustment()->SetPageSize( static_cast<float>( GetDisplayedItemCount() ) );
			m_scrollbar->GetAdjustment()->SetLower( 0.f );
			m_scrollbar->GetAdjustment()->SetUpper( static_cast<float>( GetItemCount() ) );
			m_scrollbar->GetAdjustment()->SetMinorStep( 1.f );
			m_scrollbar->GetAdjustment()->SetMajorStep( 1.f );

			auto weak_this = std::weak_ptr<Widget>( shared_from_this() );

			m_scrollbar->GetAdjustment()->GetSignal( Adjustment::OnChange ).Connect( [weak_this] {
				auto shared_this = weak_this.lock();

				if( !shared_this ) {
					return;
				}

				auto combo_box = std::dynamic_pointer_cast<ComboBox>( shared_this );

				if( !combo_box ) {
					return;
				}

				combo_box->ChangeStartEntry();
			} );

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

		m_start_entry = 0;
	}
}

ComboBox::IndexType ComboBox::GetDisplayedItemCount() const {
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

	auto available_space = static_cast<float>( Renderer::Get().GetWindowSize().y ) - ( GetAbsolutePosition().y + item_size.y );

	auto num_displayed_entries = static_cast<IndexType>( available_space / item_size.y );

	num_displayed_entries = ( GetItemCount() < num_displayed_entries ? GetItemCount() : num_displayed_entries );

	return num_displayed_entries;
}

ComboBox::IndexType ComboBox::GetDisplayedItemStart() const {
	return m_start_entry;
}

void ComboBox::HandleUpdate( float seconds ) {
	Bin::HandleUpdate( seconds );

	if( GetState() != State::ACTIVE ) {
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
