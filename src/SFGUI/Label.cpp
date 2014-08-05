#include <SFGUI/Label.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Font.hpp>

namespace sfg {

Label::Label( const sf::String& text ) :
	m_text( text ),
	m_wrap( false )
{
	SetAlignment( sf::Vector2f( .5f, .5f ) );
	Invalidate();
}

Label::Ptr Label::Create( const sf::String& text ) {
	Ptr label( new Label( text ) );
	label->RequestResize();
	return label;
}

void Label::SetText( const sf::String& text ) {
	m_text = text;

	if( m_wrap ) {
		WrapText();
	}

	RequestResize();
	Invalidate();
}

const sf::String& Label::GetText() const {
	return m_text;
}

void Label::SetLineWrap( bool wrap ) {
	if( wrap == m_wrap ) {
		return;
	}

	m_wrap = wrap;

	RequestResize();

	if( wrap ) {
		WrapText();
	}
	else {
		m_wrapped_text = L"";
	}

	Invalidate();
}

bool Label::GetLineWrap() const {
	return m_wrap;
}

sf::String Label::GetWrappedText() const {
	if( !m_wrap ) {
		return m_text;
	}

	return m_wrapped_text;
}

void Label::WrapText() {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	std::basic_string<sf::Uint32> wrapped_text;
	std::basic_string<sf::Uint32> text( m_text.begin(), m_text.end() );

	std::basic_string<sf::Uint32> line;

	while( !text.empty() ) {
		auto next_newline = text.find( L'\n' );

		line = text.substr( 0, next_newline );

		if( next_newline != std::basic_string<sf::Uint32>::npos ) {
			text.erase( 0, next_newline + 1 );
		}
		else {
			text.clear();
		}

		if( !wrapped_text.empty() ) {
			wrapped_text += L'\n';
		}

		// Check if line needs to be wrapped.
		if( Context::Get().GetEngine().GetTextStringMetrics( line, font, font_size ).x <= GetAllocation().width ) {
			wrapped_text += line;
		}
		else {
			// Line needs to be wrapped.
			while( !line.empty() ) {
				auto last_space = line.size();

				while( Context::Get().GetEngine().GetTextStringMetrics( line.substr( 0, last_space ), font, font_size ).x > GetAllocation().width ) {
					last_space = line.find_last_of( L' ', last_space - 1 );

					if( last_space == std::basic_string<sf::Uint32>::npos ) {
						wrapped_text += line;
						line.clear();
						break;
					}
				}

				if( last_space != std::basic_string<sf::Uint32>::npos ) {
					wrapped_text += line.substr( 0, last_space );
					line.erase( 0, last_space );
				}

				if( !line.empty() ) {
					wrapped_text += L'\n';

					// If this is a new line remove the leading space.
					if( line[0] == L' ' ) {
						line.erase( 0, 1 );
					}
				}
			}
		}
	}

	m_wrapped_text = wrapped_text;
}

void Label::HandleRequisitionChange() {
	if( m_wrap ) {
		WrapText();
	}

	static auto calculate_y_requisition = false;

	if( !calculate_y_requisition ) {
		calculate_y_requisition = true;
		RequestResize();
	}
	else {
		calculate_y_requisition = false;
	}
}

void Label::HandleSizeChange() {
	if( !m_wrap || ( GetAllocation().width <= 0 ) ) {
		return;
	}

	WrapText();

	static auto calculate_y_requisition = false;

	if( !calculate_y_requisition ) {
		calculate_y_requisition = true;
		RequestResize();
	}
	else {
		calculate_y_requisition = false;
	}
}

std::unique_ptr<RenderQueue> Label::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateLabelDrawable( std::dynamic_pointer_cast<const Label>( shared_from_this() ) );
}

sf::Vector2f Label::CalculateRequisition() {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	auto metrics = Context::Get().GetEngine().GetTextStringMetrics( GetWrappedText(), font, font_size );
	metrics.y = Context::Get().GetEngine().GetFontLineHeight( font, font_size );

	sf::String wrapped_text( GetWrappedText() );
	std::basic_string<sf::Uint32> text( wrapped_text.begin(), wrapped_text.end() );

	std::size_t lines = 1;

	do {
		auto next_newline = text.find( L'\n' );

		if( next_newline != std::basic_string<sf::Uint32>::npos ) {
			text.erase( 0, next_newline + 1 );
		}
		else {
			break;
		}

		if( !text.empty() ) {
			lines++;
		}
	} while( !text.empty() );

	metrics.y *= static_cast<float>( lines );

	if( m_wrap ) {
		metrics.x = 0.f;
	}

	return metrics;
}

const std::string& Label::GetName() const {
	static const std::string name( "Label" );
	return name;
}

}
