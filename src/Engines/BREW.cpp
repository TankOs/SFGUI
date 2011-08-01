#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/TextBox.hpp>
#include <SFGUI/ListBox.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sfg {
namespace eng {

BREW::BREW() :
	Engine()
{
	// Set defaults.
	SetProperty<float>( "Window.Title.Height", 20.f );
	SetProperty<unsigned int>( "Window.Title.FontSize", 14 );
	SetProperty<sf::Color>( "Window.Title.BackgroundColor", sf::Color( 0x99, 0x99, 0x99 ) );
	SetProperty<sf::Color>( "Window.BackgroundColor", sf::Color( 0x88, 0x88, 0x88 ) );
	SetProperty<float>( "Window.BorderWidth", 2.f );
	SetProperty<sf::Color>( "Window.LightBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );
	SetProperty<sf::Color>( "Window.DarkBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty<float>( "Window.ShadowDistance", 2.f );
	SetProperty<sf::Uint8>( "Window.ShadowAlpha", 100 );

	SetProperty<sf::Color>( "Button.Normal.LightBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );
	SetProperty<sf::Color>( "Button.Normal.DarkBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty<sf::Color>( "Button.Normal.BackgroundColor", sf::Color( 0x99, 0x99, 0x99 ) );
	SetProperty<sf::Color>( "Button.Normal.TextColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	SetProperty<float>( "Button.Normal.BorderWidth", 1.f );
	SetProperty<sf::Color>( "Button.Prelight.BackgroundColor", sf::Color( 0xAA, 0xAA, 0xAA ) );
	SetProperty<sf::Color>( "Button.Prelight.TextColor", sf::Color( 0x00, 0x00, 0x00 ) );
	SetProperty<sf::Color>( "Button.Active.BackgroundColor", sf::Color( 0x77, 0x77, 0x77 ) );
	SetProperty<sf::Color>( "Button.Active.TextColor", sf::Color( 0x00, 0x00, 0x00 ) );
	SetProperty<sf::Color>( "Button.Active.LightBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty<sf::Color>( "Button.Active.DarkBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );

	SetProperty<std::string>( "Label.Font", "" );
	SetProperty<unsigned int>( "Label.FontSize", 12 );

	SetProperty<sf::Color>( "TextBox.Normal.LightBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );
	SetProperty<sf::Color>( "TextBox.Normal.DarkBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty<sf::Color>( "TextBox.Normal.BackgroundColor", sf::Color( 0x99, 0x99, 0x99 ) );
	SetProperty<sf::Color>( "TextBox.Normal.TextColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	SetProperty<float>( "TextBox.Normal.TextPadding", 2.f );
	SetProperty<sf::Color>( "TextBox.Normal.CursorColor", sf::Color( 0x00, 0x00, 0x00 ) );
	SetProperty<float>( "TextBox.Normal.CursorThickness", 1.f );
	SetProperty<float>( "TextBox.Normal.BorderWidth", 1.f );
	SetProperty<std::string>( "TextBox.Font", "" );
	SetProperty<unsigned int>( "TextBox.FontSize", 12 );

	SetProperty<sf::Color>( "ListBox.LightBorderColor", sf::Color( 0xCC, 0xCC, 0xCC ) );
	SetProperty<sf::Color>( "ListBox.DarkBorderColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty<sf::Color>( "ListBox.Normal.BackgroundColor", sf::Color( 0x77, 0x77, 0x77 ) );
	SetProperty<sf::Color>( "ListBox.Hover.BackgroundColor", sf::Color( 0x66, 0x66, 0x66 ) );
	SetProperty<sf::Color>( "ListBox.Selected.BackgroundColor", sf::Color( 0xEE, 0xEE, 0xEE ) );
	SetProperty<sf::Color>( "ListBox.Normal.TextColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	SetProperty<sf::Color>( "ListBox.Hover.TextColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	SetProperty<sf::Color>( "ListBox.Selected.TextColor", sf::Color( 0x33, 0x33, 0x33 ) );
	SetProperty<sf::Color>( "ListBox.Button.Background.Normal", sf::Color( 0xAA, 0xAA, 0xAA ) );
	SetProperty<sf::Color>( "ListBox.Button.Background.Active", sf::Color( 0x33, 0x33, 0x33 ) );
	SetProperty<sf::Color>( "ListBox.Button.Color.Normal", sf::Color( 0x33, 0x33, 0x33 ) );
	SetProperty<sf::Color>( "ListBox.Button.Color.Active", sf::Color( 0xAA, 0xAA, 0xAA ) );
	SetProperty<sf::Color>( "ListBox.Slider.Background", sf::Color( 0x80, 0x80, 0x80 ) );
	SetProperty<sf::Color>( "ListBox.Slider.Color", sf::Color( 0x95, 0x95, 0x95 ) );
	SetProperty<std::string>( "ListBox.Font", "" );
	SetProperty<unsigned int>( "ListBox.FontSize", 12 );
	SetProperty<float>( "ListBox.BorderWidth", 1.f );
	SetProperty<float>( "ListBox.TextPadding", 2.f );
	SetProperty<float>( "ListBox.EntrySpacing", 2.f );
	SetProperty<float>( "ListBox.ScrollbarWidth", 12.f );
	SetProperty<float>( "ListBox.ScrollSpeed", 3.f );

	// Register property types.
	RegisterProperty( "Button.Normal.BackgroundColor", Color );
	RegisterProperty( "Window.Title.FontSize", UnsignedInteger );
}

sf::Drawable* BREW::CreateWindowDrawable( boost::shared_ptr<Window> window, const sf::RenderTarget& /*target*/ ) const {
	RenderQueue* queue( new RenderQueue );
	sf::Color background_color( GetProperty<sf::Color>( "Window.BackgroundColor", window ) );
	sf::Color border_color_light( GetProperty<sf::Color>( "Window.LightBorderColor", window ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "Window.DarkBorderColor", window ) );
	sf::Color title_background_color( GetProperty<sf::Color>( "Window.Title.BackgroundColor", window ) );
	float border_width( GetProperty<float>( "Window.BorderWidth", window ) );
	float title_size( GetProperty<float>( "Window.Title.Height", window ) );
	float shadow_distance( GetProperty<float>( "Window.ShadowDistance", window ) );
	sf::Uint8 shadow_alpha( GetProperty<sf::Uint8>( "Window.ShadowAlpha", window ) );
	unsigned int title_font_size( GetProperty<unsigned int>( "Window.Title.FontSize", window ) );

	if( window->HasStyle( Window::Background ) ) {
		// Shadow.
		if( shadow_distance > 0.f ) {
			sf::Color  shadow_color( 0, 0, 0, shadow_alpha );

			queue->Add( new sf::Shape( sf::Shape::Rectangle( window->GetAllocation().Width + .1f, shadow_distance + .1f, shadow_distance, window->GetAllocation().Height - shadow_distance, shadow_color ) ) ); // Right.
			queue->Add( new sf::Shape( sf::Shape::Rectangle( shadow_distance + .1f, window->GetAllocation().Height + .1f, window->GetAllocation().Width, shadow_distance, shadow_color ) ) ); // Bottom.
		}

		if( border_width > 0 ) {
			queue->Add( CreateBorder( window->GetAllocation(), border_width, border_color_light, border_color_dark ) );
		}

		sf::Shape*  background(
			new sf::Shape(
				sf::Shape::Rectangle(
					border_width + .1f,
					border_width + .1f,
					window->GetAllocation().Width - 2 * border_width,
					window->GetAllocation().Height - 2 * border_width,
					background_color
				)
			)
		);

		queue->Add( background );
	}

	if( !window->HasStyle( Window::Titlebar ) ) {
		title_size = 0;
	}

	if( title_size > 0 ) {
		sf::Shape*  title(
			new sf::Shape(
				sf::Shape::Rectangle(
					border_width + .1f,
					border_width + .1f,
					window->GetAllocation().Width - 2 * border_width,
					title_size,
					title_background_color
				)
			)
		);

		sf::Text*  title_text(
			new sf::Text(
				window->GetTitle(),
				sf::Font::GetDefaultFont(),
				title_font_size
			)
		);

		// Calculate title text position.
		sf::Vector2f  title_position(
			std::floor( border_width + 5.f + .5f ),
			std::floor( border_width + ((title_size / 2.f) - (static_cast<float>( title_font_size ) / 2.f)) + .5f )
		);

		title_text->SetPosition( title_position );
		title_text->SetColor( sf::Color( 0, 0, 0 ) );

		queue->Add( title );
		queue->Add( title_text );
	}


	return queue;
}

RenderQueue* BREW::CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color ) {
	RenderQueue* queue( new RenderQueue );

	queue->Add( new sf::Shape( sf::Shape::Rectangle( rect.Width - border_width + .1f, .1f, border_width, rect.Height, dark_color ) ) ); // Right.
	queue->Add( new sf::Shape( sf::Shape::Rectangle( .1f, rect.Height - border_width + .1f, rect.Width, border_width, dark_color ) ) ); // Bottom.

	for( float delta = 0.f; delta < border_width; delta += 1.f ) {
		queue->Add( new sf::Shape( sf::Shape::Line( .1f, delta + .1f, rect.Width - delta, delta, 1.f, light_color ) ) ); // Top.
		queue->Add( new sf::Shape( sf::Shape::Line( delta + .1f, .1f, delta, rect.Height - delta, 1.f, light_color ) ) ); // Left.
	}
	
	return queue;
}

sf::Drawable* BREW::CreateButtonDrawable( boost::shared_ptr<Button> button, const sf::RenderTarget& /*target*/ ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "Button.Normal.LightBorderColor", button ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "Button.Normal.DarkBorderColor", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "Button.Normal.BackgroundColor", button ) );
	sf::Color text_color( GetProperty<sf::Color>( "Button.Normal.TextColor", button ) );
	float border_width( GetProperty<float>( "Button.Normal.BorderWidth", button ) );

	if( button->GetState() == Widget::Prelight ) {
		background_color = GetProperty<sf::Color>( "Button.Prelight.BackgroundColor", button );
		text_color = GetProperty<sf::Color>( "Button.Prelight.TextColor", button );
	}
	else if( button->GetState() == Widget::Active ) {
		background_color = GetProperty<sf::Color>( "Button.Active.BackgroundColor", button );
		text_color = GetProperty<sf::Color>( "Button.Active.TextColor", button );
		border_color_light = GetProperty<sf::Color>( "Button.Active.LightBorderColor", button );
		border_color_dark = GetProperty<sf::Color>( "Button.Active.DarkBorderColor", button );
	}

	RenderQueue*  queue( new RenderQueue );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f,
				0.f,
				button->GetAllocation().Width,
				button->GetAllocation().Height,
				background_color
			)
		)
	);

	queue->Add( CreateBorder( button->GetAllocation(), border_width, border_color_light, border_color_dark ) );

	return queue;
}

sf::Drawable* BREW::CreateLabelDrawable( boost::shared_ptr<Label> label, const sf::RenderTarget& /*target*/ ) const {
	const sf::Font& font( LoadFontFromFile( GetProperty<std::string>( "Label.Font", label ) ) );
	const unsigned int font_size( GetProperty<unsigned int>( "Label.FontSize", label ) );
	const sf::Color font_color( GetProperty<sf::Color>( "Label.TextColor", label ) );

	sf::Text*  vis_label( new sf::Text( label->GetText(), font, font_size ) );
	vis_label->SetColor( font_color );

	// Calculate alignment.
	sf::Vector2f  avail_space( label->GetAllocation().Width - label->GetRequisition().x, label->GetAllocation().Height - label->GetRequisition().y );
	sf::Vector2f  position( avail_space.x * label->GetAlignment().x, avail_space.y * label->GetAlignment().y );

	vis_label->SetPosition( std::floor( position.x + .5f ), std::floor( position.y + .5f ) );

	RenderQueue*  queue( new RenderQueue );
	queue->Add( vis_label );

	return queue;
}

sf::Drawable* BREW::CreateTextBoxDrawable( boost::shared_ptr<TextBox> textbox, const sf::RenderTarget& /*target*/ ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "TextBox.Normal.LightBorderColor", textbox ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "TextBox.Normal.DarkBorderColor", textbox ) );
	sf::Color background_color( GetProperty<sf::Color>( "TextBox.Normal.BackgroundColor", textbox ) );
	sf::Color text_color( GetProperty<sf::Color>( "TextBox.Normal.TextColor", textbox ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "TextBox.Normal.CursorColor", textbox ) );
	float text_padding( GetProperty<float>( "TextBox.Normal.TextPadding", textbox ) );
	float cursor_thickness( GetProperty<float>( "TextBox.Normal.CursorThickness", textbox ) );
	float border_width( GetProperty<float>( "TextBox.Normal.BorderWidth", textbox ) );
	const sf::Font&  font( LoadFontFromFile( GetProperty<std::string>( "TextBox.Font", textbox ) ) );
	const unsigned int&  font_size( GetProperty<unsigned int>( "TextBox.FontSize", textbox ) );

	RenderQueue* queue( new RenderQueue );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f, 0.f,
				textbox->GetAllocation().Width,
				font_size * 1.4f,
				background_color
			)
		)
	);

	queue->Add( CreateBorder( textbox->GetAllocation(), border_width, border_color_dark, border_color_light) );
	
	sf::Text*  vis_label( new sf::Text( textbox->GetVisibleText(), font, font_size ) );
	vis_label->SetColor( text_color );
	vis_label->SetPosition( text_padding, 0.f );

	queue->Add( vis_label );
	
	// Draw cursor if it is currently visible
	if( textbox->GetCursorStatus() ) {
		sf::Sprite*  vis_cursor( new sf::Sprite() );
		vis_cursor->SetPosition( GetTextMetrics( textbox->GetVisibleLeft(), font, font_size ).x + text_padding, 2.f );
		vis_cursor->Resize( cursor_thickness, textbox->GetAllocation().Height - 4.f );
		vis_cursor->SetColor( cursor_color );
		queue->Add( vis_cursor );
	}
	
	return queue;
}

sf::Drawable* BREW::CreateListBoxDrawable( boost::shared_ptr<ListBox> listbox, const sf::RenderTarget& /*target*/ ) const {
  sf::Color border_color_light( GetProperty<sf::Color>( "ListBox.LightBorderColor", listbox ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "ListBox.DarkBorderColor", listbox ) );
	sf::Color background_color( GetProperty<sf::Color>( "ListBox.Normal.BackgroundColor", listbox ) );
	sf::Color hover_color( GetProperty<sf::Color>( "ListBox.Hover.BackgroundColor", listbox ) );
	sf::Color selected_color( GetProperty<sf::Color>( "ListBox.Selected.BackgroundColor", listbox ) );
	sf::Color font_color( GetProperty<sf::Color>( "ListBox.Normal.TextColor", listbox ) );
	sf::Color font_color_selected( GetProperty<sf::Color>( "ListBox.Selected.TextColor", listbox ) );
	sf::Color button_background_normal( GetProperty<sf::Color>( "ListBox.Button.Background.Normal", listbox ) );
	sf::Color button_background_active( GetProperty<sf::Color>( "ListBox.Button.Background.Active", listbox ) );
	sf::Color button_color_normal( GetProperty<sf::Color>( "ListBox.Button.Color.Normal", listbox ) );
	sf::Color button_color_active( GetProperty<sf::Color>( "ListBox.Button.Color.Active", listbox ) );
	sf::Color slider_background( GetProperty<sf::Color>( "ListBox.Slider.Background", listbox ) );
	sf::Color slider_color( GetProperty<sf::Color>( "ListBox.Slider.Color", listbox ) );
	float border_width( GetProperty<float>( "ListBox.BorderWidth", listbox ) );
	float entry_spacing( GetProperty<float>( "ListBox.EntrySpacing", listbox ) );
	float scrollbar_width( GetProperty<float>( "ListBox.ScrollbarWidth", listbox ) );
	float text_padding( GetProperty<float>( "ListBox.TextPadding", listbox ) );
	unsigned int font_size( GetProperty<unsigned int>( "ListBox.FontSize", listbox ) );
	const sf::Font& font( LoadFontFromFile( GetProperty<std::string>( "ListBox.Font", listbox ) ) );

  float text_height = GetTextMetrics( L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", font, font_size ).y;

  RenderQueue*  queue( new RenderQueue );

  // Background of the entries area
  queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				border_width, border_width,
				listbox->GetAllocation().Width - 2 * border_width - scrollbar_width,
				listbox->GetAllocation().Height - 2 * border_width,
				background_color
			)
		)
	);

  {
    sf::Color button_background = button_background_normal;
    sf::Color button_color = button_color_normal;
    if( listbox->IsUpPressed() ) {
      button_background = button_background_active;
      button_color = button_color_active;
    }
    // Scrollbar button up
    queue->Add(
      new sf::Shape(
        sf::Shape::Rectangle(
          listbox->GetAllocation().Width - border_width - scrollbar_width,
          border_width,
          scrollbar_width,
          scrollbar_width,
          button_background
        )
      )
    );

    sf::Shape* button_up = new sf::Shape();
    button_up->AddPoint(
      listbox->GetAllocation().Width - border_width - scrollbar_width + scrollbar_width / 5,
      border_width + scrollbar_width - scrollbar_width / 3,
      button_color
    );
    button_up->AddPoint(
      listbox->GetAllocation().Width - border_width - scrollbar_width / 5,
      border_width + scrollbar_width - scrollbar_width / 3,
      button_color
    );
    button_up->AddPoint(
      listbox->GetAllocation().Width - border_width - scrollbar_width / 2,
      border_width + scrollbar_width / 3,
      button_color
    );

    queue->Add(button_up);
  }

  {
    sf::Color button_background = button_background_normal;
    sf::Color button_color = button_color_normal;
    if( listbox->IsDownPressed() ) {
      button_background = button_background_active;
      button_color = button_color_active;
    }
    // Scrollbar button down
    queue->Add(
      new sf::Shape(
        sf::Shape::Rectangle(
          listbox->GetAllocation().Width - border_width - scrollbar_width,
          listbox->GetAllocation().Height - border_width - scrollbar_width,
          scrollbar_width,
          scrollbar_width,
          button_background
        )
      )
    );

    sf::Shape* button_down = new sf::Shape();
    button_down->AddPoint(
      listbox->GetAllocation().Width - border_width - scrollbar_width / 5,
      listbox->GetAllocation().Height - border_width - scrollbar_width + scrollbar_width / 3,
      button_color
    );
    button_down->AddPoint(
      listbox->GetAllocation().Width - border_width - scrollbar_width + scrollbar_width / 5,
      listbox->GetAllocation().Height - border_width - scrollbar_width + scrollbar_width / 3,
      button_color
    );
    button_down->AddPoint(
      listbox->GetAllocation().Width - border_width - scrollbar_width / 2,
      listbox->GetAllocation().Height - border_width - scrollbar_width / 3,
      button_color
    );

    queue->Add(button_down);
  }

  float slider_area_height = listbox->GetAllocation().Height - 2 * ( border_width + scrollbar_width );

  // Slider area
  queue->Add(
    new sf::Shape(
      sf::Shape::Rectangle(
        listbox->GetAllocation().Width - border_width - scrollbar_width,
        border_width + scrollbar_width,
        scrollbar_width,
        slider_area_height,
        slider_background
      )
    )
  );

  std::size_t displayed_entries = listbox->GetMaxDisplayedEntries();
  std::size_t total_entries = listbox->GetNumEntries();
  std::size_t start_entry = listbox->GetDisplayStart();

  float slider_size = slider_area_height * (float)displayed_entries / (float)total_entries;
  float slider_position = ( (float)start_entry ) / ( (float)total_entries - (float)displayed_entries );

  if( displayed_entries >= total_entries ) {
    slider_size = slider_area_height;
    slider_position = 0.f;
  }

  float slider_drag_area = slider_area_height - slider_size;
  float slider_start_position = slider_drag_area * slider_position;

  // Slider
  queue->Add(
    new sf::Shape(
      sf::Shape::Rectangle(
        listbox->GetAllocation().Width - border_width - scrollbar_width,
        border_width + scrollbar_width + slider_start_position,
        scrollbar_width,
        slider_size,
        slider_color
      )
    )
  );

  // Border around entries and scrollbar
  queue->Add( CreateBorder( listbox->GetAllocation(), border_width, border_color_dark, border_color_light) );

  for( std::size_t i = 0; i < listbox->GetMaxDisplayedEntries() && i < listbox->GetNumEntries(); i++ ) {
    sf::Text*  vis_entry( new sf::Text( listbox->GetDisplayedEntry( i ), font, font_size ) );
    vis_entry->SetColor( font_color );
    vis_entry->SetPosition( border_width + text_padding, border_width + (text_height + entry_spacing) * i );

    if( listbox->IsSelected(i) ) {
      // Background around a selected entry
      queue->Add(
        new sf::Shape(
          sf::Shape::Rectangle(
            border_width,
            border_width + (text_height + entry_spacing) * i,
            listbox->GetAllocation().Width - 2 * border_width - scrollbar_width,
            text_height,
            selected_color
          )
        )
      );
      vis_entry->SetColor( font_color_selected );
    }
    else if( listbox->IsHovered(i) ) {
      // Background around a hovered entry
      queue->Add(
        new sf::Shape(
          sf::Shape::Rectangle(
            border_width,
            border_width + (text_height + entry_spacing) * i,
            listbox->GetAllocation().Width - 2 * border_width - scrollbar_width,
            text_height,
            hover_color
          )
        )
      );
    }

    queue->Add( vis_entry );
  }

	return queue;
}

}
}
