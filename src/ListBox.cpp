#include <cmath>
#include <SFGUI/ListBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>

namespace sfg {

ListBox::ListBox( std::size_t num_entries, float width ) :
  m_num_entries( num_entries ),
  m_width( width ),
	m_entries(),
	m_display_start( 0 ),
	m_active_index( -1 ),
	m_hover_index( -1 )
{
	OnFocusChange.Connect( &ListBox::HandleFocusChange, this );
	OnStateChange.Connect( &ListBox::HandleStateChange, this );
	OnMouseMove.Connect( &ListBox::HandleMouseMove, this );
	OnMouseLeave.Connect( &ListBox::HandleMouseLeave, this );
	OnMouseButtonClick.Connect( &ListBox::HandleMouseButtonClick, this );
}

ListBox::Ptr ListBox::Create( std::size_t num_entries, float width ) {
	ListBox::Ptr  ptr( new ListBox( num_entries, width ) );

	return ptr;
}

ListBox::u32string ListBox::GetEntry( std::size_t index ) {
  if( index < GetNumEntries() ) {
    return m_entries[index];
  }

  return ListBox::u32string();
}

ListBox::u32string ListBox::GetDisplayedEntry( std::size_t index ) {
  if( m_display_start + index < GetNumEntries() ) {
    return m_entries[m_display_start + index];
  }

  return ListBox::u32string();
}

void ListBox::AddEntry( const sf::String& entry ) {
  m_entries.push_back( ListBox::u32string( entry.GetData() ) );
}

void ListBox::RemoveEntry( std::size_t index ) {
  for( std::deque<u32string>::iterator i = m_entries.begin(); i != m_entries.end(); i++ ) {
    if( index == 0 ) {
      m_entries.erase( i );
      return;
    }
    index--;
  }
}

bool ListBox::IsHovered( std::size_t index ) const {
  if( m_hover_index < 0 ) {
    return false;
  }

  return index == (m_display_start + m_hover_index);
}

bool ListBox::IsSelected( std::size_t index ) const {
  if( m_active_index < 0 ) {
    return false;
  }

  return index == (m_display_start + m_active_index);
}

sf::Drawable* ListBox::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetEngine().CreateListBoxDrawable( boost::shared_dynamic_cast<ListBox>( shared_from_this() ), target );
}

void ListBox::HandleStateChange( Widget::Ptr /*widget*/, State /*oldstate*/ ) {
	Invalidate();
}

void ListBox::HandleMouseMove( Widget::Ptr /*widget*/, int /*x*/, int y ) {
  const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "ListBox.FontSize", shared_from_this() ) );
	float entry_spacing( Context::Get().GetEngine().GetProperty<float>( "ListBox.EntrySpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.BorderWidth", shared_from_this() ) );

	float text_height = Context::Get().GetEngine().GetTextMetrics(
		L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", font, font_size ).y;

  m_hover_index = (-1);

  for( std::size_t i = 0; i < m_num_entries; i++ ) {
    if( (float)y >= GetAllocation().Top + border_width + (float)(i) * (text_height + entry_spacing) &&
        (float)y < GetAllocation().Top + border_width + (float)(i+1) * (text_height + entry_spacing) ) {
      m_hover_index = m_display_start + i;
      Invalidate();
      break;
    }
  }
}

void ListBox::HandleMouseLeave( Widget::Ptr /*widget*/, int /*x*/, int y ) {
  m_hover_index = (-1);
  Invalidate();
}

bool ListBox::HandleMouseButtonClick( Widget::Ptr /*widget*/, int x, int /*y*/, sf::Mouse::Button /*button*/ ) {
  if( m_active_index != m_hover_index ) {
    m_active_index = m_hover_index;
  }
  else {
    m_active_index = (-1);
  }

	GrabFocus();
	Invalidate();

	return true;
}

void ListBox::HandleFocusChange( Widget::Ptr /*widget*/ ) {

}

sf::Vector2f ListBox::GetRequisitionImpl() const {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "ListBox.FontSize", shared_from_this() ) );
	float entry_spacing( Context::Get().GetEngine().GetProperty<float>( "ListBox.EntrySpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.BorderWidth", shared_from_this() ) );

	sf::Vector2f metrics = Context::Get().GetEngine().GetTextMetrics(
		L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", font, font_size );
	metrics.x = m_width + 2 * border_width;
  metrics.y *= m_num_entries;

  if( m_num_entries > 1 ) {
    metrics.y += ( entry_spacing * m_num_entries - 1 );
  }

  metrics.y += 2 * border_width;

	return metrics;
}

}


