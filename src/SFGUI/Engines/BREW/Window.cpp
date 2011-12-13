#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Window.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateWindowDrawable( SharedPtr<const Window> window ) const {
	RenderQueue* queue( new RenderQueue );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", window ) );
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", window ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", window ) );
	sf::Color title_background_color( GetProperty<sf::Color>( "TitleBackgroundColor", window ) );
	sf::Color title_text_color( GetProperty<sf::Color>( "Color", window ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", window ) );
	float border_width( GetProperty<float>( "BorderWidth", window ) );
	float title_padding( GetProperty<float>( "TitlePadding", window ) );
	float shadow_distance( GetProperty<float>( "ShadowDistance", window ) );
	float handle_size( GetProperty<float>( "HandleSize", window ) );
	sf::Uint8 shadow_alpha( GetProperty<sf::Uint8>( "ShadowAlpha", window ) );
	unsigned int title_font_size( GetProperty<unsigned int>( "FontSize", window ) );
	const sf::Font& title_font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", window ) ) );
	float title_size( GetLineHeight( title_font, title_font_size ) + 2 * title_padding );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	if( window->HasStyle( Window::Background ) ) {
		// Shadow.
		if( shadow_distance > 0.f ) {
			sf::Color shadow_color( 0, 0, 0, shadow_alpha );

			queue->Add( new sf::Shape( sf::Shape::Rectangle( window->GetAllocation().Width + .1f, shadow_distance + .1f, shadow_distance, window->GetAllocation().Height - shadow_distance, shadow_color ) ) ); // Right.
			queue->Add( new sf::Shape( sf::Shape::Rectangle( shadow_distance + .1f, window->GetAllocation().Height + .1f, window->GetAllocation().Width, shadow_distance, shadow_color ) ) ); // Bottom.
		}

		if( border_width > 0 ) {
			queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, window->GetAllocation().Width, window->GetAllocation().Height ), border_width, border_color_light, border_color_dark ) );
		}

		sf::Shape* background(
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

	if( window->HasStyle( Window::Resize ) ) {
		sf::Shape* handle_shape( new sf::Shape );
		handle_shape->AddPoint( window->GetAllocation().Width - 1.f, window->GetAllocation().Height - 1.f - handle_size, title_background_color );
		handle_shape->AddPoint( window->GetAllocation().Width - 1.f - handle_size, window->GetAllocation().Height - 1.f, title_background_color );
		handle_shape->AddPoint( window->GetAllocation().Width - 1.f, window->GetAllocation().Height - 1.f, title_background_color );
		handle_shape->EnableFill( true );
		queue->Add( handle_shape );
	}


	if( !window->HasStyle( Window::Titlebar ) ) {
		title_size = 0;
	}

	if( title_size > 0 ) {
		sf::Shape* title(
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

		// Find out visible text, count in "...".
		float avail_width( window->GetAllocation().Width - 2.f * border_width - 2.f * title_padding );

		sf::Text* title_text(
			new sf::Text(
				window->GetTitle(),
				title_font,
				title_font_size
			)
		);

		if( title_text->GetRect().Width > avail_width ) {
			sf::Text dots( "...", title_font, title_font_size );
			const sf::String& title_string( window->GetTitle() );
			sf::String visible_title;

			avail_width = window->GetAllocation().Width - 2.f * border_width - 2.f * title_padding - dots.GetRect().Width;

			for( std::size_t ch_index = 0; ch_index < title_string.GetSize(); ++ch_index ) {
				avail_width -= static_cast<float>( title_font.GetGlyph( title_string[ch_index], title_font_size, false ).Advance );

				if( avail_width < 0.f ) {
					visible_title += "...";
					break;
				}

				visible_title += title_string[ch_index];
			}

			title_text->SetString( visible_title );
		}

		// Calculate title text position.
		sf::Vector2f title_position(
			std::floor( border_width + title_padding + .5f ),
			std::floor( border_width + ((title_size / 2.f) - (static_cast<float>( title_font_size ) / 2.f)) + .5f )
		);

		title_text->SetPosition( title_position );
		title_text->SetColor( title_text_color );

		queue->Add( title );
		queue->Add( title_text );
	}

	return queue;
}

}
}
