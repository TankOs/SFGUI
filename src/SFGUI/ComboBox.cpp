#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

ComboBox::ComboBox() :
	Bin(),
	m_active( false ),
	m_active_index( -1 ),
	m_highlighted( -1 )
{
}

ComboBox::~ComboBox() {
}

ComboBox::Ptr ComboBox::Create() {
	ComboBox::Ptr  ptr( new ComboBox );

	return ptr;
}

RenderQueue* ComboBox::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateComboBoxDrawable( DynamicPointerCast<const ComboBox>( shared_from_this() ) );
}

int ComboBox::GetActive() const {
	return m_active_index;
}

int ComboBox::GetHighlighted() const {
	return m_highlighted;
}

void ComboBox::SetActive( int index ) {
	m_active_index = index;
	if( index >= GetNumberEntries() || index < 0 ) {
		m_active_index = -1;
	}
}

void ComboBox::AppendText( const sf::String& text ) {
	m_entries.push_back( text );
	RequestResize();
}

void ComboBox::InsertText( int position, const sf::String& text ) {
	m_entries.insert( m_entries.begin() + position, text );
	if( m_active_index >= position ) {
		++m_active_index;
	}
	RequestResize();
}

void ComboBox::PrependText( const sf::String& text ) {
	m_entries.insert( m_entries.begin(), text );
	m_active_index += 1;
	RequestResize();
}

void ComboBox::ChangeText( int index, const sf::String& text ) {
	if( index < 0 || index >= static_cast<int>( m_entries.size() ) ) {
		return;
	}
	m_entries[ index ] = text;
	RequestResize();
}

void ComboBox::RemoveText( int index ) {
	m_entries.erase( m_entries.begin() + index );
	if( m_active_index >= index ) {
		--m_active_index;
	}
	RequestResize();
}

const sf::String& ComboBox::GetActiveText() const {
	if( m_active_index >= 0 && m_active_index < static_cast<int>( m_entries.size() ) ) {
		return m_entries.at( m_active_index );
	}
	static sf::String empty( "" );
	return empty;
}

std::size_t ComboBox::GetNumberEntries() const {
	return static_cast<int>( m_entries.size() );
}

const sf::String& ComboBox::GetEntryText( const int index ) const {
	static sf::String empty( "" );
	if( index == -1 ) {
		return empty;
	}
	return m_entries.at( index );
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

void ComboBox::HandleMouseMoveEvent( int /*x*/, int y ) {
	if( m_active ) {
		float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
		const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
		unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
		const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
		
		int line_y = y;
		line_y -= static_cast<int>( GetAllocation().Top + GetAllocation().Height + GetBorderWidth() + padding );
		line_y /= static_cast<int>( Context::Get().GetEngine().GetLineHeight( font, font_size ) );
		
		if( line_y >= 0 && line_y < GetNumberEntries() ) {
			if( line_y != m_highlighted ) {
				Invalidate();
			}
			m_highlighted = line_y;
		}
	}
}

void ComboBox::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( button == sf::Mouse::Left && IsMouseInWidget() ) {
		if( press ) {
			SetState( ACTIVE );
		}
		else {
			SetState( PRELIGHT );
		}
	}
	
	if( button == sf::Mouse::Left ) {
		if( m_active && !press ) {
			if( m_highlighted != -1 ) {
				m_active_index = m_highlighted;
				m_active = false;
				OnSelect();
				Invalidate();
				m_highlighted = -1;
			}
			else {
				m_active = false;
				Invalidate();
				m_highlighted = -1;
			}
		}
		else if( !m_active && press ) {
			m_active = true;
			OnOpen();
			Invalidate();
		}
	}
	
	if( !IsMouseInWidget() ) {
		SetState( NORMAL );
		m_active = false;
		m_highlighted = -1;
		Invalidate();
		return;
	}
}

sf::Vector2f ComboBox::CalculateRequisition() {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	sf::Vector2f metrics( 0.f, 0.f );
	for ( int i = 0; i < GetNumberEntries(); ++i ) {
		if( Context::Get().GetEngine().GetTextMetrics( m_entries.at( i ), font, font_size ).x > metrics.x ) {
			metrics = Context::Get().GetEngine().GetTextMetrics( m_entries.at( i ), font, font_size );
		}
	}
	metrics.y = Context::Get().GetEngine().GetLineHeight( font, font_size );

	sf::Vector2f requisition(
		metrics.x + 2 * GetBorderWidth() + 2 * padding + metrics.y,
		metrics.y + 2 * GetBorderWidth() + 2 * padding
	);

	return requisition;
}

const std::string& ComboBox::GetName() const {
	static const std::string name( "ComboBox" );
	return name;
}

}
