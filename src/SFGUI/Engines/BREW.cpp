#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sfg {
namespace eng {

BREW::BREW() :
	Engine()
{
	// Set defaults.
	SetProperty( "", "Color", sf::Color( 0xc6, 0xcb, 0xc4 ) );
	SetProperty( "", "FontSize", 12 );
	SetProperty( "", "FontName", "" ); // Uses SFML's default font when empty.
	SetProperty( "", "BackgroundColor", sf::Color( 0x46, 0x46, 0x46 ) );
	SetProperty( "", "BorderColor", sf::Color( 0x66, 0x66, 0x66 ) );
	SetProperty( "", "BorderColorShift", 0x20 );
	SetProperty( "", "BorderWidth", 1.f );
	SetProperty( "", "Padding", 5.f );
	SetProperty( "", "Thickness", 2.f );

	// Window-specific.
	SetProperty( "Window", "TitleHeight", 25.f );
	SetProperty( "Window", "TitleBackgroundColor", sf::Color( 0x5a, 0x6a, 0x50 ) );

	// Button-specific.
	SetProperty( "Button", "BackgroundColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "Button", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
	SetProperty( "Button:Prelight", "BackgroundColor", sf::Color( 0x65, 0x67, 0x62 ) );
	SetProperty( "Button:Prelight > Label", "Color", sf::Color::White );
	SetProperty( "Button:Active", "BackgroundColor", sf::Color( 0x55, 0x55, 0x55 ) );
	SetProperty( "Button:Active > Label", "Color", sf::Color::Black );

	// Entry-specific.
	SetProperty( "Entry", "BackgroundColor", sf::Color( 0x5e, 0x5e, 0x5e ) );
	SetProperty( "Entry", "BorderColor", sf::Color( 0x30, 0x32, 0x2f ) );
	SetProperty( "Entry", "Color", sf::Color::White );

	// Scale-specific.
	SetProperty( "Scale", "SliderColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scale", "TroughColor", sf::Color( 0x70, 0x70, 0x70 ) );
	SetProperty( "Scale", "TroughWidth", 5.f );

	// Scrollbar-specific.
	SetProperty( "Scrollbar", "SliderColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scrollbar", "TroughColor", sf::Color( 0x70, 0x70, 0x70 ) );
	SetProperty( "Scrollbar", "StepperBackgroundColor", sf::Color( 0x68, 0x6a, 0x65 ) );
	SetProperty( "Scrollbar", "StepperArrowColor", sf::Color( 0xd9, 0xdc, 0xd5 ) );
	SetProperty( "Scrollbar", "StepperSpeed", 20.f );
	SetProperty( "Scrollbar", "SliderMinimumLength", 15.f );

	// ScrolledWindow-specific.
	SetProperty( "ScrolledWindow", "ScrollbarWidth", 20.f );
	SetProperty( "ScrolledWindow", "ScrollbarSpacing", 5.f );
	SetProperty( "ScrolledWindow", "BorderColor", sf::Color( 0x55, 0x57, 0x52 ) );
}

sf::Drawable* BREW::CreateWindowDrawable( boost::shared_ptr<Window> window ) const {
	RenderQueue* queue( new RenderQueue );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", window ) );
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", window ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", window ) );
	sf::Color title_background_color( GetProperty<sf::Color>( "TitleBackgroundColor", window ) );
	sf::Color title_text_color( GetProperty<sf::Color>( "Color", window ) );
	float border_width( GetProperty<float>( "BorderWidth", window ) );
	float title_size( GetProperty<float>( "TitleHeight", window ) );
	float shadow_distance( GetProperty<float>( "ShadowDistance", window ) );
	sf::Uint8 shadow_alpha( GetProperty<sf::Uint8>( "ShadowAlpha", window ) );
	unsigned int title_font_size( GetProperty<unsigned int>( "FontSize", window ) );

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
		title_text->SetColor( title_text_color );

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

sf::Drawable* BREW::CreateButtonDrawable( boost::shared_ptr<Button> button ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", button ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", button ) );
	int border_color_shift( GetProperty<int>( "BorderColorShift", button ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", button ) );
	sf::Color text_color( GetProperty<sf::Color>( "Color", button ) );
	float border_width( GetProperty<float>( "BorderWidth", button ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

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

sf::Drawable* BREW::CreateLabelDrawable( boost::shared_ptr<Label> label ) const {
	const sf::Font& font( LoadFontFromFile( GetProperty<std::string>( "FontName", label ) ) );
	const unsigned int font_size( GetProperty<unsigned int>( "FontSize", label ) );
	const sf::Color font_color( GetProperty<sf::Color>( "Color", label ) );

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

sf::Drawable* BREW::CreateEntryDrawable( boost::shared_ptr<Entry> entry ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", entry ) );
	sf::Color border_color_dark( GetProperty<sf::Color>( "BorderColor", entry ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", entry ) );
	sf::Color text_color( GetProperty<sf::Color>( "Color", entry ) );
	sf::Color cursor_color( GetProperty<sf::Color>( "Color", entry ) );
	float text_padding( GetProperty<float>( "Padding", entry ) );
	float cursor_thickness( GetProperty<float>( "Thickness", entry ) );
	float border_width( GetProperty<float>( "BorderWidth", entry ) );
	const sf::Font&  font( LoadFontFromFile( GetProperty<std::string>( "FontName", entry ) ) );
	const unsigned int&  font_size( GetProperty<unsigned int>( "FontSize", entry ) );

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

	queue->Add( CreateBorder( entry->GetAllocation(), border_width, border_color_dark, border_color_light) );
	
	sf::Text*  vis_label( new sf::Text( entry->GetVisibleText(), font, font_size ) );
	vis_label->SetColor( text_color );
	vis_label->SetPosition( text_padding, text_padding );

	queue->Add( vis_label );
	
	// Draw cursor if entry is active and cursor is visible.
	if( entry->GetState() == Widget::Active && entry->IsCursorVisible() ) {
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

RenderQueue* BREW::CreateAbsoluteBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color ) {
	RenderQueue* queue( new RenderQueue );

	queue->Add( new sf::Shape( sf::Shape::Rectangle( rect.Left + rect.Width - border_width + .1f, rect.Top + .1f, border_width, rect.Height, dark_color ) ) ); // Right.
	queue->Add( new sf::Shape( sf::Shape::Rectangle( rect.Left + .1f, rect.Top + rect.Height - border_width + .1f, rect.Width, border_width, dark_color ) ) ); // Bottom.

	for( float delta = 0.f; delta < border_width; delta += 1.f ) {
		queue->Add( new sf::Shape( sf::Shape::Line( rect.Left + .1f, rect.Top + delta + .1f, rect.Left + rect.Width - delta, rect.Top + delta, 1.f, light_color ) ) ); // Top.
		queue->Add( new sf::Shape( sf::Shape::Line( rect.Left + delta + .1f, rect.Top + .1f, rect.Left + delta, rect.Top + rect.Height - delta, 1.f, light_color ) ) ); // Left.
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
	queue->Add( CreateAbsoluteBorder( aligned_rect, border_width, light_color, dark_color ) ); // Border

	return queue;
}

sf::Drawable* BREW::CreateScaleDrawable( boost::shared_ptr<Scale> scale ) const {
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

	if( orientation == Scale::Horizontal ) {
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
		queue->Add( CreateAbsoluteBorder( aligned_rect, border_width, dark_color, light_color ) );
	}
	else {
		queue->Add( CreateAbsoluteBorder( aligned_rect, border_width, light_color, dark_color ) );
	}

	return queue;
}

sf::Drawable* BREW::CreateScrollbarDrawable( boost::shared_ptr<Scrollbar> scrollbar ) const {
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

	if( orientation == Scrollbar::Horizontal ) {
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

sf::Drawable* BREW::CreateScrolledWindowDrawable( boost::shared_ptr<ScrolledWindow> scrolled_window ) const {
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

	queue->Add( CreateAbsoluteBorder( rect, border_width, border_color_dark, border_color_light ) );

	return queue;
}

}
}
