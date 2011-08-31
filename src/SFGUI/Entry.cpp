#include <SFGUI/Entry.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <cmath>

namespace sfg {

Entry::Entry() :
	m_string(),
	m_visible_string(),
	m_visible_offset( 0 ),
	m_cursor_position( 0 ),
	m_cursor_status( false ),
	m_text_placeholder( 0 )
{
	OnFocusChange.Connect( &Entry::HandleFocusChange, this );
	OnStateChange.Connect( &Entry::HandleStateChange, this );
	OnMouseEnter.Connect( &Entry::HandleMouseEnter, this );
	OnMouseLeave.Connect( &Entry::HandleMouseLeave, this );
	OnMouseButtonClick.Connect( &Entry::HandleMouseButtonClick, this );
	OnText.Connect( &Entry::HandleText, this );
	OnKeyPress.Connect( &Entry::HandleKeyPress, this );
	OnExpose.Connect( &Entry::HandleExpose, this );
}

Entry::Ptr Entry::Create( const sf::String& text ) {
	Entry::Ptr  ptr( new Entry );
	
	ptr->SetText( text );
	
	return ptr;
}

sf::Drawable* Entry::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetEngine().CreateEntryDrawable( boost::shared_dynamic_cast<Entry>( shared_from_this() ), target );
}

void Entry::SetText( const sf::String& text ) {
	m_string = text;
	m_visible_offset = 0;
	m_cursor_position = 0;
	RecalculateVisibleString();
	OnTextChanged.Sig(shared_from_this());
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

std::size_t Entry::GetPositionFromMouseX( int mouse_pos_x ) {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Label.FontSize", shared_from_this() ) );

  std::basic_string<sf::Uint32> string( m_visible_string.Begin(), m_visible_string.End() );
	float text_start = GetAllocation().Left + 2.f;
	float last_delta = std::fabs( text_start - (float)mouse_pos_x );
	std::size_t cursor_position = 0;

	for( cursor_position = 0; cursor_position < string.size(); cursor_position++ ) {
		float text_length = Context::Get().GetEngine().GetTextMetrics( string.substr( 0, cursor_position + 1 ), font, font_size ).x;
		float new_delta = std::fabs( text_start + text_length - (float)mouse_pos_x );
		if( new_delta < last_delta ) {
			last_delta = new_delta;
		}
		else {
			break;
		}
	}

	return m_visible_offset + cursor_position;
}


void Entry::RecalculateVisibleString() {
  float text_padding( Context::Get().GetEngine().GetProperty<float>( "Entry.TextPadding", shared_from_this() ) );
  const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
  const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
  unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Label.FontSize", shared_from_this() ) );

  if( m_string.IsEmpty() ) {
    m_visible_string.Clear();
    Invalidate();
    return;
  }

	std::basic_string<sf::Uint32> string( m_string.Begin(), m_string.End() );
  string = string.substr( m_visible_offset );

	if( m_text_placeholder != 0 ) {
		string.replace( 0, string.size(), string.size(), m_text_placeholder );
	}

  float length = Context::Get().GetEngine().GetTextMetrics( string, font, font_size ).x;

  // While the string is too long for the given space keep chopping off characters
  // on the right end of the string until the cursor is reached, then start
  // chopping off characters on the left side of the string.
	while( (GetAllocation().Width > 0) && (length > GetAllocation().Width - (2.f * text_padding)) ) {
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
	if( delta && ( m_cursor_position + delta >= 0 ) && ( m_cursor_position + delta <= m_string.GetSize() ) ) {
		m_cursor_position += delta;

		if( m_cursor_position < m_visible_offset ) {
      m_visible_offset = m_cursor_position;
		}

		RecalculateVisibleString();
	}
}

void Entry::HandleText( Widget::Ptr /*widget*/, sf::Uint32 unicode ) {
	if( unicode > 0x1f && unicode != 0x7f ) {
		// not a control character
    m_string.Insert( m_cursor_position, unicode );
    m_cursor_position++;

		RecalculateVisibleString();
		OnTextChanged.Sig( shared_from_this() );
	}
}

void Entry::HandleKeyPress( Widget::Ptr /*widget*/, sf::Event::KeyEvent event ) {
	switch(event.Code) {
	case sf::Keyboard::Back: { // backspace
		if( ( m_string.GetSize() > 0 ) && ( m_cursor_position > 0 ) ) {
			m_string.Erase( m_cursor_position - 1 );
			MoveCursor( -1 );
			OnTextChanged.Sig( shared_from_this() );
		}
	} break;
	case sf::Keyboard::Delete: {
		if( ( m_string.GetSize() > 0 ) && ( m_cursor_position < m_string.GetSize() ) ) {
			m_string.Erase( m_cursor_position );
			RecalculateVisibleString();
			OnTextChanged.Sig( shared_from_this() );
		}
	} break;
	case sf::Keyboard::Home: {
		if( m_string.GetSize() > 0 ) {
			m_visible_offset = 0;
      SetCursorPosition( 0 );
		}
	} break;
	case sf::Keyboard::End: {
		if( m_string.GetSize() > 0 ) {
			m_visible_offset = 0;
      SetCursorPosition( m_string.GetSize() );
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

void Entry::HandleStateChange( Widget::Ptr /*widget*/, State /*oldstate*/ ) {
	Invalidate();
}

void Entry::HandleMouseEnter( Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Prelight );
	}
}

void Entry::HandleMouseLeave( Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Normal );
	}
}

bool Entry::HandleMouseButtonClick( Widget::Ptr /*widget*/, int x, int /*y*/, sf::Mouse::Button /*button*/ ) {	
	GrabFocus();
	
	if( m_string.IsEmpty() ) {
		return true;
	}
	
	SetCursorPosition( GetPositionFromMouseX( x ) );
	
	return true;
}

void Entry::HandleFocusChange( Widget::Ptr /*widget*/ ) {
	if( GetState() == Active ) {
		SetState( Normal );
	}
	else {
		SetState( Active );
	}
}

void Entry::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& /*target*/ ) {
	if( GetState() != Active ) {
		return;
	}
	
	// Toggle cursor state every 500ms
	if( m_cursor_timer.GetElapsedTime() > 500 ) {
		m_cursor_timer.Reset();
		m_cursor_status = !m_cursor_status;
		Invalidate();
	}
}

sf::Vector2f Entry::GetRequisitionImpl() const {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Entry.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Entry.FontSize", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "Entry.Normal.BorderWidth", shared_from_this() ) );
  float text_padding( Context::Get().GetEngine().GetProperty<float>( "Entry.TextPadding", shared_from_this() ) );

	sf::Vector2f m = Context::Get().GetEngine().GetTextMetrics( m_string, font, font_size );
	m.x += 2 * border_width + 2 * text_padding;
	m.y += 2 * border_width + 2 * text_padding;

	return m;
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

}
