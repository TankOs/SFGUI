#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/CheckButton.hpp>
#include <SFGUI/ProgressBar.hpp>
#include <SFGUI/Separator.hpp>
#include <SFGUI/Frame.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Spinner.hpp>

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sfg {
namespace eng {

BREW::BREW() :
	Engine()
{
	// Set defaults.
	SetProperty( "*", "Color", sf::Color( 0xc6, 0xcb, 0xc4 ) );
	SetProperty( "*", "FontSize", 12 );
	SetProperty( "*", "FontName", "" ); // Uses SFML's default font when empty.
	SetProperty( "*", "BackgroundColor", sf::Color( 0x46, 0x46, 0x46 ) );
	SetProperty( "*", "BorderColor", sf::Color( 0x66, 0x66, 0x66 ) );
	SetProperty( "*", "BorderColorShift", 0x20 );
	SetProperty( "*", "BorderWidth", 1.f );
	SetProperty( "*", "Padding", 5.f );
	SetProperty( "*", "Thickness", 2.f );

	// Window-specific.
	SetProperty( "Window", "TitleHeight", 25.f );
	SetProperty( "Window", "TitleBackgroundColor", sf::Color( 0x5a, 0x6a, 0x50 ) );
	SetProperty( "Window", "TitlePadding", 5.f );
	SetProperty( "Window", "HandleSize", 10.f );
	SetProperty( "Window", "ShadowDistance", 3.f );
	SetProperty( "Window", "ShadowAlpha", 100.f );

	// Button-specific.
	SetProperty( "Button", "BackgroundColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "Button", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "Button", "Spacing", 5.f );
	SetProperty( "Button:PRELIGHT", "BackgroundColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "Button:PRELIGHT", "Color", sf::Color::White );
	SetProperty( "Button:ACTIVE", "BackgroundColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "Button:ACTIVE", "Color", sf::Color::Black );

	// ToggleButton-specific.
	SetProperty( "ToggleButton", "BackgroundColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ToggleButton", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ToggleButton", "Padding", 5.f );
	SetProperty( "ToggleButton:PRELIGHT", "BackgroundColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "ToggleButton:PRELIGHT", "Color", sf::Color::White );
	SetProperty( "ToggleButton:ACTIVE", "BackgroundColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "ToggleButton:ACTIVE", "Color", sf::Color::Black );

	// CheckButton-specific.
	SetProperty( "CheckButton", "Spacing", 5.f );
	SetProperty( "CheckButton", "BoxSize", 14.f );
	SetProperty( "CheckButton", "CheckSize", 6.f );
	SetProperty( "CheckButton", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "CheckButton", "BackgroundColor", sf::Color( 0x36, 0x36, 0x36 ) );
	SetProperty( "CheckButton", "CheckColor", sf::Color( 0x9e, 0x9e, 0x9e ) );
	SetProperty( "CheckButton:PRELIGHT", "BackgroundColor", sf::Color( 0x46, 0x46, 0x46 ) );
	SetProperty( "CheckButton:ACTIVE", "BackgroundColor", sf::Color( 0x56, 0x56, 0x56 ) );

	// Entry-specific.
	SetProperty( "Entry", "BackgroundColor", sf::Color( 0x5e, 0x5e, 0x5e ) );
	SetProperty( "Entry", "Color", sf::Color::White );

	// Scale-specific.
	SetProperty( "Scale", "SliderColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scale", "SliderLength", 15.f );
	SetProperty( "Scale", "TroughColor", sf::Color( 0x70, 0x70, 0x70 ) );
	SetProperty( "Scale", "TroughWidth", 5.f );

	// Scrollbar-specific.
	SetProperty( "Scrollbar", "SliderColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scrollbar", "TroughColor", sf::Color( 0x70, 0x70, 0x70 ) );
	SetProperty( "Scrollbar", "StepperBackgroundColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scrollbar", "StepperArrowColor", sf::Color( 0xd9, 0xdc, 0xd5 ) );
	SetProperty( "Scrollbar", "StepperSpeed", 10.f );
	SetProperty( "Scrollbar", "StepperRepeatDelay", 300 );
	SetProperty( "Scrollbar", "SliderMinimumLength", 15.f );

	// ScrolledWindow-specific.
	SetProperty( "ScrolledWindow", "ScrollbarWidth", 20.f );
	SetProperty( "ScrolledWindow", "ScrollbarSpacing", 5.f );
	SetProperty( "ScrolledWindow", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );

	// ProgressBar-specific.
	SetProperty( "ProgressBar", "BackgroundColor", sf::Color( 0x36, 0x36, 0x36 ) );
	SetProperty( "ProgressBar", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "ProgressBar", "BarColor", sf::Color( 0x67, 0x89, 0xab ) );
	SetProperty( "ProgressBar", "BarBorderColor", sf::Color( 0x67, 0x89, 0xab ) );
	SetProperty( "ProgressBar", "BarBorderColorShift", 0x20 );
	SetProperty( "ProgressBar", "BarBorderWidth", 1.f );

	// Separator-specific.
	SetProperty( "Separator", "Color", sf::Color( 0x55, 0x57, 0x52 ) );

	// Frame-specific.
	SetProperty( "Frame", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "Frame", "Padding", 7.f );
	SetProperty( "Frame", "LabelPadding", 5.f );

	// Notebook-specific.
	SetProperty( "Notebook", "BorderColor", sf::Color( 0x50, 0x52, 0x4D ) );
	SetProperty( "Notebook", "BackgroundColor", sf::Color( 0x4C, 0x4C, 0x4C ) );
	SetProperty( "Notebook", "BackgroundColorDark", sf::Color( 0x42, 0x42, 0x42 ) );
	SetProperty( "Notebook", "BackgroundColorPrelight", sf::Color( 0x48, 0x48, 0x48 ) );

	// Spinner-specific.
	SetProperty( "Spinner", "CycleDuration", 800.f );
	SetProperty( "Spinner", "Steps", 13 );
	SetProperty( "Spinner", "Fade", 5.f );
	SetProperty( "Spinner", "StoppedAlpha", 47 );
}

RenderQueue* BREW::CreateWindowDrawable( SharedPtr<const Window> window ) const {
	RenderQueue* queue( new RenderQueue );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", window ) );
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", window ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", window ) );
	sf::Color title_background_color( GetProperty<sf::Color>( "TitleBackgroundColor", window ) );
	sf::Color title_text_color( GetProperty<sf::Color>( "Color", window ) );
	float border_width( GetProperty<float>( "BorderWidth", window ) );
	float title_size( GetProperty<float>( "TitleHeight", window ) );
	float title_padding( GetProperty<float>( "TitlePadding", window ) );
	float shadow_distance( GetProperty<float>( "ShadowDistance", window ) );
	float handle_size( GetProperty<float>( "HandleSize", window ) );
	sf::Uint8 shadow_alpha( GetProperty<sf::Uint8>( "ShadowAlpha", window ) );
	unsigned int title_font_size( GetProperty<unsigned int>( "FontSize", window ) );
	const sf::Font& title_font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", window ) ) );

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
		float avail_width( window->GetAllocation().Width - 2.f * border_width  - 2.f * title_padding );

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

			avail_width = window->GetAllocation().Width - 2.f * border_width  - 2.f * title_padding - dots.GetRect().Width;

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

RenderQueue* BREW::CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color ) {
	RenderQueue* queue( new RenderQueue );
	sf::FloatRect rounded_rect(
		std::floor( rect.Left + .5f ),
		std::floor( rect.Top + .5f ),
		std::floor( rect.Width + .5f ),
		std::floor( rect.Height + .5f )
	);

	border_width = std::floor( border_width + .5f );

	sf::Shape* right( new sf::Shape );
	right->EnableFill( true );
	right->EnableOutline( false );
	right->AddPoint( rect.Left + rect.Width, rect.Top, dark_color );
	right->AddPoint( rect.Left + rect.Width - border_width, rect.Top + border_width, dark_color );
	right->AddPoint( rect.Left + rect.Width - border_width, rect.Top + rect.Height - border_width, dark_color );
	right->AddPoint( rect.Left + rect.Width, rect.Top + rect.Height, dark_color );
	queue->Add( right );

	sf::Shape* bottom( new sf::Shape );
	bottom->EnableFill( true );
	bottom->EnableOutline( false );
	bottom->AddPoint( rect.Left + rect.Width - border_width, rect.Top + rect.Height - border_width, dark_color );
	bottom->AddPoint( rect.Left + border_width, rect.Top + rect.Height - border_width, dark_color );
	bottom->AddPoint( rect.Left, rect.Top + rect.Height, dark_color );
	bottom->AddPoint( rect.Left + rect.Width, rect.Top + rect.Height, dark_color );
	queue->Add( bottom );

	sf::Shape* left( new sf::Shape );
	left->EnableFill( true );
	left->EnableOutline( false );
	left->AddPoint( rect.Left, rect.Top, light_color );
	left->AddPoint( rect.Left, rect.Top + rect.Height, light_color );
	left->AddPoint( rect.Left + border_width, rect.Top + rect.Height - border_width, light_color );
	left->AddPoint( rect.Left + border_width, rect.Top + border_width, light_color );
	queue->Add( left );

	sf::Shape* top( new sf::Shape );
	top->EnableFill( true );
	top->EnableOutline( false );
	top->AddPoint( rect.Left, rect.Top, light_color );
	top->AddPoint( rect.Left + border_width, rect.Top + border_width, light_color );
	top->AddPoint( rect.Left + rect.Width - border_width, rect.Top + border_width, light_color );
	top->AddPoint( rect.Left + rect.Width, rect.Top, light_color );
	queue->Add( top );

	return queue;
}

RenderQueue* BREW::CreateButtonDrawable( SharedPtr<const Button> button ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", button ) );
	sf::Color border_color_dark( border_color_light );
	int border_color_shift( GetProperty<int>( "BorderColorShift", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", button ) );
	sf::Color color( GetProperty<sf::Color>( "Color", button ) );
	float border_width( GetProperty<float>( "BorderWidth", button ) );
	float spacing( GetProperty<float>( "Spacing", button ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

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

	if( button->GetState() != Button::ACTIVE ) {
		queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, button->GetAllocation().Width, button->GetAllocation().Height ), border_width, border_color_light, border_color_dark ) );
	}
	else {
		queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, button->GetAllocation().Width, button->GetAllocation().Height ), border_width, border_color_dark, border_color_light ) );
	}

	// Label.
	if( button->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( button->GetLabel(), font, font_size );
		metrics.y = GetLineHeight( font, font_size );

		sf::Text* text( new sf::Text( button->GetLabel(), font, font_size ) );
		float offset = ( button->GetState() == Button::ACTIVE ) ? border_width : 0.f;
		sfg::Widget::PtrConst child( button->GetChild() );

		if( !child ) {
			text->SetPosition(
				std::floor( button->GetAllocation().Width / 2.f - metrics.x / 2.f + .5f + offset ),
				std::floor( button->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f + offset )
			);
		}
		else {
			float width( button->GetAllocation().Width - spacing - child->GetAllocation().Width );

			text->SetPosition(
				std::floor( child->GetAllocation().Width + spacing + (width / 2.f - metrics.x / 2.f) + .5f + offset ),
				std::floor( button->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f + offset )
			);
		}

		text->SetColor( color );
		queue->Add( text );
	}

	return queue;
}

RenderQueue* BREW::CreateToggleButtonDrawable( SharedPtr<const ToggleButton> button ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", button ) );
	sf::Color border_color_dark( border_color_light );
	int border_color_shift( GetProperty<int>( "BorderColorShift", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", button ) );
	sf::Color color( GetProperty<sf::Color>( "Color", button ) );
	float border_width( GetProperty<float>( "BorderWidth", button ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

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

	if( button->GetState() != Button::ACTIVE && !button->IsActive() ) {
		queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, button->GetAllocation().Width, button->GetAllocation().Height ), border_width, border_color_light, border_color_dark ) );
	}
	else {
		queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, button->GetAllocation().Width, button->GetAllocation().Height ), border_width, border_color_dark, border_color_light ) );
	}

	// Label.
	if( button->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( button->GetLabel(), font, font_size );
		metrics.y = GetLineHeight( font, font_size );

		sf::Text* text( new sf::Text( button->GetLabel(), font, font_size ) );
		float offset = ( ( button->GetState() == Button::ACTIVE ) || button->IsActive() ) ? border_width : 0.f;

		text->SetPosition(
			std::floor( button->GetAllocation().Width / 2.f - metrics.x / 2.f + .5f + offset ),
			std::floor( button->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f + offset )
		);
		text->SetColor( color );
		queue->Add( text );
	}

	return queue;
}

RenderQueue* BREW::CreateCheckButtonDrawable( SharedPtr<const CheckButton> check ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", check ) );
	sf::Color border_color_dark( border_color_light );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", check ) );
	sf::Color color( GetProperty<sf::Color>( "Color", check ) );
	sf::Color check_color( GetProperty<sf::Color>( "CheckColor", check ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", check ) );
	float border_width( GetProperty<float>( "BorderWidth", check ) );
	float box_size( GetProperty<float>( "BoxSize", check ) );
	float spacing( GetProperty<float>( "Spacing", check ) );
	float check_size( std::min( box_size, GetProperty<float>( "CheckSize", check ) ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", check ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", check ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
	RenderQueue* queue( new RenderQueue );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f,
				check->GetAllocation().Height / 2.f - box_size / 2.f,
				box_size,
				box_size,
				background_color
			)
		)
	);

	if( check->IsActive() ) {
		float diff( box_size - check_size );

		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					diff / 2.f,
					check->GetAllocation().Height / 2.f - box_size / 2.f + diff / 2.f,
					box_size - diff,
					box_size - diff,
					check_color
				)
			)
		);
	}

	queue->Add(
		CreateBorder(
			sf::FloatRect(
				0.f,
				check->GetAllocation().Height / 2.f - box_size / 2.f,
				box_size,
				box_size
			),
			border_width,
			border_color_dark,
			border_color_light
		)
	);

	// Label.
	if( check->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( check->GetLabel(), font, font_size );
		metrics.y = GetLineHeight( font, font_size );

		sf::Text* text( new sf::Text( check->GetLabel(), font, font_size ) );
		text->SetPosition(
			std::floor( box_size + spacing ),
			std::floor( check->GetAllocation().Height / 2.f - metrics.y / 2.f + .5f )
		);
		text->SetColor( color );
		queue->Add( text );
	}

	return queue;
}

RenderQueue* BREW::CreateLabelDrawable( SharedPtr<const Label> label ) const {
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", label ) ) );
	const unsigned int font_size( GetProperty<unsigned int>( "FontSize", label ) );
	const sf::Color font_color( GetProperty<sf::Color>( "Color", label ) );

	sf::Text* vis_label( new sf::Text( label->GetText(), font, font_size ) );
	vis_label->SetColor( font_color );

	// Calculate alignment.
	sf::Vector2f avail_space( label->GetAllocation().Width - label->GetRequisition().x, label->GetAllocation().Height - label->GetRequisition().y );
	sf::Vector2f position( avail_space.x * label->GetAlignment().x, avail_space.y * label->GetAlignment().y );

	vis_label->SetPosition( std::floor( position.x + .5f ), std::floor( position.y + .5f ) );

	RenderQueue* queue( new RenderQueue );
	queue->Add( vis_label );

	return queue;
}

RenderQueue* BREW::CreateEntryDrawable( SharedPtr<const Entry> entry ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", entry ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", entry ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", entry ) );
	sf::Color text_color( GetProperty<sf::Color>( "Color", entry ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "Color", entry ) );
	float text_padding( GetProperty<float>( "Padding", entry ) );
	float cursor_thickness( GetProperty<float>( "Thickness", entry ) );
	float border_width( GetProperty<float>( "BorderWidth", entry ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", entry ) );
	const sf::Font& font( *GetResourceManager().GetFont( GetProperty<std::string>( "FontName", entry ) ) );
	const unsigned int& font_size( GetProperty<unsigned int>( "FontSize", entry ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f, 0.f,
				entry->GetAllocation().Width,
				entry->GetAllocation().Height,
				background_color
			)
		)
	);

	queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, entry->GetAllocation().Width, entry->GetAllocation().Height ), border_width, border_color_dark, border_color_light) );

	sf::Text* vis_label( new sf::Text( entry->GetVisibleText(), font, font_size ) );
	vis_label->SetColor( text_color );
	vis_label->SetPosition( text_padding, text_padding );

	queue->Add( vis_label );

	// Draw cursor if entry is active and cursor is visible.
	if( entry->GetState() == Widget::ACTIVE && entry->IsCursorVisible() ) {
		sf::String cursor_string( entry->GetVisibleText() );
		if( entry->GetCursorPosition() - entry->GetVisibleOffset() < cursor_string.GetSize() ) {
			cursor_string.Erase( entry->GetCursorPosition() - entry->GetVisibleOffset(), cursor_string.GetSize() );
		}

		// Get metrics.
		sf::Vector2f metrics( GetTextMetrics( cursor_string, font, font_size ) );
		float line_height = GetLineHeight( font, font_size );

		sf::Sprite* vis_cursor( new sf::Sprite() );
		vis_cursor->SetPosition( metrics.x + text_padding, border_width + text_padding );
		vis_cursor->Resize( cursor_thickness, line_height );
		vis_cursor->SetColor( cursor_color );
		queue->Add( vis_cursor );
	}

	return queue;
}

RenderQueue* BREW::CreateSlider( const sf::FloatRect& rect, sf::Color& background, float border_width, const sf::Color& light_color, const sf::Color& dark_color ) {
	RenderQueue* queue( new RenderQueue );

	// Align rect to pixels
	sf::FloatRect aligned_rect;
	aligned_rect.Left = std::floor( rect.Left + .5f );
	aligned_rect.Top = std::floor( rect.Top + .5f );
	aligned_rect.Width = std::floor( rect.Width + .5f );
	aligned_rect.Height = std::floor( rect.Height + .5f );

	queue->Add( new sf::Shape( sf::Shape::Rectangle( aligned_rect, background ) ) ); // Background.
	queue->Add( CreateBorder( aligned_rect, border_width, light_color, dark_color ) ); // Border

	return queue;
}

RenderQueue* BREW::CreateScaleDrawable( SharedPtr<const Scale> scale ) const {
	sf::Color trough_color( GetProperty<sf::Color>( "TroughColor", scale ) );
	sf::Color slider_color( GetProperty<sf::Color>( "SliderColor", scale ) );
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", scale ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", scale ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", scale ) );
	float trough_thickness( GetProperty<float>( "TroughWidth", scale ) );
	float border_width( GetProperty<float>( "BorderWidth", scale ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	Scale::Orientation orientation = scale->GetOrientation();

	sf::FloatRect slider_rect = scale->GetSliderRect();

	if( orientation == Scale::HORIZONTAL ) {
		// Trough
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					slider_rect.Width / 2.f,
					( scale->GetAllocation().Height - trough_thickness ) / 2.f,
					scale->GetAllocation().Width - slider_rect.Width,
					trough_thickness,
					trough_color
				)
			)
		);
	}
	else {
		// Trough
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					( scale->GetAllocation().Width - trough_thickness ) / 2.f,
					slider_rect.Height / 2.f,
					trough_thickness,
					scale->GetAllocation().Height - slider_rect.Height,
					trough_color
				)
			)
		);
	}

	// Slider
	queue->Add( CreateSlider( slider_rect, slider_color, border_width, border_color_light, border_color_dark ) );

	return queue;
}

RenderQueue* BREW::CreateStepper( const sf::FloatRect& rect, sf::Color& background, float border_width, const sf::Color& light_color, const sf::Color& dark_color, bool pressed ) {
	RenderQueue* queue( new RenderQueue );

	// Align rect to pixels
	sf::FloatRect aligned_rect;
	aligned_rect.Left = std::floor( rect.Left + .5f );
	aligned_rect.Top = std::floor( rect.Top + .5f );
	aligned_rect.Width = std::floor( rect.Width + .5f );
	aligned_rect.Height = std::floor( rect.Height + .5f );

	queue->Add( new sf::Shape( sf::Shape::Rectangle( aligned_rect, background ) ) ); // Background.

	if( pressed ) {
		queue->Add( CreateBorder( aligned_rect, border_width, dark_color, light_color ) );
	}
	else {
		queue->Add( CreateBorder( aligned_rect, border_width, light_color, dark_color ) );
	}

	return queue;
}

RenderQueue* BREW::CreateScrollbarDrawable( SharedPtr<const Scrollbar> scrollbar ) const {
	sf::Color trough_color( GetProperty<sf::Color>( "TroughColor", scrollbar ) );
	sf::Color slider_color( GetProperty<sf::Color>( "SliderColor", scrollbar ) );
	sf::Color slider_border_color_light( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color slider_border_color_dark( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_color( GetProperty<sf::Color>( "StepperBackgroundColor", scrollbar ) );
	sf::Color stepper_border_color_light( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_border_color_dark( GetProperty<sf::Color>( "BorderColor", scrollbar ) );
	sf::Color stepper_arrow_color( GetProperty<sf::Color>( "StepperArrowColor", scrollbar ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", scrollbar ) );
	float border_width( GetProperty<float>( "BorderWidth", scrollbar ) );

	ShiftBorderColors( slider_border_color_light, slider_border_color_dark, border_color_shift );
	ShiftBorderColors( stepper_border_color_light, stepper_border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	Scrollbar::Orientation orientation = scrollbar->GetOrientation();

	if( orientation == Scrollbar::HORIZONTAL ) {
		float stepper_length = scrollbar->GetAllocation().Height;

		// Trough
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					stepper_length,
					0.f,
					scrollbar->GetAllocation().Width - 2.f * stepper_length,
					scrollbar->GetAllocation().Height,
					trough_color
				)
			)
		);

		// Stepper left
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					0.f,
					0.f,
					stepper_length,
					scrollbar->GetAllocation().Height
				),
				stepper_color,
				border_width,
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsDecreaseStepperPressed()
			)
		);

		sf::Shape* arrow_left = new sf::Shape();
		arrow_left->AddPoint(
			stepper_length * .66f,
			scrollbar->GetAllocation().Height * .33f,
			stepper_arrow_color
		);
		arrow_left->AddPoint(
			stepper_length * .33f,
			scrollbar->GetAllocation().Height * .5f,
			stepper_arrow_color
		);
		arrow_left->AddPoint(
			stepper_length * .66f,
			scrollbar->GetAllocation().Height * .66f,
			stepper_arrow_color
		);

		queue->Add(arrow_left);

		// Stepper right
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					scrollbar->GetAllocation().Width - stepper_length,
					0.f,
					stepper_length,
					scrollbar->GetAllocation().Height
				),
				stepper_color,
				border_width,
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsIncreaseStepperPressed()
			)
		);

		sf::Shape* arrow_right = new sf::Shape();
		arrow_right->AddPoint(
			scrollbar->GetAllocation().Width - stepper_length * .66f,
			scrollbar->GetAllocation().Height * .33f,
			stepper_arrow_color
		);
		arrow_right->AddPoint(
			scrollbar->GetAllocation().Width - stepper_length * .33f,
			scrollbar->GetAllocation().Height * .5f,
			stepper_arrow_color
		);
		arrow_right->AddPoint(
			scrollbar->GetAllocation().Width - stepper_length * .66f,
			scrollbar->GetAllocation().Height * .66f,
			stepper_arrow_color
		);

		queue->Add(arrow_right);
	}
	else {
		float stepper_length = scrollbar->GetAllocation().Width;

		// Trough
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					0.f,
					stepper_length,
					scrollbar->GetAllocation().Width,
					scrollbar->GetAllocation().Height - 2.f * stepper_length,
					trough_color
				)
			)
		);

		// Stepper top
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					0.f,
					0.f,
					scrollbar->GetAllocation().Width,
					stepper_length
				),
				stepper_color,
				border_width,
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsDecreaseStepperPressed()
			)
		);

		sf::Shape* arrow_up = new sf::Shape();
		arrow_up->AddPoint(
			scrollbar->GetAllocation().Width * .33f,
			stepper_length * .66f,
			stepper_arrow_color
		);
		arrow_up->AddPoint(
			scrollbar->GetAllocation().Width * .66f,
			stepper_length * .66f,
			stepper_arrow_color
		);
		arrow_up->AddPoint(
			scrollbar->GetAllocation().Width * .5f,
			stepper_length * .33f,
			stepper_arrow_color
		);

		queue->Add(arrow_up);

		// Stepper bottom
		queue->Add(
			CreateStepper(
				sf::FloatRect(
					0.f,
					scrollbar->GetAllocation().Height - stepper_length,
					scrollbar->GetAllocation().Width,
					stepper_length
				),
				stepper_color,
				border_width,
				stepper_border_color_light,
				stepper_border_color_dark,
				scrollbar->IsIncreaseStepperPressed()
			)
		);

		sf::Shape* arrow_down = new sf::Shape();
		arrow_down->AddPoint(
			scrollbar->GetAllocation().Width * .33f,
			scrollbar->GetAllocation().Height - stepper_length * .66f,
			stepper_arrow_color
		);
		arrow_down->AddPoint(
			scrollbar->GetAllocation().Width * .66f,
			scrollbar->GetAllocation().Height - stepper_length * .66f,
			stepper_arrow_color
		);
		arrow_down->AddPoint(
			scrollbar->GetAllocation().Width * .5f,
			scrollbar->GetAllocation().Height - stepper_length * .33f,
			stepper_arrow_color
		);

		queue->Add(arrow_down);
	}

	// Slider
	queue->Add( CreateSlider( scrollbar->GetSliderRect(), slider_color, border_width, slider_border_color_light, slider_border_color_dark ) );

	return queue;
}

RenderQueue* BREW::CreateScrolledWindowDrawable( SharedPtr<const ScrolledWindow> scrolled_window ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", scrolled_window ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", scrolled_window ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", scrolled_window ) );
	float border_width( GetProperty<float>( "BorderWidth", scrolled_window ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	sf::FloatRect rect = scrolled_window->GetContentAllocation();

	rect.Left -= ( border_width + scrolled_window->GetAllocation().Left );
	rect.Top -= ( border_width + scrolled_window->GetAllocation().Top );
	rect.Width += 2.f * border_width;
	rect.Height += 2.f * border_width;

	queue->Add( CreateBorder( sf::FloatRect( 0.f, 0.f, rect.Width, rect.Height ), border_width, border_color_dark, border_color_light ) );

	return queue;
}

RenderQueue* BREW::CreateProgressBarDrawable( SharedPtr<const ProgressBar> progress_bar ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", progress_bar ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", progress_bar ) );
	sf::Color bar_border_color_light( GetProperty<sf::Color>( "BarBorderColor", progress_bar ) );
	sf::Color bar_border_color_dark( GetProperty<sf::Color>( "BarBorderColor", progress_bar ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", progress_bar ) );
	sf::Color progress_color( GetProperty<sf::Color>( "BarColor", progress_bar ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", progress_bar ) );
	int bar_border_color_shift( GetProperty<int>( "BarBorderColorShift", progress_bar ) );
	float border_width( GetProperty<float>( "BorderWidth", progress_bar ) );
	float bar_border_width( GetProperty<float>( "BarBorderWidth", progress_bar ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );
	ShiftBorderColors( bar_border_color_light, bar_border_color_dark, bar_border_color_shift );

	RenderQueue* queue( new RenderQueue );

	// Background.
	queue->Add(
		new sf::Shape(
			sf::Shape::Rectangle(
				0.f,
				0.f,
				progress_bar->GetAllocation().Width,
				progress_bar->GetAllocation().Height,
				background_color
			)
		)
	);

	if( progress_bar->GetFraction() > 0.f ) {
		sf::FloatRect bar_rect;

		if( progress_bar->GetOrientation() == ProgressBar::HORIZONTAL ) {
			float frac_width( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().Width * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				std::floor( border_width + .5f ),
				std::floor( border_width + .5f ),
				std::floor( std::max( 0.f, frac_width - 2.f * border_width ) + .5f ),
				std::floor( std::max( 0.f, progress_bar->GetAllocation().Height - 2.f * border_width ) + .5f )
			);
		}
		else {
			float frac_height( std::max( 2.f * bar_border_width, progress_bar->GetAllocation().Height * progress_bar->GetFraction() ) );

			bar_rect = sf::FloatRect(
				std::floor( border_width + .5f ),
				std::floor( std::max( 0.f, progress_bar->GetAllocation().Height - frac_height + border_width ) + .5f ),
				std::floor( std::max( 0.f, progress_bar->GetAllocation().Width - 2.f * border_width ) + .5f ),
				std::floor( std::max( 0.f, frac_height - 2.f * border_width ) + .5f )
			);
		}

		// Bar.
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					bar_rect,
					progress_color
				)
			)
		);

		// Bar border.
		queue->Add(
			CreateBorder(
				bar_rect,
				bar_border_width,
				bar_border_color_light,
				bar_border_color_dark
			)
		);
	}

	// Border.
	queue->Add(
		CreateBorder(
			sf::FloatRect(
				0.f,
				0.f,
				progress_bar->GetAllocation().Width,
				progress_bar->GetAllocation().Height
			),
			border_width,
			border_color_dark,
			border_color_light
		)
	);

	return queue;
}

RenderQueue* BREW::CreateSeparatorDrawable( SharedPtr<const Separator> separator ) const {
	sf::Color color_light( GetProperty<sf::Color>( "Color", separator ) );
	sf::Color color_dark( color_light );
	int color_shift( GetProperty<int>( "BorderColorShift", separator ) );

	ShiftBorderColors( color_light, color_dark, color_shift );

	RenderQueue* queue( new RenderQueue );

	if( separator->GetOrientation() == Separator::HORIZONTAL ) {
		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					.5f,
					std::floor( separator->GetAllocation().Height / 2.f - .5f ) + .5f,
					std::floor( separator->GetAllocation().Width - .5f ) + .5f,
					std::floor( separator->GetAllocation().Height / 2.f - .5f ) + .5f,
					1.f,
					color_dark
				)
			)
		);

		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					.5f,
					std::floor( separator->GetAllocation().Height / 2.f - .5f ) + 1.5f,
					std::floor( separator->GetAllocation().Width - .5f ) + .5f,
					std::floor( separator->GetAllocation().Height / 2.f - .5f ) + 1.5f,
					1.f,
					color_light
				)
			)
		);
	}
	else {
		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					std::floor( separator->GetAllocation().Width / 2.f - .5f ) + .5f,
					.5f,
					std::floor( separator->GetAllocation().Width / 2.f - .5f ) + .5f,
					std::floor( separator->GetAllocation().Height - .5f ) + .5f,
					1.f,
					color_dark
				)
			)
		);

		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					std::floor( separator->GetAllocation().Width / 2.f - .5f ) + 1.5f,
					.5f,
					std::floor( separator->GetAllocation().Width / 2.f - .5f ) + 1.5f,
					std::floor( separator->GetAllocation().Height - .5f ) + .5f,
					1.f,
					color_light
				)
			)
		);
	}

	return queue;
}

RenderQueue* BREW::CreateFrameDrawable( SharedPtr<const Frame> frame ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", frame ) );
	sf::Color border_color_dark( border_color_light );
	int border_color_shift( GetProperty<int>( "BorderColorShift", frame ) );
	sf::Color color( GetProperty<sf::Color>( "Color", frame ) );
	float border_width( GetProperty<float>( "BorderWidth", frame ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", frame ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", frame ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
	float label_padding( GetProperty<float>( "LabelPadding", frame ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	float line_height = GetLineHeight( font, font_size );

	RenderQueue* queue( new RenderQueue );

	// Bottom
	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				0.f + .5f,
				std::floor( frame->GetAllocation().Height - border_width + .5f ) + .5f,
				frame->GetAllocation().Width + .5f,
				std::floor( frame->GetAllocation().Height - border_width + .5f ) + .5f,
				border_width,
				border_color_light
			)
		)
	);

	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				0.f + .5f,
				std::floor( frame->GetAllocation().Height - 2.f * border_width + .5f ) + .5f,
				frame->GetAllocation().Width - border_width + .5f,
				std::floor( frame->GetAllocation().Height - 2.f * border_width + .5f ) + .5f,
				border_width,
				border_color_dark
			)
		)
	);

	// Left
	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				0.f + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				0.f + .5f,
				std::floor( frame->GetAllocation().Height - border_width + .5f ) + .5f,
				border_width,
				border_color_dark
			)
		)
	);

	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				border_width + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				border_width + .5f,
				std::floor( frame->GetAllocation().Height - border_width + .5f ) + .5f,
				border_width,
				border_color_light
			)
		)
	);

	// Right
	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				std::floor( frame->GetAllocation().Width - border_width + .5f ) + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				std::floor( frame->GetAllocation().Width - border_width + .5f ) + .5f,
				std::floor( frame->GetAllocation().Height - 2.f * border_width + .5f ) + .5f,
				border_width,
				border_color_light
			)
		)
	);

	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				std::floor( frame->GetAllocation().Width - 2.f * border_width + .5f ) + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				std::floor( frame->GetAllocation().Width - 2.f * border_width + .5f ) + .5f,
				std::floor( frame->GetAllocation().Height - 2.f * border_width + .5f ) + .5f,
				border_width,
				border_color_dark
			)
		)
	);

	float label_start_x = line_height;
	float label_end_x = line_height;

	float alignment = frame->GetAlignment().x;

	if( frame->GetLabel().GetSize() > 0 ) {
		sf::Vector2f metrics = GetTextMetrics( frame->GetLabel(), font, font_size );
		metrics.x += ( 2 * label_padding );

		label_start_x += ( alignment * ( frame->GetAllocation().Width - 2 * line_height - metrics.x ) );
		label_end_x += ( metrics.x + alignment * ( frame->GetAllocation().Width - 2 * line_height - metrics.x ) );

		sf::Text* text( new sf::Text( frame->GetLabel(), font, font_size ) );
		text->SetPosition( std::floor( label_start_x + label_padding + .5f ), .0f );
		text->SetColor( color );
		queue->Add( text );
	}

	// Top Left
	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				0.f + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				std::floor( label_start_x + .5f ) + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				border_width,
				border_color_dark
			)
		)
	);

	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				border_width + .5f,
				std::floor( line_height / 2.f + border_width + .5f ) + .5f,
				std::floor( label_start_x + .5f ) + .5f,
				std::floor( line_height / 2.f + border_width + .5f ) + .5f,
				border_width,
				border_color_light
			)
		)
	);

	// Top Right
	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				std::floor( label_end_x + .5f ) + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				std::floor( frame->GetAllocation().Width + .5f ) + .5f,
				std::floor( line_height / 2.f + .5f ) + .5f,
				border_width,
				border_color_dark
			)
		)
	);

	queue->Add(
		new sf::Shape(
			sf::Shape::Line(
				std::floor( label_end_x + .5f ) + .5f,
				std::floor( line_height / 2.f + border_width + .5f ) + .5f,
				std::floor( frame->GetAllocation().Width - border_width + .5f ) + .5f,
				std::floor( line_height / 2.f + border_width + .5f ) + .5f,
				border_width,
				border_color_light
			)
		)
	);

	return queue;
}

RenderQueue* BREW::CreateImageDrawable( SharedPtr<const Image> image ) const {
	RenderQueue* queue( new RenderQueue );

	sf::Sprite* sprite = new sf::Sprite( image->GetTexture() );
	sprite->SetPosition(
		( image->GetAllocation().Width - image->GetRequisition().x ) * image->GetAlignment().x,
		( image->GetAllocation().Height - image->GetRequisition().y ) * image->GetAlignment().y
	);

	queue->Add( sprite );

	return queue;
}

RenderQueue* BREW::CreateNotebookDrawable( SharedPtr<const Notebook> notebook ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", notebook ) );
	sf::Color border_color_dark( border_color_light );
	int border_color_shift( GetProperty<int>( "BorderColorShift", notebook ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", notebook ) );
	sf::Color background_color_dark( GetProperty<sf::Color>( "BackgroundColorDark", notebook ) );
	sf::Color background_color_prelight( GetProperty<sf::Color>( "BackgroundColorPrelight", notebook ) );
	float padding( GetProperty<float>( "Padding", notebook ) );
	float border_width( GetProperty<float>( "BorderWidth", notebook ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	Notebook::IndexType page_count = notebook->GetPageCount();

	if( !page_count ) {
		return queue;
	}

	Notebook::IndexType current_page = notebook->GetCurrentPage();
	Notebook::IndexType prelight_tab = notebook->GetPrelightTab();

	// Get size in the dimension all tabs have uniform size.
	sf::Vector2f tab_size( notebook->GetNthTabLabel( 0 )->GetAllocation().Width, notebook->GetNthTabLabel( 0 )->GetAllocation().Height );

	// Get size in the dimension all children have uniform size.
	sf::Vector2f child_size( notebook->GetNthPage( 0 )->GetAllocation().Width, notebook->GetNthPage( 0 )->GetAllocation().Height );

	if( notebook->GetTabPosition() == Notebook::TOP ) {
		// Tabs are positioned at top.

		// Pane background
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					0.f,
					tab_size.y + 2.f * ( border_width + padding ),
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding ),
					background_color
				)
			)
		);

		// Pane border
		queue->Add(
			CreateBorder(
				sf::FloatRect(
					0.f,
					tab_size.y + 2.f * ( border_width + padding ),
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding )
				),
				border_width,
				border_color_light,
				border_color_dark
			)
		);

		// First tab label left border
		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					.5f,
					.5f,
					.5f,
					std::floor( tab_size.y + 3.f * border_width + 2.f * padding ) - .5f,
					border_width,
					border_color_light
				)
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Top border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left - border_width - padding ) + .5f,
						std::floor( label_allocation.Top - border_width - padding ) + .5f,
						std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
						std::floor( label_allocation.Top - border_width - padding ) + .5f,
						border_width,
						border_color_light
					)
				)
			);

			// Right border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
						std::floor( label_allocation.Top - border_width - padding ) + .5f,
						std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
						std::floor( label_allocation.Top + label_allocation.Height + 2.f * border_width + padding ) - .5f,
						border_width,
						border_color_dark
					)
				)
			);

			if( index == current_page ) {
				// Active left border
				queue->Add(
					new sf::Shape(
						sf::Shape::Line(
							std::floor( label_allocation.Left - border_width - padding ) + .5f,
							std::floor( label_allocation.Top - border_width - padding ) + .5f,
							std::floor( label_allocation.Left - border_width - padding ) + .5f,
							std::floor( label_allocation.Top + label_allocation.Height + 2.f * border_width + padding ) - .5f,
							border_width,
							border_color_light
						)
					)
				);

				// Active background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding,
							label_allocation.Height + 2.f * ( border_width + padding ),
							background_color
						)
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding,
							label_allocation.Height + border_width + 2.f * padding,
							( index == prelight_tab ) ? background_color_prelight : background_color_dark
						)
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::BOTTOM ) {
		// Tabs are positioned at bottom.

		// Pane background
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					0.f,
					0.f,
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding ),
					background_color
				)
			)
		);

		// Pane border
		queue->Add(
			CreateBorder(
				sf::FloatRect(
					0.f,
					0.f,
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding )
				),
				border_width,
				border_color_light,
				border_color_dark
			)
		);

		// First tab label left border
		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					.5f,
					std::floor( child_size.y + 2.f * ( border_width + padding ) ) - .5f,
					.5f,
					std::floor( child_size.y + 2.f * ( border_width + padding ) + tab_size.y + 2.f * ( border_width + padding ) ) - .5f,
					border_width,
					border_color_light
				)
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Bottom border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left - border_width - padding ) + .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						std::floor( label_allocation.Left + label_allocation.Width + 2.f * border_width + padding ) - .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						border_width,
						border_color_dark
					)
				)
			);

			// Right border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
						std::floor( label_allocation.Top - border_width - padding ) + .5f,
						std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						border_width,
						border_color_dark
					)
				)
			);

			if( index == current_page ) {
				// Active left border
				queue->Add(
					new sf::Shape(
						sf::Shape::Line(
							std::floor( label_allocation.Left - border_width - padding ) + .5f,
							std::floor( label_allocation.Top - border_width - padding ) + .5f,
							std::floor( label_allocation.Left - border_width - padding ) + .5f,
							std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
							border_width,
							border_color_light
						)
					)
				);

				// Active background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - padding,
							label_allocation.Top - padding - 2.f * border_width,
							label_allocation.Width + 2.f * padding,
							label_allocation.Height + 2.f * ( border_width + padding ),
							background_color
						)
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding,
							label_allocation.Height + 2.f * padding,
							( index == prelight_tab ) ? background_color_prelight : background_color_dark
						)
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::LEFT ) {
		// Tabs are positioned at left.

		// Pane background
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					tab_size.x + 2.f * ( border_width + padding ),
					0.f,
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding ),
					background_color
				)
			)
		);

		// Pane border
		queue->Add(
			CreateBorder(
				sf::FloatRect(
					tab_size.x + 2.f * ( border_width + padding ),
					0.f,
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding )
				),
				border_width,
				border_color_light,
				border_color_dark
			)
		);

		// First tab label top border
		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					.5f,
					.5f,
					std::floor( tab_size.x + 2.f * padding + 3.f * border_width ) - .5f,
					.5f,
					border_width,
					border_color_light
				)
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Left border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left - border_width - padding ) + .5f,
						std::floor( label_allocation.Top - border_width - padding ) + .5f,
						std::floor( label_allocation.Left - border_width - padding ) + .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						border_width,
						border_color_light
					)
				)
			);

			// Bottom border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left - border_width - padding ) + .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						std::floor( label_allocation.Left + label_allocation.Width + 2.f * border_width + padding ) - .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						border_width,
						border_color_dark
					)
				)
			);

			if( index == current_page ) {
				// Active top border
				queue->Add(
					new sf::Shape(
						sf::Shape::Line(
							std::floor( label_allocation.Left - border_width - padding ) + .5f,
							std::floor( label_allocation.Top - border_width - padding ) + .5f,
							std::floor( label_allocation.Left + label_allocation.Width + 2.f * border_width + padding ) - .5f,
							std::floor( label_allocation.Top - border_width - padding ) + .5f,
							border_width,
							border_color_light
						)
					)
				);

				// Active background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * ( border_width + padding ),
							label_allocation.Height + 2.f * padding,
							background_color
						)
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + border_width + 2.f * padding,
							label_allocation.Height + 2.f * padding,
							( index == prelight_tab ) ? background_color_prelight : background_color_dark
						)
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::RIGHT ) {
		// Tabs are positioned at right.

		// Pane background
		queue->Add(
			new sf::Shape(
				sf::Shape::Rectangle(
					0.f,
					0.f,
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding ),
					background_color
				)
			)
		);

		// Pane border
		queue->Add(
			CreateBorder(
				sf::FloatRect(
					0.f,
					0.f,
					child_size.x + 2.f * ( border_width + padding ),
					child_size.y + 2.f * ( border_width + padding )
				),
				border_width,
				border_color_light,
				border_color_dark
			)
		);

		// First tab label top border
		queue->Add(
			new sf::Shape(
				sf::Shape::Line(
					std::floor( child_size.x + 2.f * ( border_width + padding ) ) - .5f,
					.5f,
					std::floor( child_size.x + 4.f * padding + 3.f * border_width + tab_size.x ) - .5f,
					.5f,
					border_width,
					border_color_light
				)
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Right border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
						std::floor( label_allocation.Top - border_width - padding ) + .5f,
						std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						border_width,
						border_color_dark
					)
				)
			);

			// Bottom border
			queue->Add(
				new sf::Shape(
					sf::Shape::Line(
						std::floor( label_allocation.Left - border_width - padding ) + .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						std::floor( label_allocation.Left + label_allocation.Width + 2.f * border_width + padding ) - .5f,
						std::floor( label_allocation.Top + label_allocation.Height + border_width + padding ) - .5f,
						border_width,
						border_color_dark
					)
				)
			);

			if( index == current_page ) {
				// Active top border
				queue->Add(
					new sf::Shape(
						sf::Shape::Line(
							std::floor( label_allocation.Left - border_width - padding ) + .5f,
							std::floor( label_allocation.Top - border_width - padding ) + .5f,
							std::floor( label_allocation.Left + label_allocation.Width + border_width + padding ) - .5f,
							std::floor( label_allocation.Top - border_width - padding ) + .5f,
							border_width,
							border_color_light
						)
					)
				);

				// Active background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - 2.f * border_width - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * ( border_width + padding ),
							label_allocation.Height + 2.f * padding,
							background_color
						)
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					new sf::Shape(
						sf::Shape::Rectangle(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding,
							label_allocation.Height + 2.f * padding,
							( index == prelight_tab ) ? background_color_prelight : background_color_dark
						)
					)
				);
			}
		}
	}

	return queue;
}

RenderQueue* BREW::CreateSpinnerDrawable( SharedPtr<const Spinner> spinner ) const {
	sf::Color color( GetProperty<sf::Color>( "Color", spinner ) );
	float steps( GetProperty<float>( "Steps", spinner ) );
	float fade( GetProperty<float>( "Fade", spinner ) );
	unsigned int stopped_alpha( GetProperty<unsigned int>( "StoppedAlpha", spinner ) );
	float radius = std::min( spinner->GetAllocation().Width, spinner->GetAllocation().Height ) / 2.f;

	// Make sure steps is sane.
	steps = std::max( steps, 3.f );

	sf::Shape* shape = new sf::Shape;

	// SFML does this too, for compatibility reasons, so lay off the flame :P
	static const float two_pi = 3.141592654f * 2.f;

	for( float index = 0.f; index < steps; index += 1.f ) {
		sf::Color rod_color( color );

		if( spinner->Started() ) {
			rod_color.a = static_cast<sf::Uint8>( 255.f * pow( index / ( steps - 1.f ), fade ) );
		}
		else {
			rod_color.a = static_cast<sf::Uint8>( stopped_alpha );
		}

		// Time for some hardcore trigonometry...
		sf::Vector2f point1(
			static_cast<float>( cos( two_pi * index / steps ) ) * radius,
			static_cast<float>( sin( two_pi * index / steps ) ) * radius
		);

		sf::Vector2f point2(
			static_cast<float>( cos( two_pi * ( index + .5f ) / steps ) ) * radius,
			static_cast<float>( sin( two_pi * ( index + .5f ) / steps ) ) * radius
		);

		shape->AddPoint( sf::Vector2f( 0.f, 0.f ), rod_color );
		shape->AddPoint( point1, rod_color );
		shape->AddPoint( point2, rod_color );
	}

	shape->SetPosition( spinner->GetAllocation().Width / 2.f, spinner->GetAllocation().Height / 2.f );

	shape->EnableOutline( false );
	shape->EnableFill( true );

	RenderQueue* queue( new RenderQueue );

	queue->Add( shape );

	return queue;
}

}
}
