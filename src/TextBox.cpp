#include <cmath>
#include <SFGUI/TextBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

TextBox::TextBox( float width ) :
	m_left_string(),
	m_right_string(),
	m_visible_left_string(),
	m_visible_right_string(),
	m_width( width ),
	m_border_width( 1.f ),
	m_cursor_status( false ),
	m_text_placeholder( 0 )
{
	OnFocusChange.Connect( &TextBox::HandleFocusChange, this );
	OnStateChange.Connect( &TextBox::HandleStateChange, this );
	OnMouseEnter.Connect( &TextBox::HandleMouseEnter, this );
	OnMouseLeave.Connect( &TextBox::HandleMouseLeave, this );
	OnMouseButtonClick.Connect( &TextBox::HandleMouseButtonClick, this );
	OnText.Connect( &TextBox::HandleText, this );
	OnKeyPress.Connect( &TextBox::HandleKeyPress, this );
	OnExpose.Connect( &TextBox::HandleExpose, this );
}

TextBox::Ptr TextBox::Create( float width, const sf::String& text ) {
	TextBox::Ptr  ptr( new TextBox( width ) );
	
	ptr->SetText( text );
	
	return ptr;
}

sf::Drawable* TextBox::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetEngine().CreateTextBoxDrawable( boost::shared_dynamic_cast<TextBox>( shared_from_this() ), target );
}

void TextBox::SetText( const sf::String& text ) {
	m_left_string.clear();
	m_right_string.clear();
	m_left_string.insert(m_left_string.begin(), text.GetData(), text.GetData() + text.GetSize());
	Invalidate();
	OnTextChanged.Sig(shared_from_this());
}

sf::String TextBox::GetText() const {
	return sf::String(m_left_string + m_right_string);
}

sf::String TextBox::GetVisibleText() {
	return sf::String( GetVisibleLeft() + GetVisibleRight() );
}

std::basic_string<sf::Uint32> const TextBox::GetVisibleLeft() {
	std::basic_string<sf::Uint32> visible_left_string = m_visible_left_string;
	if( m_text_placeholder != 0 ) {
		visible_left_string.replace( 0, visible_left_string.size(), visible_left_string.size(), m_text_placeholder );
	}

	return visible_left_string;
}
		
std::basic_string<sf::Uint32> const TextBox::GetVisibleRight() {
	std::basic_string<sf::Uint32> visible_right_string = m_visible_right_string;
	if( m_text_placeholder != 0 ) {
		visible_right_string.replace( 0, visible_right_string.size(), visible_right_string.size(), m_text_placeholder );
	}

	return visible_right_string;
}

std::size_t TextBox::GetCursorPos() const {
	return m_left_string.size();
}

void TextBox::SetCursorPos( std::size_t pos ) {
	if( pos == m_visible_left_string.size() ) {
		return;
	}
	else {
		int delta = (int)pos - (int)( m_visible_left_string.size() );
		
		while( delta < 0 ) {
			MoveCursorLeft();
			delta++;
		}
		
		while( delta > 0 ) {
			MoveCursorRight();
			delta--;
		}
	}
}

void TextBox::HideText( sf::Uint32 c ) {
	if( c == 0x00 || ( c > 0x1f && c != 0x7f ) ) {
		// not a control character
		m_text_placeholder = c;
		m_visible_right_string = m_right_string;
		RecalculateVisibleStrings();
	}
}

std::size_t TextBox::GetPositionFromMouseX( int mouse_pos_x ) {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Label.FontSize", shared_from_this() ) );
	u32string total_visible_string = m_visible_left_string + m_visible_right_string;
	
	float text_start = GetAllocation().Left + 2.f;
	float last_delta = std::fabs( text_start - (float)mouse_pos_x );
	std::size_t pos = 0;
	
	for( pos = 0; pos < total_visible_string.size(); pos++ ) {
		float text_length = Context::Get().GetEngine().GetTextMetrics( total_visible_string.substr( 0, pos + 1 ), font, font_size ).x;
		float new_delta = std::fabs( text_start + text_length - (float)mouse_pos_x );
		if( new_delta < last_delta ) {
			last_delta = new_delta;
		}
		else {
			break;
		}
	}
	
	return pos;
}

void TextBox::RecalculateVisibleStrings() {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Label.FontSize", shared_from_this() ) );
	float text_padding( Context::Get().GetEngine().GetProperty<float>( "TextBox.Normal.TextPadding", shared_from_this() ) );
	
	float visible_left_string_size = Context::Get().GetEngine().GetTextMetrics( GetVisibleLeft(), font, font_size ).x;
	float visible_right_string_size = Context::Get().GetEngine().GetTextMetrics( GetVisibleRight(), font, font_size ).x;
	
	// Truncate left portion of string if it is too long to fit in the allocated space
	while( visible_left_string_size > GetAllocation().Width - 2.f * text_padding ) {
		m_visible_left_string.erase( 0, 1 );
		visible_left_string_size = Context::Get().GetEngine().GetTextMetrics( GetVisibleLeft(), font, font_size ).x;
	}
	
	// Truncate right portion of string if the total string is too long to fit in the allocated space
	while( visible_left_string_size + visible_right_string_size > GetAllocation().Width - 2.f * text_padding ) {
		m_visible_right_string.resize( m_visible_right_string.size() - 1 );
		visible_right_string_size = Context::Get().GetEngine().GetTextMetrics( GetVisibleRight(), font, font_size ).x;
	}
}

void TextBox::MoveCursorLeft() {
	if( m_left_string.size() > 0 ) {
		sf::Uint32 chr = m_left_string[m_left_string.size() - 1];
		m_left_string.resize( m_left_string.size() - 1 );
		m_right_string.insert( 0, 1, chr );
		
		if( m_visible_left_string.size() > 0 ) {
			sf::Uint32 vis_chr = m_visible_left_string[m_visible_left_string.size() - 1];
			m_visible_left_string.resize( m_visible_left_string.size() - 1 );
			m_visible_right_string.insert(0, 1, vis_chr);
		}
		else {
			m_visible_right_string.insert( 0, 1, chr );
		}
		
		RecalculateVisibleStrings();
		Invalidate();
	}
}

void TextBox::MoveCursorRight() {
	if(m_right_string.size() > 0) {
		sf::Uint32 chr = m_right_string[0];
		m_right_string.erase( 0, 1 );
		m_left_string += chr;
		
		if( m_visible_right_string.size() > 0 ) {
			sf::Uint32 vis_chr = m_visible_right_string[0];
			m_visible_right_string.erase( 0, 1 );
			m_visible_left_string += vis_chr;
		}
		else {
			m_visible_left_string += chr;
		}
			
		RecalculateVisibleStrings();
		Invalidate();
	}
}

void TextBox::HandleText( Widget::Ptr /*widget*/, sf::Uint32 unicode ) {
	if( unicode > 0x1f && unicode != 0x7f ) {
		// not a control character
		m_left_string += unicode;
		m_visible_left_string += unicode;
		
		RecalculateVisibleStrings();
		Invalidate();
		OnTextChanged.Sig( shared_from_this() );
	}
}

void TextBox::HandleKeyPress( Widget::Ptr /*widget*/, sf::Event::KeyEvent event ) {
	switch(event.Code) {
	case sf::Keyboard::Back: {// backspace
		if( m_left_string.size() > 0 ) {
			m_left_string.resize( m_left_string.size() - 1 );
			
			if( m_visible_left_string.size() > 0 ) {
				m_visible_left_string.resize( m_visible_left_string.size() - 1 );
				m_visible_right_string = m_right_string;
				
				RecalculateVisibleStrings();
			}
			
			Invalidate();
			OnTextChanged.Sig( shared_from_this() );
		}
	} break;
	case sf::Keyboard::Delete: {
		if( m_right_string.size() > 0 ) {
			m_right_string.erase( 0, 1 );
			
			if( m_visible_right_string.size() > 0 ) {
				m_visible_right_string = m_right_string;
				
				RecalculateVisibleStrings();
			}
			
			Invalidate();
			OnTextChanged.Sig( shared_from_this() );
		}
	} break;
	case sf::Keyboard::Home: {
		if( m_left_string.size() > 0 ) {
			m_right_string = m_left_string + m_right_string;
			m_left_string.clear();
			
			m_visible_right_string = m_right_string;
			m_visible_left_string.clear();
			
			RecalculateVisibleStrings();
			Invalidate();
		}
	} break;
	case sf::Keyboard::End: {
		if( m_right_string.size() > 0 ) {
			m_left_string += m_right_string;
			m_right_string.clear();
			
			m_visible_left_string = m_left_string;
			m_visible_right_string.clear();
			
			RecalculateVisibleStrings();
			Invalidate();
		}
	} break;
	case sf::Keyboard::Left: {
		MoveCursorLeft();
	} break;
	case sf::Keyboard::Right: {
		MoveCursorRight();
	} break;
	default: break;
	}
}

void TextBox::HandleStateChange( Widget::Ptr /*widget*/, State /*oldstate*/ ) {
	Invalidate();
}

void TextBox::HandleMouseEnter( Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Prelight );
	}
}

void TextBox::HandleMouseLeave( Widget::Ptr /*widget*/, int /*x*/, int /*y*/ ) {
	if( GetState() != Active ) {
		SetState( Normal );
	}
}

bool TextBox::HandleMouseButtonClick( Widget::Ptr /*widget*/, int x, int /*y*/, sf::Mouse::Button /*button*/ ) {	
	GrabFocus();
	
	if( m_left_string.empty() && m_right_string.empty() ) {
		return true;
	}
	
	SetCursorPos( GetPositionFromMouseX( x ) );
	
	return true;
}

void TextBox::HandleFocusChange( Widget::Ptr /*widget*/ ) {
	if( GetState() == Active ) {
		SetState( Normal );
		m_cursor_status = false;
	}
	else {
		SetState( Active );
		m_cursor_timer.Reset();
		m_cursor_status = true;
	}
}

void TextBox::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& /*target*/ ) {
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

sf::Vector2f TextBox::GetRequisitionImpl() const {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Label.FontSize", shared_from_this() ) );

	sf::Vector2f m = Context::Get().GetEngine().GetTextMetrics(
		L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", font, font_size );
	m.x = m_width + 2 * GetBorderWidth();
	m.y += 2 * GetBorderWidth();
	return m;
}

void TextBox::SetBorderWidth(float width) {
	m_border_width = width;
	RequestSize();
	Invalidate();
}

}


