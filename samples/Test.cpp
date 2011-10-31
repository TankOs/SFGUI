#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <boost/lexical_cast.hpp>

class SampleApp {
	public:
		void Run();

	private:
		void OnAddButtonHClick();
		void OnAddButtonVClick();
		void OnNewButtonClick();
		void OnToggleTitlebarClick();
		void OnHideWindowClicked();
		void OnRangeValueChange();
		void OnToggleSpaceClick();

		sfg::Window::Ptr m_wndmain;
		sfg::Box::Ptr m_boxbuttonsh;
		sfg::Box::Ptr m_boxbuttonsv;
		sfg::Entry::Ptr m_entry;
		sfg::Table::Ptr m_table;
		sfg::ScrolledWindow::Ptr m_scrolled_window;
		sfg::Box::Ptr m_scrolled_window_box;

		unsigned int m_fps_counter;
		sf::Clock m_fps_clock;
};

void SampleApp::Run() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event  event;

	window.SetFramerateLimit( 60 );
	//window.UseVerticalSync( true );

	sfg::Context::Get().GetEngine().SetProperty<sf::Color>( "Button#close:Normal > Label", "Color", sf::Color::Red );

	// Create widgets.
	m_wndmain = sfg::Window::Create();
	m_wndmain->SetTitle( L"Example application" );
	m_wndmain->SetBorderWidth( 10.f );

	sfg::Button::Ptr btnaddbuttonh( sfg::Button::Create( L"Add button horizontally" ) );
	sfg::Button::Ptr btnaddbuttonv( sfg::Button::Create( L"Add button vertically" ) );
	sfg::Button::Ptr btntoggletitlebar( sfg::Button::Create( L"Toggle titlebar" ) );
	sfg::Button::Ptr btnhidewindow( sfg::Button::Create( L"Close window" ) );
	btnhidewindow->SetId( "close" );

	sfg::Button::Ptr btntogglespace( sfg::Button::Create( L"Box Spacing") );

	m_entry = sfg::Entry::Create( L"Type something!" );
	m_entry->SetRequisition( sf::Vector2f( 100.f, 0.f ) );

	sfg::Label::Ptr test_label( sfg::Label::Create( L"Foobar?" ) );
	sfg::Label::Ptr another_label( sfg::Label::Create( L"Meow?" ) );

	sfg::Entry::Ptr password( sfg::Entry::Create() );
	password->HideText( '*' );

	// Layout.
	sfg::Box::Ptr boxtoolbar( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxtoolbar->SetSpacing( 5.f );
	boxtoolbar->Pack( btnaddbuttonh, false );
	boxtoolbar->Pack( btnaddbuttonv, false );
	boxtoolbar->Pack( btntoggletitlebar, false );
	boxtoolbar->Pack( btnhidewindow, false );
	boxtoolbar->Pack( m_entry, true );
	boxtoolbar->Pack( btntogglespace, false );

	m_boxbuttonsh = sfg::Box::Create( sfg::Box::Horizontal );
	m_boxbuttonsh->SetSpacing( 5.f );

	m_boxbuttonsv = sfg::Box::Create( sfg::Box::Vertical );
	m_boxbuttonsv->SetSpacing( 5.f );

	m_table = sfg::Table::Create();
	m_table->Attach( sfg::Label::Create( L"Username:" ), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Entry::Create(), sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Label::Create( L"Password:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( password, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Button::Create( L"Login" ), sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), 0, 0 );
	m_table->SetRowSpacings( 5.f );
	m_table->SetColumnSpacings( 5.f );

	m_scrolled_window_box = sfg::Box::Create( sfg::Box::Vertical );

	for( int i = 0; i < 7; i++ ) {
		sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::Horizontal );

		for( int j = 0; j < 5; j++ ) {
			box->Pack( sfg::Button::Create( L"One button among many" ), true );
		}

		m_scrolled_window_box->Pack( box, false );
	}

	m_scrolled_window = sfg::ScrolledWindow::Create();
	m_scrolled_window->SetRequisition( sf::Vector2f( .0f, 200.f ) );
	m_scrolled_window->SetScrollbarPolicy( sfg::ScrolledWindow::HorizontalAutomatic | sfg::ScrolledWindow::VerticalAutomatic );
	m_scrolled_window->SetPlacement( sfg::ScrolledWindow::TopLeft );
	m_scrolled_window->Add( m_scrolled_window_box );

	sfg::Scrollbar::Ptr scrollbar( sfg::Scrollbar::Create() );
	scrollbar->SetRange( 0.f, 100.f );

	sfg::Box::Ptr  boxmain( sfg::Box::Create( sfg::Box::Vertical ) );
	boxmain->SetSpacing( 5.f );
	boxmain->Pack( scrollbar, false );
	boxmain->Pack( boxtoolbar, false );
	boxmain->Pack( m_boxbuttonsh, false );
	boxmain->Pack( m_boxbuttonsv, false );
	boxmain->Pack( m_table );
	//boxmain->Pack( m_scrolled_window );

	m_wndmain->Add( boxmain );

	// Signals.
	btnaddbuttonh->OnClick.Connect( &SampleApp::OnAddButtonHClick, this );
	btnaddbuttonv->OnClick.Connect( &SampleApp::OnAddButtonVClick, this );
	btntoggletitlebar->OnClick.Connect( &SampleApp::OnToggleTitlebarClick, this );
	btnhidewindow->OnClick.Connect( &SampleApp::OnHideWindowClicked, this );
	btntogglespace->OnClick.Connect( &SampleApp::OnToggleSpaceClick, this );

	// Load theme.
	//sfg::ThemeLoader::LoadFromFile<sfg::loaders::YAML>( "data/default.yaml", sfg::Context::Get().GetEngine() );
	// TODO: Reinvalidate widgets when loading theme.

	m_fps_counter = 0;
	m_fps_clock.Reset();

	while( window.IsOpened() ) {
		while( window.PollEvent( event ) ) {
			if( m_wndmain->HandleEvent( event ) == sfg::Widget::EatEvent ) {
				continue;
			}

			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
		}

		if( m_fps_clock.GetElapsedTime() >= 1000 ) {
			m_fps_clock.Reset();

			window.SetTitle( std::string( "SFGUI test -- FPS: " ) + boost::lexical_cast<std::string>( m_fps_counter ) );

			m_fps_counter = 0;
		}

		++m_fps_counter;

		window.Clear( sf::Color( 80, 80, 80 ) );
		m_wndmain->Expose( window );
		window.Display();
	}
}

void SampleApp::OnAddButtonHClick() {
	sfg::Button::Ptr  button( sfg::Button::Create( L"New ->" ) );

	boost::shared_dynamic_cast<sfg::Label>( button->GetChild() )->SetAlignment( sf::Vector2f( 1.f, .5f ) );
	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );

	m_boxbuttonsh->Pack( button, true );
}

void SampleApp::OnAddButtonVClick() {
	sfg::Button::Ptr  button( sfg::Button::Create( L"<- New" ) );

	boost::shared_dynamic_cast<sfg::Label>( button->GetChild() )->SetAlignment( sf::Vector2f( 0.f, .5f ) );
	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );

	m_boxbuttonsv->Pack( button, false );
}

void SampleApp::OnNewButtonClick() {
	//sfg::Button::Ptr  button( boost::shared_dynamic_cast<sfg::Button>( widget ) );
	//button->SetLabel( "Ouch" );
}

void SampleApp::OnToggleTitlebarClick() {
	m_wndmain->SetStyle( m_wndmain->GetStyle() ^ sfg::Window::Titlebar );
}

void SampleApp::OnHideWindowClicked() {
	m_wndmain->Show( !m_wndmain->IsVisible() );
}

void SampleApp::OnToggleSpaceClick() {
	if( m_scrolled_window_box->GetSpacing() > 0.f ) {
		m_scrolled_window_box->SetSpacing( 0.f );
		m_scrolled_window_box->SetBorderWidth( 0.f );
	}
	else {
		m_scrolled_window_box->SetSpacing( 10.f );
		m_scrolled_window_box->SetBorderWidth( 30.f );
	}
}

int main() {
	SampleApp  app;
	app.Run();
	return 0;
}
