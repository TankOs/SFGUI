#include <SFGUI/Entry.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Clipboard.hpp>
#include <cmath>

namespace sfg {

// Signals.
Signal::SignalID Entry::OnTextChanged = 0;
Signal::SignalID Entry::OnReturnPressed = 0;

Entry::Entry() :
	m_string(),
	m_visible_string(),
	m_visible_offset( 0 ),
	m_text_placeholder( 0 ),
	m_max_length( 0 ),
	m_cursor_position( 0 ),
	m_cursor_selection_start( 0 ),
	m_elapsed_time( 0.f ),
	m_text_margin( 0.f ),
	m_cursor_status( false )
{
}

Entry::Ptr Entry::Create( const sf::String& text ) {
	Ptr ptr( new Entry );
	ptr->SetText( text );
	return ptr;
}

std::unique_ptr<RenderQueue> Entry::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateEntryDrawable( std::dynamic_pointer_cast<const Entry>( shared_from_this() ) );
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
	SetText( m_string + text );
}

void Entry::PrependText( const sf::String& text ) {
	SetText( text + m_string );
}

void Entry::SetCursorPosition( int new_position ) {
	if( new_position == m_cursor_position ) {
		return;
	}

	auto delta = static_cast<int>( new_position ) - static_cast<int>( m_cursor_position );
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

int Entry::GetPositionFromMouseX( int mouse_pos_x ) {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	float text_padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );

	std::basic_string<sf::Uint32> string( m_visible_string.begin(), m_visible_string.end() );

	auto text_start = GetAllocation().left + text_padding;
	auto last_delta = std::fabs( text_start - static_cast<float>( mouse_pos_x ) );
	int cursor_position = 0;
	auto length = static_cast<int>( string.size() );

	for( cursor_position = 0; cursor_position < length; cursor_position++ ) {
		auto text_length = Context::Get().GetEngine().GetTextStringMetrics( string.substr( 0, static_cast<std::size_t>( cursor_position + 1 ) ), font, font_size ).x;
		auto new_delta = std::fabs( text_start + text_length - static_cast<float>( mouse_pos_x ) );
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
	string.erase( 0, static_cast<std::size_t>( m_visible_offset ) );

	if( m_text_placeholder != 0 ) {
		string.replace( 0, string.size(), string.size(), m_text_placeholder );
	}

	auto length = Context::Get().GetEngine().GetTextStringMetrics( string, font, font_size ).x;

	// While the string is too long for the given space keep chopping off characters
	// on the right end of the string until the cursor is reached, then start
	// chopping off characters on the left side of the string.
	while( !string.empty() && (GetAllocation().width - m_text_margin > 0) && (length > GetAllocation().width - m_text_margin - (2.f * text_padding)) ) {
		if( ( m_cursor_position - m_visible_offset ) < static_cast<int>( string.size() ) ) {
			string.erase( string.size() - 1, 1 );
		}
		else {
			string.erase( 0, 1 );
			m_visible_offset++;
		}

		length = Context::Get().GetEngine().GetTextStringMetrics( string, font, font_size ).x;
	}

	m_visible_string = string;
	Invalidate();
}

void Entry::MoveCursor( int delta ) {
	if( delta && ( m_cursor_position + delta >= 0 ) && ( m_cursor_position + delta <= static_cast<int>( m_string.getSize() ) ) ) {
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
	int left, right;
	GetSelectionBounds(left, right);

	if( m_max_length > 0 && left == right && static_cast<int>( m_string.getSize() ) >= m_max_length ) {
		return;
	}

	if( character > 0x1f && character != 0x7f ) {
		// not a control character
		if (left != right) {
			m_string.erase(static_cast<std::size_t>(left), static_cast<std::size_t>(right - left));
			if (m_cursor_position == right) {
				m_cursor_position -= right - left;
			}
		}

		m_string.insert( static_cast<std::size_t>( m_cursor_position ), character );
		MoveCursor( 1 );
		m_cursor_selection_start = m_cursor_position;

		GetSignals().Emit( OnTextChanged );
	}
}

void Entry::HandleKeyEvent( sf::Keyboard::Key key, bool press ) {
	if( !press || !HasFocus() ) {
		return;
	}

	switch( key ) {
	case sf::Keyboard::A: {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			m_cursor_selection_start = 0;
			size_t end = m_string.getSize();

			if (m_cursor_position == end) {
				m_cursor_position = 0; // make sure that is gets updated
			}
			SetCursorPosition(end);
		}
	} break;
	case sf::Keyboard::C: {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			int left, right;
			GetSelectionBounds(left, right);

			if (left != right) {
				sf::Clipboard::setString(GetSelectedText());
			}
		}
	} break;
	case sf::Keyboard::V: {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			int left, right;
			GetSelectionBounds(left, right);
			const sf::String& str = sf::Clipboard::getString();

			if (!str.isEmpty()) {
				sf::String left_substr = m_string.substring(0, left);
				sf::String right_substr = m_string.substring(right, m_string.getSize() - right);

				SetText(left_substr + str + right_substr);
				SetCursorPosition(left_substr.getSize() + str.getSize());
				m_cursor_selection_start = m_cursor_position;
			}
		}
	} break;
	case sf::Keyboard::X: {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			int left, right;
			GetSelectionBounds(left, right);

			if (left != right) {
				sf::String selected = GetSelectedText();
				sf::Clipboard::setString(selected);
				sf::String left_substr = m_string.substring(0, left);
				sf::String right_substr = m_string.substring(right, m_string.getSize() - right);

				SetText(left_substr + right_substr);
				SetCursorPosition(left_substr.getSize());
				m_cursor_selection_start = m_cursor_position;
			}
		}
	} break;
	case sf::Keyboard::BackSpace: { // backspace
		if( ( m_string.getSize() > 0 ) && ( m_cursor_position > 0 || m_cursor_selection_start > 0 ) ) {
			// Store old number of visible characters.
			auto old_num_visible_chars = m_visible_string.getSize();

			int left, right;
			GetSelectionBounds(left, right);

			if (left == right) {
				m_string.erase(static_cast<std::size_t>(left - 1));
				MoveCursor(-1);
			}
			else {
				m_string.erase(static_cast<std::size_t>(left), static_cast<std::size_t>(right - left));
				if (m_cursor_position == right) {
					MoveCursor(left - right);
				}
			}
			m_cursor_selection_start = m_cursor_position;

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
		if( ( m_string.getSize() > 0 ) && ( std::min(m_cursor_position, m_cursor_selection_start) < static_cast<int>( m_string.getSize() ) ) ) {
			// Store old number of visible characters.
			auto old_num_visible_chars = m_visible_string.getSize();

			int left, right;
			GetSelectionBounds(left, right);

			if (left == right) {
				m_string.erase(static_cast<std::size_t>(left));
			}
			else {
				m_string.erase(static_cast<std::size_t>(left), static_cast<std::size_t>(right - left));
				if (m_cursor_position == right) {
					MoveCursor(left - right);
				}
			}
			m_cursor_selection_start = m_cursor_position;

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
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
			m_cursor_selection_start = m_cursor_position;
		}
	} break;
	case sf::Keyboard::End: {
		if( m_string.getSize() > 0 ) {
			m_visible_offset = 0;
			SetCursorPosition( static_cast<int>( m_string.getSize() ) );
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
			m_cursor_selection_start = m_cursor_position;
		}
	} break;
	case sf::Keyboard::Left: {
		MoveCursor( -1 );
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
			m_cursor_selection_start = m_cursor_position;
		}
	} break;
	case sf::Keyboard::Right: {
		MoveCursor( 1 );
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
			m_cursor_selection_start = m_cursor_position;
		}
	} break;
	case sf::Keyboard::Return: {
		GetSignals().Emit( OnReturnPressed );
	} break;
	default: break;
	}
}

void Entry::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( !HasFocus() ) {
		SetState( State::PRELIGHT );
	}
}

void Entry::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( !HasFocus() ) {
		SetState( State::NORMAL );
	}
}

void Entry::HandleMouseMoveEvent(int x, int y)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		SetCursorPosition(GetPositionFromMouseX(x));
	}
}

void Entry::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int /*y*/ ) {
	if( !IsMouseInWidget() ) {
		return;
	}

	if( button != sf::Mouse::Left ) {
		// TODO: Maybe some more support for right clicking in the future.
		return;
	}

	if (press) {
		GrabFocus();
		SetCursorPosition(GetPositionFromMouseX(x));
		m_cursor_selection_start = m_cursor_position;
	}
	else {
		SetCursorPosition(GetPositionFromMouseX(x));
	}
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

void Entry::HandleFocusChange( Widget::Ptr focused_widget ) {
	if( HasFocus() ) {
		m_elapsed_time = 0.f;
		m_cursor_status = true;
	}
	else {
		m_cursor_position = 0;
		m_cursor_selection_start = 0;
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
	auto line_height = Context::Get().GetEngine().GetFontLineHeight( font, font_size );

	return sf::Vector2f( 2 * (border_width + text_padding), line_height + 2 * ( border_width + text_padding ) );
}

bool Entry::IsCursorVisible() const {
	return m_cursor_status;
}

int Entry::GetVisibleOffset() const {
	return m_visible_offset;
}

int Entry::GetVisibleLength() const
{
	return m_visible_string.getSize();
}

int Entry::GetCursorPosition() const {
	return m_cursor_position;
}

const sf::String& Entry::GetVisibleText() const {
	return m_visible_string;
}

const std::string& Entry::GetName() const {
	static const std::string name( "Entry" );
	return name;
}

int Entry::GetMaximumLength() const {
	return m_max_length;
}

void Entry::SetMaximumLength( int max_length ) {
	SetCursorPosition( 0 );

	m_max_length = max_length;

	// Truncate text if longer than maximum.
	if( m_max_length < static_cast<int>( m_string.getSize() ) && m_max_length != 0 ) {
		m_string.erase( static_cast<std::size_t>( m_max_length ), static_cast<std::size_t>( m_max_length ) - m_string.getSize() );
		RecalculateVisibleString();
		GetSignals().Emit( OnTextChanged );
	}
}

void Entry::GetSelectionBounds(int& left, int& right) const
{
	if (m_cursor_position < m_cursor_selection_start) {
		left = m_cursor_position;
		right = m_cursor_selection_start;
	}
	else {
		left = m_cursor_selection_start;
		right = m_cursor_position;
	}
}

sf::String Entry::GetSelectedText() const
{
	int left, right;
	GetSelectionBounds(left, right);
	return m_string.substring(left, right - left);
}

void Entry::SetTextMargin( float margin ) {
	m_text_margin = margin;
}

}
