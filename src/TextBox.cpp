#include <cmath>
#include <SFGUI/TextBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

TextBox::TextBox(float width) :
	m_left_string(),
	m_right_string(),
	m_width( width ),
	m_border_width( 1.f ),
	m_cursor_status( false )
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

sf::Drawable* TextBox::InvalidateImpl( const sf::RenderTarget& target) {
	return Context::Get().GetEngine().CreateTextBoxDrawable( boost::shared_dynamic_cast<TextBox>( shared_from_this() ), target );
}

void TextBox::SetText(const sf::String& text) {
	m_left_string.clear();
	m_right_string.clear();
	m_left_string.insert(m_left_string.begin(), text.GetData(), text.GetData() + text.GetSize());
	Invalidate();
	OnTextChanged.Sig(shared_from_this());
}

sf::String TextBox::GetText() const {
	return sf::String(m_left_string + m_right_string);
}

std::size_t TextBox::GetCursorPos() const {
	return m_left_string.size();
}

void TextBox::SetCursorPos(std::size_t const pos) {
	if( pos == m_left_string.size() ) {
		return;
	}
	else {
		u32string total_string = m_left_string + m_right_string;
		m_left_string = total_string.substr( 0, pos );
		m_right_string = total_string.substr( pos );
		Invalidate();
	}
}

std::size_t TextBox::GetPositionFromMouseX( int mouse_pos_x ) {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Label.FontSize", shared_from_this() ) );
	u32string total_string = m_left_string + m_right_string;
	
	float text_start = GetAllocation().Left + 2.f;
	float last_delta = std::fabs( text_start - (float)mouse_pos_x );
	std::size_t pos = 0;
	
	for( pos = 0; pos < total_string.size(); pos++ ) {
		float text_length = Context::Get().GetEngine().GetTextMetrics( total_string.substr( 0, pos + 1 ), font, font_size ).x;
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

void TextBox::HandleText( Widget::Ptr /*widget*/, sf::Uint32 unicode ) {
	if(unicode > 0x1f && unicode != 0x7f) {
		// not a control character
		m_left_string += unicode;
		Invalidate();
		OnTextChanged.Sig(shared_from_this());
	}
}

void TextBox::HandleKeyPress( Widget::Ptr /*widget*/, sf::Event::KeyEvent event ) {
	switch(event.Code) {
	case sf::Keyboard::Back: {// backspace
		if(m_left_string.size() > 0) {
			m_left_string.resize(m_left_string.size() - 1);
			Invalidate();
			OnTextChanged.Sig(shared_from_this());
		}
	} break;
	case sf::Keyboard::Delete: {
		if(m_right_string.size() > 0) {
			m_right_string.erase(0, 1);
			Invalidate();
			OnTextChanged.Sig(shared_from_this());
		}
	} break;
	case sf::Keyboard::Home: {
		if(m_left_string.size() > 0) {
			m_right_string = m_left_string + m_right_string;
			m_left_string.clear();
			Invalidate();
		}
	} break;
	case sf::Keyboard::End: {
		if(m_right_string.size() > 0) {
			m_left_string += m_right_string;
			m_right_string.clear();
			Invalidate();
		}
	} break;
	case sf::Keyboard::Left: {
		if(m_left_string.size() > 0) {
			sf::Uint32 chr = m_left_string[m_left_string.size() - 1];
			m_left_string.resize(m_left_string.size() - 1);
			m_right_string.insert(0, 1, chr);
			Invalidate();
		}
	} break;
	case sf::Keyboard::Right: {
		if(m_right_string.size() > 0) {
			sf::Uint32 chr = m_right_string[0];
			m_right_string.erase(0, 1);
			m_left_string += chr;
			Invalidate();
		}
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


