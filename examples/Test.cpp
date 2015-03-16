#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/Renderers.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/OpenGL.hpp>
#include <sstream>
#include <cmath>
#include <memory>

class SampleApp {
	public:
		SampleApp();
		~SampleApp();
		void Run();

	private:
		void OnAddButtonHClick();
		void OnAddButtonVClick();
		void OnToggleTitlebarClick();
		void OnHideWindowClicked();
		void OnToggleSpaceClick();
		void OnLimitCharsToggle();
		void OnLoadThemeClick();
		void OnAdjustmentChange();
		void OnToggleSpinner();
		void OnMirrorImageClick();
		void OnSwitchRendererClick();
		void RenderCustomGL();
		void RenderCustomSFML();

		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		sfg::Window::Ptr m_wndmain;
		sfg::Box::Ptr m_boxbuttonsh;
		sfg::Box::Ptr m_boxbuttonsv;
		sfg::Entry::Ptr m_entry;
		sfg::Table::Ptr m_table;
		sfg::ScrolledWindow::Ptr m_scrolled_window;
		sfg::Box::Ptr m_scrolled_window_box;
		sfg::ToggleButton::Ptr m_titlebar_toggle;
		sfg::CheckButton::Ptr m_limit_check;
		sfg::Scale::Ptr m_scale;
		sfg::ComboBox::Ptr m_combo_box;
		sfg::ProgressBar::Ptr m_progress;
		sfg::ProgressBar::Ptr m_progress_vert;
		sfg::Spinner::Ptr m_spinner;
		sfg::Image::Ptr m_image;
		sfg::Canvas::Ptr m_gl_canvas;
		sfg::Canvas::Ptr m_sfml_canvas;
		sfg::Button::Ptr m_switch_renderer;

		sfg::Desktop m_desktop;

		unsigned int m_fps_counter;
		sf::Clock m_fps_clock;

		sf::Texture m_background_texture;
		sf::Sprite m_background_sprite;
		sf::Sprite m_canvas_sprite;

		sf::RenderWindow m_window;

		GLuint m_custom_draw_display_list;
};

class Ouchy : public std::enable_shared_from_this<Ouchy> {
	public:
		typedef std::shared_ptr<Ouchy> Ptr; //!< Shared pointer.
		Ouchy( sfg::Button::Ptr button );

		void DoOuch();

		static std::vector<Ptr> m_ouchies;
	private:
		sfg::Button::Ptr m_button;
		bool m_state;
};

std::vector<Ouchy::Ptr> Ouchy::m_ouchies;

Ouchy::Ouchy( sfg::Button::Ptr button ) :
	m_button( button ),
	m_state( false )
{
}

void Ouchy::DoOuch() {
	if( !m_state ) {
		m_button->SetLabel( "Ouch" );
	}
	else {
		m_button->SetLabel( "Boom" );
	}

	m_state = !m_state;
}

SampleApp::SampleApp() :
	m_desktop(),
	m_window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test", sf::Style::Default, sf::ContextSettings( 16, 0, 0, 2, 1 ) ),
	m_custom_draw_display_list( 0 )
{
	m_background_texture.create( 1024, 768 );

	std::vector<sf::Uint8> pixels( 1024 * 768 * 4 );

	sf::Uint8 pixel_value = 139;

	for( std::size_t index = 0; index < 1024 * 768; ++index ) {
		pixel_value = static_cast<sf::Uint8>( pixel_value ^ ( index + 809 ) );
		pixel_value = static_cast<sf::Uint8>( pixel_value << ( index % 11 ) );
		pixel_value = static_cast<sf::Uint8>( pixel_value * 233 );

		pixels[ index * 4 + 0 ] = static_cast<sf::Uint8>( pixel_value % 16 + 72 ); // R

		pixel_value ^= static_cast<sf::Uint8>( index );
		pixel_value = static_cast<sf::Uint8>( pixel_value * 23 );

		pixels[ index * 4 + 1 ] = static_cast<sf::Uint8>( pixel_value % 16 + 72 ); // G

		pixel_value ^= static_cast<sf::Uint8>( index );
		pixel_value = static_cast<sf::Uint8>( pixel_value * 193 );

		pixels[ index * 4 + 2 ] = static_cast<sf::Uint8>( pixel_value % 16 + 72 ); // B

		pixels[ index * 4 + 3 ] = 255; // A
	}

	m_background_texture.update( pixels.data() );

	m_background_sprite.setTexture( m_background_texture );
}

SampleApp::~SampleApp() {
	Ouchy::m_ouchies.clear();
}

void SampleApp::Run() {
	sf::Event event;

	//m_window.SetFramerateLimit( 60 );
	//m_window.EnableVerticalSync( true );

	std::string renderer_string;

	// Tune Renderer
	if( m_sfgui.GetRenderer().GetName() == "Non-Legacy Renderer" ) {
		static_cast<sfg::NonLegacyRenderer*>( &m_sfgui.GetRenderer() )->TuneUseFBO( true );
		static_cast<sfg::NonLegacyRenderer*>( &m_sfgui.GetRenderer() )->TuneCull( true );

		renderer_string = "NLR";
	}
	if( m_sfgui.GetRenderer().GetName() == "Vertex Buffer Renderer" ) {
		static_cast<sfg::VertexBufferRenderer*>( &m_sfgui.GetRenderer() )->TuneUseFBO( true );
		static_cast<sfg::VertexBufferRenderer*>( &m_sfgui.GetRenderer() )->TuneAlphaThreshold( .2f );
		static_cast<sfg::VertexBufferRenderer*>( &m_sfgui.GetRenderer() )->TuneCull( true );

		renderer_string = "VBR";
	}
	else if( m_sfgui.GetRenderer().GetName() == "Vertex Array Renderer" ) {
		static_cast<sfg::VertexArrayRenderer*>( &m_sfgui.GetRenderer() )->TuneAlphaThreshold( .2f );
		static_cast<sfg::VertexArrayRenderer*>( &m_sfgui.GetRenderer() )->TuneCull( true );

		renderer_string = "VAR";
	}

	// Play around with resource manager.
	std::shared_ptr<sf::Font> my_font = std::make_shared<sf::Font>();
	my_font->loadFromFile( "data/linden_hill.otf" );
	m_desktop.GetEngine().GetResourceManager().AddFont( "custom_font", my_font );

	// Set properties.
	// Multiple properties can be set at once to save calls.

	// m_desktop.SetProperty( "Button#close:Normal", "Color", sf::Color::Yellow );
	// #close is sufficient since there is only 1 widget with this id
	// m_desktop.SetProperty( "#close", "FontName", "data/linden_hill.otf" );
	// m_desktop.SetProperty( "#close", "FontSize", 15.f );

	// We will batch the above properties into this call.
	m_desktop.SetProperties(
		"Window#second_window > Box > Label {"
		"	FontName: custom_font;"
		"	FontSize: 18;"
		"}"
		"Button#close:Normal {"
		"	Color: #FFFF00FF;"
		"}"
		"#close {"
		"	FontName: data/linden_hill.otf;"
		"	FontSize: 15;"
		"}"
	);

	// Create widgets.
	m_wndmain = sfg::Window::Create( sfg::Window::Style::TITLEBAR | sfg::Window::Style::BACKGROUND | sfg::Window::Style::RESIZE | sfg::Window::Style::CLOSE );
	m_wndmain->SetTitle( L"Example application" );

	auto btnaddbuttonh = sfg::Button::Create( L"Add button horizontally" );
	auto btnaddbuttonv = sfg::Button::Create( L"Add button vertically" );
	m_titlebar_toggle = sfg::ToggleButton::Create( "Toggle titlebar" );
	m_titlebar_toggle->SetActive( true );

	{
		sf::Image add_image;
		if( add_image.loadFromFile( "data/add.png" ) ) {
			auto image = sfg::Image::Create( add_image );
			btnaddbuttonh->SetImage( image );

			image = sfg::Image::Create( add_image );
			btnaddbuttonv->SetImage( image );
		}
	}

	auto btnhidewindow = sfg::Button::Create( L"Close window" );
	btnhidewindow->SetId( "close" );

	{
		sf::Image close_image;
		if( close_image.loadFromFile( "data/delete.png" ) ) {
			auto image = sfg::Image::Create( close_image );
			btnhidewindow->SetImage( image );
		}
	}

	auto btntogglespace = sfg::Button::Create( L"Box Spacing" );
	auto btnloadstyle = sfg::Button::Create( L"Load theme" );

	m_entry = sfg::Entry::Create( L"Type" );
	m_entry->SetRequisition( sf::Vector2f( 100.f, .0f ) );
	m_entry->AppendText( L" something!" );

	m_limit_check = sfg::CheckButton::Create( L"Limit to 4 chars" );
	m_limit_check->SetId( "limit_check" );

	auto password = sfg::Entry::Create();
	password->HideText( '*' );

	// Layout.
	auto boxtoolbar = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
	boxtoolbar->SetSpacing( 5.f );
	boxtoolbar->Pack( btnaddbuttonh, false );
	boxtoolbar->Pack( btnaddbuttonv, false );
	boxtoolbar->Pack( m_titlebar_toggle, false );
	boxtoolbar->Pack( btnhidewindow, false );
	boxtoolbar->Pack( m_entry, true );
	boxtoolbar->Pack( m_limit_check, false );

	auto frame1 = sfg::Frame::Create( L"Toolbar 1" );
	frame1->Add( boxtoolbar );

	auto boxtoolbar2 = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
	boxtoolbar2->SetSpacing( 5.f );
	boxtoolbar2->Pack( btntogglespace, false );
	boxtoolbar2->Pack( btnloadstyle, false );

	m_boxbuttonsh = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
	m_boxbuttonsh->SetSpacing( 5.f );

	m_boxbuttonsv = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	m_boxbuttonsv->SetSpacing( 5.f );

	auto username_entry = sfg::Entry::Create();
	username_entry->SetMaximumLength( 8 );

	m_progress = sfg::ProgressBar::Create( sfg::ProgressBar::Orientation::HORIZONTAL );
	m_progress->SetRequisition( sf::Vector2f( 0.f, 20.f ) );

	m_progress_vert = sfg::ProgressBar::Create( sfg::ProgressBar::Orientation::VERTICAL );
	m_progress_vert->SetRequisition( sf::Vector2f( 20.f, 0.f ) );

	auto separatorv = sfg::Separator::Create( sfg::Separator::Orientation::VERTICAL );

	m_table = sfg::Table::Create();
	m_table->Attach( sfg::Label::Create( L"Please login using your username and password (span example)." ), sf::Rect<sf::Uint32>( 0, 0, 2, 1 ), sfg::Table::FILL, sfg::Table::FILL | sfg::Table::EXPAND );
	m_table->Attach( sfg::Label::Create( L"Username:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( username_entry, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Label::Create( L"Password:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( password, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Button::Create( L"Login" ), sf::Rect<sf::Uint32>( 2, 1, 1, 2 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( separatorv, sf::Rect<sf::Uint32>( 3, 0, 1, 3 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( m_progress_vert, sf::Rect<sf::Uint32>( 4, 0, 1, 3 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->SetRowSpacings( 5.f );
	m_table->SetColumnSpacings( 5.f );

	m_scrolled_window_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

	for( int i = 0; i < 5; i++ ) {
		auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

		for( int j = 0; j < 20; j++ ) {
			box->Pack( sfg::Button::Create( L"One button among many" ), true );
		}

		m_scrolled_window_box->Pack( box, false );
	}

	m_scrolled_window = sfg::ScrolledWindow::Create();
	m_scrolled_window->SetRequisition( sf::Vector2f( .0f, 160.f ) );
	m_scrolled_window->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	m_scrolled_window->SetPlacement( sfg::ScrolledWindow::Placement::TOP_LEFT );
	m_scrolled_window->AddWithViewport( m_scrolled_window_box );

	auto scrollbar = sfg::Scrollbar::Create();
	scrollbar->SetRange( .0f, 100.f );

	m_scale = sfg::Scale::Create();
	m_scale->SetAdjustment( scrollbar->GetAdjustment() );
	m_scale->SetRequisition( sf::Vector2f( 100.f, .0f ) );
	boxtoolbar2->Pack( m_scale, false );

	m_combo_box = sfg::ComboBox::Create();

	for( int index = 0; index < 30; ++index ) {
		std::stringstream sstr;

		sstr << "Item " << index;

		m_combo_box->AppendItem( sstr.str() );
	}

	boxtoolbar2->Pack( m_combo_box, true );

	m_switch_renderer = sfg::Button::Create( "Renderer: " + renderer_string );

	boxtoolbar2->Pack( m_switch_renderer, false );

	auto frame2 = sfg::Frame::Create( L"Toolbar 2" );
	frame2->Add( boxtoolbar2 );
	frame2->SetAlignment( sf::Vector2f( .8f, .0f ) );

	auto separatorh = sfg::Separator::Create( sfg::Separator::Orientation::HORIZONTAL );

	auto box_image = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
	box_image->SetSpacing( 15.f );

	auto fixed_container = sfg::Fixed::Create();
	auto fixed_button = sfg::Button::Create( L"I'm at (34,61)" );
	fixed_container->Put( fixed_button, sf::Vector2f( 34.f, 61.f ) );
	box_image->Pack( fixed_container, false );

	sf::Image sfgui_logo;
	m_image = sfg::Image::Create();

	if( sfgui_logo.loadFromFile( "data/sfgui.png" ) ) {
		m_image->SetImage( sfgui_logo );
		box_image->Pack( m_image, false );
	}

	auto mirror_image = sfg::Button::Create( L"Mirror Image" );

	box_image->Pack( mirror_image, false );

	auto spinner_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

	m_spinner = sfg::Spinner::Create();
	m_spinner->SetRequisition( sf::Vector2f( 40.f, 40.f ) );
	m_spinner->Start();
	auto spinner_toggle = sfg::ToggleButton::Create( L"Spin" );
	spinner_toggle->SetActive( true );
	spinner_box->SetSpacing( 5.f );
	spinner_box->Pack( m_spinner, false );
	spinner_box->Pack( spinner_toggle, false );

	box_image->Pack( spinner_box, false );

	auto radio_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

	auto radio1 = sfg::RadioButton::Create( "Radio 1" );
	auto radio2 = sfg::RadioButton::Create( "Radio 2", radio1->GetGroup() );
	auto radio3 = sfg::RadioButton::Create( "Radio 3", radio2->GetGroup() );

	radio_box->Pack( radio1 );
	radio_box->Pack( radio2 );
	radio_box->Pack( radio3 );

	box_image->Pack( radio_box, false );

	auto spinbutton = sfg::SpinButton::Create( scrollbar->GetAdjustment() );
	spinbutton->SetRequisition( sf::Vector2f( 80.f, 0.f ) );
	spinbutton->SetDigits( 3 );

	auto spinbutton_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	spinbutton_box->Pack( spinbutton, false, false );

	box_image->Pack( spinbutton_box, false, false );

	auto aligned_combo_box = sfg::ComboBox::Create();
	aligned_combo_box->AppendItem( L"I'm way over here" );
	aligned_combo_box->AppendItem( L"Me too" );
	aligned_combo_box->AppendItem( L"Me three" );
	aligned_combo_box->SelectItem( 0 );

	auto alignment = sfg::Alignment::Create();
	alignment->Add( aligned_combo_box );
	box_image->Pack( alignment, true );
	alignment->SetAlignment( sf::Vector2f( 1.f, .5f ) );
	alignment->SetScale( sf::Vector2f( 0.f, .01f ) );

	auto boxmain = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	boxmain->SetSpacing( 5.f );
	boxmain->Pack( scrollbar, false );
	boxmain->Pack( m_progress, false );
	boxmain->Pack( frame1, false );
	boxmain->Pack( frame2, false );
	boxmain->Pack( m_boxbuttonsh, false );
	boxmain->Pack( m_boxbuttonsv, false );
	boxmain->Pack( box_image, true );
	boxmain->Pack( separatorh, false );
	boxmain->Pack( m_table, true );
	boxmain->Pack( m_scrolled_window );

	auto notebook1 = sfg::Notebook::Create();
	auto notebook2 = sfg::Notebook::Create();
	auto notebook3 = sfg::Notebook::Create();
	auto notebook4 = sfg::Notebook::Create();

	notebook1->SetTabPosition( sfg::Notebook::TabPosition::TOP );
	notebook2->SetTabPosition( sfg::Notebook::TabPosition::RIGHT );
	notebook3->SetTabPosition( sfg::Notebook::TabPosition::BOTTOM );
	notebook4->SetTabPosition( sfg::Notebook::TabPosition::LEFT );

	auto vertigo_box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
	auto vertigo_button = sfg::Button::Create( L"Vertigo" );
	vertigo_box->Pack( vertigo_button, true, true );

	notebook1->AppendPage( boxmain, sfg::Label::Create( "Page Name Here" ) );
	notebook1->AppendPage( notebook2, sfg::Label::Create( "Another Page" ) );
	notebook2->AppendPage( notebook3, sfg::Label::Create( "Yet Another Page" ) );
	notebook2->AppendPage( sfg::Label::Create( L"" ), sfg::Label::Create( "Dummy Page" ) );
	notebook3->AppendPage( notebook4, sfg::Label::Create( "And Another Page" ) );
	notebook3->AppendPage( sfg::Label::Create( L"" ), sfg::Label::Create( "Dummy Page" ) );
	notebook4->AppendPage( vertigo_box, sfg::Label::Create( "And The Last Page" ) );
	notebook4->AppendPage( sfg::Label::Create( L"" ), sfg::Label::Create( "Dummy Page" ) );

	m_wndmain->Add( notebook1 );

	// Signals.
	m_wndmain->GetSignal( sfg::Window::OnCloseButton ).Connect( std::bind( &SampleApp::OnHideWindowClicked, this ) );
	btnaddbuttonh->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnAddButtonHClick, this ) );
	btnaddbuttonv->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnAddButtonVClick, this ) );
	m_titlebar_toggle->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnToggleTitlebarClick, this ) );
	btnhidewindow->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnHideWindowClicked, this ) );
	btntogglespace->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnToggleSpaceClick, this ) );
	m_limit_check->GetSignal( sfg::ToggleButton::OnToggle ).Connect( std::bind( &SampleApp::OnLimitCharsToggle, this ) );
	btnloadstyle->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnLoadThemeClick, this ) );
	m_scale->GetAdjustment()->GetSignal( sfg::Adjustment::OnChange ).Connect( std::bind( &SampleApp::OnAdjustmentChange, this ) );
	spinner_toggle->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnToggleSpinner, this ) );
	mirror_image->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnMirrorImageClick, this ) );
	m_switch_renderer->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &SampleApp::OnSwitchRendererClick, this ) );

	spinbutton->SetValue( 20.f );
	spinbutton->GetAdjustment()->SetMinorStep( .8f );

	m_wndmain->SetPosition( sf::Vector2f( 100.f, 100.f ) );

	// Another window
	auto second_window = sfg::Window::Create( sfg::Window::TITLEBAR | sfg::Window::BACKGROUND | sfg::Window::RESIZE );
	second_window->SetId( "second_window" );
	second_window->SetTitle( "Resize this window to see ad-hoc wrapping." );
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );

	auto lipsum = sfg::Label::Create(
		"Nullam ut ante leo. Quisque consequat condimentum pulvinar. "
		"Duis a enim sapien, ut vestibulum est. Vestibulum commodo, orci non gravida. "
		"Aliquam sed pretium lacus. "
		"Nullam placerat mauris vel nulla sagittis pellentesque. "
		"Suspendisse in justo dui.\n"
		"Ut dolor massa, gravida eu facilisis convallis, convallis sed odio.\n"
		"Nunc placerat consequat vehicula."
	);

	lipsum->SetRequisition( sf::Vector2f( 400.f, 0.f ) );
	lipsum->SetLineWrap( true );

	box->Pack( lipsum );
	second_window->Add( box );
	second_window->SetPosition( sf::Vector2f( 10.f, 10.f ) );
	second_window->SetId( "second_window" );
	m_desktop.Add( second_window );

	auto third_window = sfg::Window::Create( sfg::Window::TITLEBAR | sfg::Window::BACKGROUND | sfg::Window::RESIZE );

	m_gl_canvas = sfg::Canvas::Create( true );
	m_gl_canvas->SetRequisition( sf::Vector2f( 200.f, 150.f ) );

	third_window->Add( m_gl_canvas );

	third_window->SetId( "third_window" );
	third_window->SetTitle( "Embedded OpenGL drawing" );
	third_window->SetPosition( sf::Vector2f( 480.f, 20.f ) );
	m_desktop.Add( third_window );

	sf::Texture texture;
	texture.loadFromImage( sfgui_logo );
	m_canvas_sprite.setTexture( texture );

	auto fourth_window = sfg::Window::Create( sfg::Window::TITLEBAR | sfg::Window::BACKGROUND | sfg::Window::RESIZE );

	m_sfml_canvas = sfg::Canvas::Create();
	m_sfml_canvas->SetRequisition( sf::Vector2f( static_cast<float>( texture.getSize().x ), static_cast<float>( texture.getSize().y ) ) );

	fourth_window->Add( m_sfml_canvas );

	fourth_window->SetId( "fourth_window" );
	fourth_window->SetTitle( "Embedded SFML drawing" );
	fourth_window->SetPosition( sf::Vector2f( 760.f, 20.f ) );
	m_desktop.Add( fourth_window );

	// Add window to desktop
	m_desktop.Add( m_wndmain );

	m_fps_counter = 0;
	m_fps_clock.restart();

	sf::Clock clock;
	sf::Clock frame_time_clock;

	sf::Int64 frame_times[5000];
	std::size_t frame_times_index = 0;

	std::fill( std::begin( frame_times ), std::end( frame_times ), 0 );

	m_desktop.Update( 0.f );

	while( m_window.isOpen() ) {
		while( m_window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed ) {
				m_window.close();
			}

			m_desktop.HandleEvent( event );
		}

		m_window.draw( m_background_sprite );

		auto microseconds = clock.getElapsedTime().asMicroseconds();

		// Only update every 5ms
		if( microseconds > 5000 ) {
			m_desktop.Update( static_cast<float>( microseconds ) / 1000000.f );
			clock.restart();

			// Only refresh canvas contents every 5ms too
			m_gl_canvas->Bind();
			m_gl_canvas->Clear( sf::Color( 0, 0, 0, 0 ), true );
			RenderCustomGL();
			m_gl_canvas->Display();
			m_gl_canvas->Unbind();

			m_sfml_canvas->Bind();
			m_sfml_canvas->Clear( sf::Color( 0, 0, 0, 0 ) );
			RenderCustomSFML();
			m_sfml_canvas->Display();
			m_sfml_canvas->Unbind();

			m_window.setActive( true );
		}

		m_sfgui.Display( m_window );

		m_window.display();

		auto frame_time = frame_time_clock.getElapsedTime().asMicroseconds();
		frame_time_clock.restart();

		frame_times[ frame_times_index ] = frame_time;
		frame_times_index = ( frame_times_index + 1 ) % 5000;

		if( m_fps_clock.getElapsedTime().asMicroseconds() >= 1000000 ) {
			m_fps_clock.restart();

			sf::Int64 total_time = 0;

			for( std::size_t index = 0; index < 5000; ++index ) {
				total_time += frame_times[index];
			}

			std::stringstream sstr;
			sstr << "SFGUI test -- FPS: " << m_fps_counter << " -- Frame Time (microsecs): min: "
			<< *std::min_element( frame_times, frame_times + 5000 ) << " max: "
			<< *std::max_element( frame_times, frame_times + 5000 ) << " avg: "
			<< static_cast<float>( total_time ) / 5000.f;

			m_window.setTitle( sstr.str() );

			m_fps_counter = 0;
		}

		++m_fps_counter;
	}

	glDeleteLists( m_custom_draw_display_list, 1 );
}

void SampleApp::OnAddButtonHClick() {
	auto button = sfg::Button::Create( L"New ->" );

	auto ouchy = std::make_shared<Ouchy>( button );
	Ouchy::m_ouchies.push_back( ouchy );

	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Ouchy::DoOuch, ouchy.get() ) );

	m_boxbuttonsh->Pack( button, true );
}

void SampleApp::OnAddButtonVClick() {
	auto button = sfg::Button::Create( L"<- New" );

	auto ouchy = std::make_shared<Ouchy>( button );
	Ouchy::m_ouchies.push_back( ouchy );

	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Ouchy::DoOuch, ouchy.get() ) );

	m_boxbuttonsv->Pack( button, false );
}

void SampleApp::OnToggleTitlebarClick() {
	m_wndmain->SetStyle( m_wndmain->GetStyle() ^ sfg::Window::TITLEBAR );
}

void SampleApp::OnHideWindowClicked() {
	m_wndmain->Show( !m_wndmain->IsLocallyVisible() );
}

void SampleApp::OnToggleSpaceClick() {
	if( m_scrolled_window_box->GetSpacing() > .0f ) {
		m_scrolled_window_box->SetSpacing( .0f );
	}
	else {
		m_scrolled_window_box->SetSpacing( 40.f );
	}
}

void SampleApp::OnLimitCharsToggle() {
	if( m_limit_check->IsActive() ) {
		m_entry->SetMaximumLength( 4 );
	}
	else {
		m_entry->SetMaximumLength( 0 );
	}
}

void SampleApp::OnLoadThemeClick() {
	m_desktop.LoadThemeFromFile( "data/example.theme" );
}

void SampleApp::OnAdjustmentChange() {
	m_progress->SetFraction( m_scale->GetValue() / 100.f );
	m_progress_vert->SetFraction( m_scale->GetValue() / 100.f );
}

void SampleApp::OnToggleSpinner() {
	if( !m_spinner->Started() ) {
		m_spinner->Start();
	}
	else {
		m_spinner->Stop();
	}
}

void SampleApp::OnMirrorImageClick() {
	auto image = m_image->GetImage();

	for( unsigned int height_index = 0; height_index < image.getSize().y; ++height_index ) {
		for( unsigned int width_index = 0; width_index < image.getSize().x / 2; ++width_index ) {
			auto color0 = image.getPixel( width_index, height_index );
			auto color1 = image.getPixel( image.getSize().x - width_index - 1, height_index );
			image.setPixel( width_index, height_index, color1 );
			image.setPixel( image.getSize().x - width_index - 1, height_index, color0 );
		}
	}

	m_image->SetImage( image );
}

void SampleApp::OnSwitchRendererClick() {
	if( sfg::Renderer::Get().GetName() == "Non-Legacy Renderer" ) {
		auto renderer = sfg::VertexBufferRenderer::Create();

		sfg::Renderer::Set( renderer );

		renderer->TuneUseFBO( true );
		renderer->TuneAlphaThreshold( .2f );
		renderer->TuneCull( true );

		m_switch_renderer->SetLabel( "Renderer: VBR" );

		m_desktop.Refresh();
		return;
	}

	if( sfg::Renderer::Get().GetName() == "Vertex Buffer Renderer" ) {
		auto renderer = sfg::VertexArrayRenderer::Create();

		sfg::Renderer::Set( renderer );

		renderer->TuneAlphaThreshold( .2f );
		renderer->TuneCull( true );

		m_switch_renderer->SetLabel( "Renderer: VAR" );

		m_desktop.Refresh();
		return;
	}

	if( sfg::NonLegacyRenderer::IsAvailable() ) {
		auto renderer = sfg::NonLegacyRenderer::Create();

		sfg::Renderer::Set( renderer );

		renderer->TuneUseFBO( true );
		renderer->TuneCull( true );

		m_switch_renderer->SetLabel( "Renderer: NLR" );

		m_desktop.Refresh();
	}
	else if( sfg::VertexBufferRenderer::IsAvailable() ) {
		auto renderer = sfg::VertexBufferRenderer::Create();

		sfg::Renderer::Set( renderer );

		renderer->TuneUseFBO( true );
		renderer->TuneAlphaThreshold( .2f );
		renderer->TuneCull( true );

		m_switch_renderer->SetLabel( "Renderer: VBR" );

		m_desktop.Refresh();
	}
}

void SampleApp::RenderCustomGL() {
	static sf::Clock clock;

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glTranslatef( 0.f, 0.f, -3.f );

	glRotatef( clock.getElapsedTime().asSeconds() * 50.f, 1.f, 0.f, 0.f );
	glRotatef( clock.getElapsedTime().asSeconds() * 30.f, 0.f, 1.f, 0.f );
	glRotatef( clock.getElapsedTime().asSeconds() * 90.f, 0.f, 0.f, 1.f );

	glViewport( 0, 0, static_cast<int>( std::floor( m_gl_canvas->GetAllocation().width + .5f ) ), static_cast<int>( std::floor( m_gl_canvas->GetAllocation().height + .5f ) ) );

	if( !m_custom_draw_display_list ) {
		m_custom_draw_display_list = glGenLists( 1 );

		glNewList( m_custom_draw_display_list, GL_COMPILE );

		glEnable( GL_DEPTH_TEST );
		glDepthMask( GL_TRUE );

		glDisable( GL_TEXTURE_2D );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();

		static const auto pi = 3.1415926535897932384626433832795f;
		static const auto fov = 90.f;
		static const auto near_distance = 1.f;
		static const auto far_distance = 20.f;

		// We set the proper aspect ratio using the dimensions of our Canvas.
		auto aspect = m_gl_canvas->GetAllocation().width / m_gl_canvas->GetAllocation().height;
		auto frustum_height = std::tan( fov / 360 * pi ) * near_distance;
		auto frustum_width = frustum_height * aspect;

		glFrustum( -frustum_width, frustum_width, -frustum_height, frustum_height, near_distance, far_distance );

		glBegin( GL_QUADS );

		glColor3f( 0.f, 0.f, 1.f );
		glVertex3f( -1.f, -1.f, -1.f );
		glVertex3f( -1.f, 1.f, -1.f );
		glVertex3f( 1.f, 1.f, -1.f );
		glVertex3f( 1.f, -1.f, -1.f );

		glColor3f( 0.f, 1.f, 0.f );
		glVertex3f( -1.f, -1.f, 1.f );
		glVertex3f( -1.f, 1.f, 1.f );
		glVertex3f( 1.f, 1.f, 1.f );
		glVertex3f( 1.f, -1.f, 1.f );

		glColor3f( 0.f, 1.f, 1.f );
		glVertex3f( -1.f, -1.f, -1.f );
		glVertex3f( -1.f, 1.f, -1.f );
		glVertex3f( -1.f, 1.f, 1.f );
		glVertex3f( -1.f, -1.f, 1.f );

		glColor3f( 1.f, 0.f, 0.f );
		glVertex3f( 1.f, -1.f, -1.f );
		glVertex3f( 1.f, 1.f, -1.f );
		glVertex3f( 1.f, 1.f, 1.f );
		glVertex3f( 1.f, -1.f, 1.f );

		glColor3f( 1.f, 0.f, 1.f );
		glVertex3f( -1.f, -1.f, 1.f );
		glVertex3f( -1.f, -1.f, -1.f );
		glVertex3f( 1.f, -1.f, -1.f );
		glVertex3f( 1.f, -1.f, 1.f );

		glColor3f( 1.f, 1.f, 0.f );
		glVertex3f( -1.f, 1.f, 1.f );
		glVertex3f( -1.f, 1.f, -1.f );
		glVertex3f( 1.f, 1.f, -1.f );
		glVertex3f( 1.f, 1.f, 1.f );

		glEnd();

		glPopMatrix();

		glEnable( GL_TEXTURE_2D );
		glDisable( GL_DEPTH_TEST );

		glEndList();
	}

	glCallList( m_custom_draw_display_list );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glViewport( 0, 0, static_cast<int>( m_window.getSize().x ), static_cast<int>( m_window.getSize().y ) );
}

void SampleApp::RenderCustomSFML() {
	m_sfml_canvas->Draw( m_canvas_sprite );
}

int main() {
	SampleApp app;
	app.Run();

	return 0;
}
