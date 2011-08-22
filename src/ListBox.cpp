#include <SFGUI/ListBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <cmath>

namespace sfg {

ListBox::ListBox( std::size_t num_entries, float width ) :
  m_style( Default ),
  m_num_entries( num_entries ),
  m_width( width ),
	m_entries(),
	m_display_start( 0 ),
	m_active_index( -1 ),
	m_hover_index( -1 ),
	m_up_pressed( false ),
	m_down_pressed( false ),
	m_sliding( false ),
	m_scroll_timer(),
	m_slide_last_y( -1 )
{
	OnFocusChange.Connect( &ListBox::HandleFocusChange, this );
	OnStateChange.Connect( &ListBox::HandleStateChange, this );
	OnMouseMove.Connect( &ListBox::HandleMouseMove, this );
	OnMouseLeave.Connect( &ListBox::HandleMouseLeave, this );
	OnMouseButtonPress.Connect( &ListBox::HandleMouseButtonPress, this );
	OnMouseButtonRelease.Connect( &ListBox::HandleMouseButtonRelease, this );
	OnExpose.Connect( &ListBox::HandleExpose, this );
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

  if( HasStyle( AutoScroll ) ) {
    m_display_start = m_entries.size() - m_num_entries;
  }
  Invalidate();
}

void ListBox::AddEntry( const sf::String& entry, std::size_t pos ) {
  for( std::deque<u32string>::iterator i = m_entries.begin(); i != m_entries.end(); i++ ) {
    if( pos == 0 ) {
      m_entries.insert( i, ListBox::u32string( entry.GetData() ) );
      break;
    }

    pos--;
  }

  if( HasStyle( AutoScroll ) ) {
    m_display_start = m_entries.size() - m_num_entries;
  }
  Invalidate();
}

void ListBox::RemoveEntry( std::size_t index ) {
  for( std::deque<u32string>::iterator i = m_entries.begin(); i != m_entries.end(); i++ ) {
    if( index == 0 ) {
      m_entries.erase( i );
      Invalidate();
      return;
    }
    index--;
  }
}

bool ListBox::IsHovered( std::size_t index ) const {
  if( m_hover_index < 0 ) {
    return false;
  }

  return m_hover_index == ( m_display_start + index );
}

bool ListBox::IsSelected( std::size_t index ) const {
  if( m_active_index < 0 ) {
    return false;
  }

  return m_active_index == ( m_display_start + index );
}

void ListBox::SetStyle( int style ) {
	m_style = style;
	RequestSize();
	Invalidate();
}

int ListBox::GetStyle() const {
	return m_style;
}

bool ListBox::HasStyle( Style style ) const {
	return m_style & style;
}

sf::Drawable* ListBox::InvalidateImpl( const sf::RenderTarget& target ) {
	return Context::Get().GetEngine().CreateListBoxDrawable( boost::shared_dynamic_cast<ListBox>( shared_from_this() ), target );
}

void ListBox::HandleStateChange( Widget::Ptr /*widget*/, State /*oldstate*/ ) {
	Invalidate();
}

void ListBox::HandleMouseMove( Widget::Ptr /*widget*/, int x, int y ) {
  const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "ListBox.FontSize", shared_from_this() ) );
	float entry_spacing( Context::Get().GetEngine().GetProperty<float>( "ListBox.EntrySpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.BorderWidth", shared_from_this() ) );
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.ScrollbarWidth", shared_from_this() ) );

	float text_height = Context::Get().GetEngine().GetTextMetrics(
		L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", font, font_size ).y;

  m_hover_index = (-1);

  if( (float)x < GetAllocation().Left + GetAllocation().Width - border_width - scrollbar_width ) {
    // Mouse is hovering over an entry

    for( std::size_t i = 0; i < m_num_entries; i++ ) {
      if( (float)y >= GetAllocation().Top + border_width + (float)(i) * (text_height + entry_spacing) &&
          (float)y < GetAllocation().Top + border_width + (float)(i+1) * (text_height + entry_spacing) ) {
        m_hover_index = m_display_start + i;
        Invalidate();
        break;
      }
    }
  }
  else {
    // Mouse is hovering over scrollbar

  }

  if( !m_sliding || ( GetMaxDisplayedEntries() >= GetNumEntries() ) ) {
    return;
  }

  float slider_area_height = GetAllocation().Height - 2 * ( border_width + scrollbar_width );

  std::size_t displayed_entries = GetMaxDisplayedEntries();
  std::size_t total_entries = GetNumEntries();

  float slider_size = slider_area_height * (float)displayed_entries / (float)total_entries;
  float entry_segment = 1 / ( (float)total_entries - (float)displayed_entries );

  float slider_drag_area = slider_area_height - slider_size;
  float entry_height = slider_drag_area * entry_segment;

  float units = (float)( y - m_slide_last_y ) / entry_height;

  while( units > .5f && ( m_display_start < m_entries.size() - m_num_entries ) ) {
    m_display_start++;
    units -= 1.f;
    m_slide_last_y = y;
    Invalidate();
  }

  while( units < -.5f && ( m_display_start > 0 ) ) {
    m_display_start--;
    units += 1.f;
    m_slide_last_y = y;
    Invalidate();
  }

}

void ListBox::HandleMouseLeave( Widget::Ptr /*widget*/, int /*x*/, int y ) {
  m_hover_index = (-1);
  m_up_pressed = false;
	m_down_pressed = false;

  Invalidate();
}

bool ListBox::HandleMouseButtonPress( Widget::Ptr /*widget*/, int /*x*/, int y, sf::Mouse::Button /*button*/ ) {
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.BorderWidth", shared_from_this() ) );
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.ScrollbarWidth", shared_from_this() ) );

	m_up_pressed = false;
	m_down_pressed = false;
	m_sliding = false;
	m_slide_last_y = (-1);

  if( m_hover_index >= 0 ) {
    // Mouse clicked on entry
    if( m_active_index != m_hover_index ) {
      m_active_index = m_hover_index;
      SetState( Selected );
    }
    else {
      m_active_index = (-1);
      SetState( Normal );
    }
  }
  else {

    float slider_area_height = GetAllocation().Height - 2 * ( border_width + scrollbar_width );

    std::size_t displayed_entries = GetMaxDisplayedEntries();
    std::size_t total_entries = GetNumEntries();
    std::size_t start_entry = GetDisplayStart();

    float slider_size = slider_area_height * (float)displayed_entries / (float)total_entries;
    float slider_position = ( (float)start_entry ) / ( (float)total_entries - (float)displayed_entries );

    float slider_drag_area = slider_area_height - slider_size;
    float slider_start_position = slider_drag_area * slider_position;

    // Mouse clicked on scrollbar
    if( ( (float)y >= GetAllocation().Top + border_width ) &&
        ( (float)y <  GetAllocation().Top + border_width + scrollbar_width ) ) {
      // Clicked on up button
      m_up_pressed = true;
    }
    else if( ( (float)y >= GetAllocation().Top + GetAllocation().Height - border_width - scrollbar_width ) &&
             ( (float)y <  GetAllocation().Top + GetAllocation().Height - border_width ) ) {
      // Clicked on down button
      m_down_pressed = true;
    }
    else if( ( (float)y >= GetAllocation().Top + border_width + scrollbar_width + slider_start_position ) &&
             ( (float)y <  GetAllocation().Top + border_width + scrollbar_width + slider_start_position + slider_size ) ) {
      // Clicked on slider
      m_sliding = true;
      m_slide_last_y = y;
    }
  }

	GrabFocus();

	return true;
}

bool ListBox::HandleMouseButtonRelease( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button /*button*/ ) {
	m_up_pressed = false;
	m_down_pressed = false;
	m_sliding = false;
	m_slide_last_y = (-1);

  Invalidate();

	return true;
}

void ListBox::HandleExpose( Widget::Ptr /*widget*/, sf::RenderTarget& /*target*/ ) {
	float scroll_speed( Context::Get().GetEngine().GetProperty<float>( "ListBox.ScrollSpeed", shared_from_this() ) );

  if( GetMaxDisplayedEntries() >= GetNumEntries() ) {
    return;
  }

  if( ( m_up_pressed || m_down_pressed ) && ( (float)( m_scroll_timer.GetElapsedTime() ) > 1000.f / scroll_speed ) ) {
    if( m_up_pressed && ( m_display_start > 0 ) ) {
      m_display_start--;
    }

    if( m_down_pressed && ( m_display_start < m_entries.size() - m_num_entries ) ) {
      m_display_start++;
    }
    Invalidate();
    m_scroll_timer.Reset();
  }

}

void ListBox::HandleFocusChange( Widget::Ptr /*widget*/ ) {

}

sf::Vector2f ListBox::GetRequisitionImpl() const {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "Label.Font", shared_from_this() ) );
	const sf::Font& font( Context::Get().GetEngine().LoadFontFromFile( font_name ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "ListBox.FontSize", shared_from_this() ) );
	float entry_spacing( Context::Get().GetEngine().GetProperty<float>( "ListBox.EntrySpacing", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.BorderWidth", shared_from_this() ) );
	float scrollbar_width( Context::Get().GetEngine().GetProperty<float>( "ListBox.ScrollbarWidth", shared_from_this() ) );

	sf::Vector2f metrics = Context::Get().GetEngine().GetTextMetrics(
		L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", font, font_size );
	metrics.x = m_width + 2 * border_width + scrollbar_width;
  metrics.y *= m_num_entries;

  if( m_num_entries > 1 ) {
    metrics.y += ( entry_spacing * (m_num_entries - 1) );
  }

  metrics.y += 2 * border_width;

	return metrics;
}

}


