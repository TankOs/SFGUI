#include <SFGUI/TextBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

TextBox::TextBox(float width)
	: left()
	, right()
	, width(width)
{
	OnFocusChange.Connect( &TextBox::HandleFocusChange, this );
	OnStateChange.Connect( &TextBox::HandleStateChange, this );
	OnMouseEnter.Connect( &TextBox::HandleMouseEnter, this );
	OnMouseLeave.Connect( &TextBox::HandleMouseLeave, this );
	OnMouseButtonClick.Connect( &TextBox::HandleMouseButtonClick, this );
	OnText.Connect( &TextBox::HandleText, this );
	OnKeyRelease.Connect( &TextBox::HandleKeyRelease, this );
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
	left.clear();
	right.clear();
	left.insert(left.begin(), text.GetData(), text.GetData() + text.GetSize());
	Invalidate();
	TextChanged.Sig(shared_from_this());
}

sf::String TextBox::GetText() const {
	return sf::String(left + right);
}

std::size_t TextBox::GetCursorPos() const {
	return left.size();
}

void TextBox::SetCursorPos(std::size_t const pos) {
	typedef u32string::difference_type diff_t;
	if(pos == left.size()) {
		return;
	}
	else if(pos > left.size()) {
		diff_t const off = diff_t(pos) - diff_t(left.size());
		left.insert(left.end(), right.begin(), right.begin() + off);
		right.erase(right.begin(), right.begin() + off);
		Invalidate();
	}
	else {
		diff_t const off = diff_t(left.size()) - diff_t(pos);
		right.insert(right.begin(), left.begin() + off, left.end());
		left.erase(left.begin() + off, left.end());
		Invalidate();
	}
}

void TextBox::HandleText( Widget::Ptr /*widget*/, sf::Uint32 unicode ) {
	if(unicode > 0x1f && unicode != 0x7f) {
		// not a control character
		left += unicode;
		Invalidate();
		TextChanged.Sig(shared_from_this());
	}
}

void TextBox::HandleKeyRelease( Widget::Ptr /*widget*/, sf::Event::KeyEvent event ) {
	switch(event.Code) {
	case sf::Key::Back: // backspace
		if(left.size() > 0) {
			left.resize(left.size() - 1);
			Invalidate();
			TextChanged.Sig(shared_from_this());
		}
		break;

	case sf::Key::Delete:
		if(right.size() > 0) {
			right.erase(0, 1);
			Invalidate();
			TextChanged.Sig(shared_from_this());
		}
		break;

	case sf::Key::Home:
		if(left.size() > 0) {
			right = left + right;
			left.clear();
			Invalidate();
		}
		break;

	case sf::Key::End:
		if(right.size() > 0) {
			left += right;
			right.clear();
			Invalidate();
		}
		break;

	case sf::Key::Left:
		if(left.size() > 0) {
			sf::Uint32 chr = left[left.size() - 1];
			left.resize(left.size() - 1);
			right.insert(0, 1, chr);
			Invalidate();
		}
		break;

	case sf::Key::Right:
		if(right.size() > 0) {
			sf::Uint32 chr = right[0];
			right.erase(0, 1);
			left += chr;
			Invalidate();
		}
		break;

	default:
		break;
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

bool TextBox::HandleMouseButtonClick( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button /*button*/ ) {
	GrabFocus();
	return true;
}

void TextBox::HandleFocusChange( Widget::Ptr /*widget*/ ) {
	if( GetState() == Active ) {
		SetState( Normal );
	}
	else {
		SetState( Active );
	}
}

sf::Vector2f TextBox::GetRequisitionImpl() const {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "Label.FontSize", shared_from_this() ) );

	sf::Vector2f m = Context::Get().GetEngine().GetTextMetrics(
		L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", font, font_size );
	m.x = width + 2 * GetBorderWidth();
	m.y += 2 * GetBorderWidth();
	return m;
}

void TextBox::SetBorderWidth(float w) {
	borderWidth = w;
	RequestSize();
	Invalidate();
}

}


