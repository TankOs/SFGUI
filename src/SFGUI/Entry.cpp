#include <SFGUI/Entry.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

#include <SFML/Graphics/Font.hpp>
#include <cmath>

namespace sfg {

// Signals.
Signal::SignalID Entry::OnTextChanged = 0;

Entry::Entry() :
	m_string(),
	m_visible_string(),
	m_visible_offset( 0 ),
	m_text_placeholder( 0 ),
	m_max_length( 0 ),
	m_cursor_position( 0 ),
	m_elapsed_time( 0.f ),
	m_text_margin( 0.f ),
	m_cursor_status( false )
{
}

Entry::Ptr Entry::Create( const sf::String& text ) {
	Entry::Ptr ptr( new Entry );

	ptr->SetText( text );

	return ptr;
}

RenderQueue* Entry::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateEntryDrawable( DynamicPointerCast<const Entry>( shared_from_this() ) );
}

void Entry::SetText( const sf::String& text ) {
	m_string = text;
	m_visible_offset = 0;
	m_cursor_position = 0;
	RecalculateVisibleString();
	GetSignals().Emit( OnTextChanged );
}

const sf::String& Entry::GetText() const {
	return m_string;
}

void Entry::AppendText( const sf::String& text ) {
	m_string += text;
}

void Entry::PrependText( const sf::String& text ) {
	m_string = text + m_string;
}

void Entry::SetCursorPosition( std::size_t new_position ) {
	if( new_position == m_cursor_position ) {
		return;
	}

	int delta = static_cast<int>( new_position ) - static_cast<int>( m_cursor_position );
	MoveCursor( delta );
}

void Entry::HideText( sf::Uint32 c ) {
	if( c == 0x00 || ( c > 0x1f && c != 0x7f ) ) {
		// not a control character
		m_text_placeholder = c;
		RecalculateVisibleString();
	}
}

sf::Uint32 Entry::GetHideCharacter() const {
	return m_text_placeholder;
}

std::size_t Entry::GetPositionFromMouseX( int mouse_pos_x ) {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	float text_padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );

	std::basic_string<sf::Uint32> string( m_visible_string.begin(), m_visible_string.end() );

	float text_start = GetAllocation().left + text_padding;
	float last_delta = std::fabs( text_start - static_cast<float>( mouse_pos_x ) );
	std::size_t cursor_position = 0;

	for( cursor_position = 0; cursor_position < string.size(); cursor_position++ ) {
		float text_length = Context::Get().GetEngine().GetTextMetrics( string.substr( 0, cursor_position + 1 ), font, font_size ).x;
		float new_delta = std::fabs( text_start + text_length - static_cast<float>( mouse_pos_x ) );
		if( new_delta < last_delta ) {
			last_delta = new_delta;
		}
		else {
			break;
		}
	}

	return m_visible_offset + cursor_position;
}


void Entry::RecalculateVisibleString() const {
	float text_padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	if( m_string.isEmpty() ) {
		m_visible_string.clear();
		Invalidate();
		return;
	}

	std::basic_string<sf::Uint32> string( m_string.begin(), m_string.end() );
	string = string.substr( m_visible_offset );

	if( m_text_placeholder != 0 ) {
		string.replace( 0, string.size(), string.size(), m_text_placeholder );
	}

	float length = Context::Get().GetEngine().GetTextMetrics( string, font, font_size ).x;

	// While the string is too long for the given space keep chopping off characters
	// on the right end of the string until the cursor is reached, then start
	// chopping off characters on the left side of the string.
	while( !string.empty() && (GetAllocation().width - m_text_margin > 0) && (length > GetAllocation().width - m_text_margin - (2.f * text_padding)) ) {
		if( ( m_cursor_position - m_visible_offset ) < string.size() ) {
			string = string.substr( 0, string.size() - 1 );
		}
		else {
			string = string.substr( 1, string.size() - 1 );
			m_visible_offset++;
		}

		length = Context::Get().GetEngine().GetTextMetrics( string, font, font_size ).x;
	}

	m_visible_string = string;
	Invalidate();
}

void Entry::MoveCursor( int delta ) {
	if( delta && ( static_cast<int>( m_cursor_position ) + delta >= 0 ) && ( static_cast<int>( m_cursor_position ) + delta <= static_cast<int>( m_string.getSize() ) ) ) {
		m_cursor_position += delta;

		if( m_cursor_position < m_visible_offset ) {
			m_visible_offset = m_cursor_position;
		}

		// Make cursor visible.
		m_elapsed_time = 0.f;
		m_cursor_status = true;

		RecalculateVisibleString();
	}
}

void Entry::HandleTextEvent( sf::Uint32 character ) {
	if( m_max_length > 0 && m_string.getSize() >= m_max_length ) {
		return;
	}

	if( character > 0x1f && character != 0x7f ) {
		// not a control character
		m_string.insert( m_cursor_position, character );
		MoveCursor( 1 );

		GetSignals().Emit( OnTextChanged );
	}
}

void Entry::HandleKeyEvent( sf::Keyboard::Key key, bool press ) {
	if( !press || !HasFocus() ) {
		return;
	}

	switch( key ) {
	case sf::Keyboard::BackSpace: { // backspace
		if( ( m_string.getSize() > 0 ) && ( m_cursor_position > 0 ) ) {
			m_string.erase( m_cursor_position - 1 );

			// Store old number of visible characters.
			std::size_t old_num_visible_chars = m_visible_string.getSize();

			MoveCursor( -1 );
			RecalculateVisibleString();

			// If new amount of chars is less and we have some chars in front, go
			// back.
			if( m_visible_offset > 0 && old_num_visible_chars > m_visible_string.getSize() ) {
				--m_visible_offset;
				RecalculateVisibleString();
			}

			m_elapsed_time = 0.f;
			m_cursor_status = true;

			GetSignals().Emit( OnTextChanged );
		}
	} break;
	case sf::Keyboard::Delete: {
		if( ( m_string.getSize() > 0 ) && ( m_cursor_position < m_string.getSize() ) ) {
			m_string.erase( m_cursor_position );

			// Store old number of visible characters.
			std::size_t old_num_visible_chars = m_visible_string.getSize();

			RecalculateVisibleString();

			// If new amount of chars is less and we have some chars in front, go
			// back.
			if( m_visible_offset > 0 && old_num_visible_chars > m_visible_string.getSize() ) {
				--m_visible_offset;
				RecalculateVisibleString();
			}

			m_elapsed_time = 0.f;
			m_cursor_status = true;

			GetSignals().Emit( OnTextChanged );
		}
	} break;
	case sf::Keyboard::Home: {
		if( m_string.getSize() > 0 ) {
			m_visible_offset = 0;
			SetCursorPosition( 0 );
		}
	} break;
	case sf::Keyboard::End: {
		if( m_string.getSize() > 0 ) {
			m_visible_offset = 0;
			SetCursorPosition( m_string.getSize() );
		}
	} break;
	case sf::Keyboard::Left: {
		MoveCursor( -1 );
	} break;
	case sf::Keyboard::Right: {
		MoveCursor( 1 );
	} break;
	default: break;
	}
}

void Entry::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( !HasFocus() ) {
		SetState( PRELIGHT );
	}
}

void Entry::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( !HasFocus() ) {
		SetState( NORMAL );
	}
}

void Entry::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int /*y*/ ) {
	if( !press || !IsMouseInWidget() ) {
		return;
	}

	if( button != sf::Mouse::Left ) {
		// TODO: Maybe some more support for right clicking in the future.
		return;
	}

	GrabFocus();
	SetCursorPosition( GetPositionFromMouseX( x ) );
}

void Entry::HandleUpdate( float seconds ) {
	if( !HasFocus() ) {
		return;
	}

	m_elapsed_time += seconds;

	// Toggle cursor state every 0.5 seconds
	if( m_elapsed_time > .5f ) {
		m_elapsed_time = 0.f;
		m_cursor_status = !m_cursor_status;
		Invalidate();
	}
}

void Entry::HandleFocusChange( const Widget::Ptr& focused_widget ) {
	if( HasFocus() ) {
		m_elapsed_time = 0.f;
		m_cursor_status = true;
	}

	Invalidate();

	Widget::HandleFocusChange( focused_widget );
}

void Entry::HandleSizeChange() {
	if( HasFocus() ) {
		GrabFocus( Widget::Ptr() );
	}

	SetCursorPosition( 0 );
	RecalculateVisibleString();
}

sf::Vector2f Entry::CalculateRequisition() {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float text_padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	float line_height = Context::Get().GetEngine().GetFontLineHeight( font, font_size );

	return sf::Vector2f( 2 * (border_width + text_padding), line_height + 2 * ( border_width + text_padding ) );
}

bool Entry::IsCursorVisible() const {
	return m_cursor_status;
}

std::size_t Entry::GetVisibleOffset() const {
	return m_visible_offset;
}

std::size_t Entry::GetCursorPosition() const {
	return m_cursor_position;
}

const sf::String& Entry::GetVisibleText() const {
	return m_visible_string;
}

const std::string& Entry::GetName() const {
	static const std::string name( "Entry" );
	return name;
}

std::size_t Entry::GetMaximumLength() const {
	return m_max_length;
}

void Entry::SetMaximumLength( std::size_t max_length ) {
	SetCursorPosition( 0 );

	m_max_length = max_length;

	// Truncate text if longer than maximum.
	if( m_max_length < m_string.getSize() && m_max_length != 0 ) {
		m_string.erase( m_max_length, m_max_length - m_string.getSize() );
		RecalculateVisibleString();
		GetSignals().Emit( OnTextChanged );
	}
}

void Entry::SetTextMargin( float margin ) {
	m_text_margin = margin;
}

}
