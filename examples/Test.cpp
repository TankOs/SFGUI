#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Viewport.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/CheckButton.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <sstream>

class SampleApp {
	public:
		SampleApp();
		void Run();

	private:
		void OnAddButtonHClick();
		void OnAddButtonVClick();
		void OnToggleTitlebarClick();
		void OnHideWindowClicked();
		void OnRangeValueChange();
		void OnToggleSpaceClick();
		void OnLimitCharsToggle();

		sfg::Window::Ptr m_wndmain;
		sfg::Box::Ptr m_boxbuttonsh;
		sfg::Box::Ptr m_boxbuttonsv;
		sfg::Entry::Ptr m_entry;
		sfg::Table::Ptr m_table;
		sfg::ScrolledWindow::Ptr m_scrolled_window;
		sfg::Box::Ptr m_scrolled_window_box;
		sfg::ToggleButton::Ptr m_titlebar_toggle;
		sfg::CheckButton::Ptr m_limit_check;

		sfg::Desktop m_desktop;

		unsigned int m_fps_counter;
		sf::Clock m_fps_clock;
};

class Ouchy : public std::enable_shared_from_this<Ouchy> {
	public:
		typedef std::shared_ptr<Ouchy>  Ptr; //!< Shared pointer.
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
	m_desktop( sf::FloatRect( .0f, .0f, 1024.f, 768.f ) )
{
}

void SampleApp::Run() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event  event;

	//window.SetFramerateLimit( 60 );
	//window.UseVerticalSync( true );

	sfg::Context::Get().GetEngine().SetProperty<sf::Color>( "Button#close:Normal > Label", "Color", sf::Color::Red );

	// Create widgets.
	m_wndmain = sfg::Window::Create();
	m_wndmain->SetTitle( L"Example application" );
	m_wndmain->SetBorderWidth( 10.f );

	sfg::Button::Ptr btnaddbuttonh( sfg::Button::Create( L"Add button horizontally" ) );
	sfg::Button::Ptr btnaddbuttonv( sfg::Button::Create( L"Add button vertically" ) );
	m_titlebar_toggle = sfg::ToggleButton::Create( "Toggle titlebar" );
	sfg::Button::Ptr btnhidewindow( sfg::Button::Create( L"Close window" ) );
	btnhidewindow->SetId( "close" );

	sfg::Button::Ptr btntogglespace( sfg::Button::Create( L"Box Spacing") );

	m_entry = sfg::Entry::Create( L"Type something!" );
	m_entry->SetRequisition( sf::Vector2f( 100.f, .0f ) );

	m_limit_check = sfg::CheckButton::Create( L"Limit to 4 chars" );

	sfg::Label::Ptr test_label( sfg::Label::Create( L"Foobar?" ) );
	sfg::Label::Ptr another_label( sfg::Label::Create( L"Meow?" ) );

	sfg::Entry::Ptr password( sfg::Entry::Create() );
	password->HideText( '*' );

	// Layout.
	sfg::Box::Ptr boxtoolbar( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxtoolbar->SetSpacing( 5.f );
	boxtoolbar->Pack( btnaddbuttonh, false );
	boxtoolbar->Pack( btnaddbuttonv, false );
	boxtoolbar->Pack( m_titlebar_toggle, false );
	boxtoolbar->Pack( btnhidewindow, false );
	boxtoolbar->Pack( m_entry, true );
	boxtoolbar->Pack( m_limit_check, false );
	boxtoolbar->Pack( btntogglespace, false );

	m_boxbuttonsh = sfg::Box::Create( sfg::Box::Horizontal );
	m_boxbuttonsh->SetSpacing( 5.f );

	m_boxbuttonsv = sfg::Box::Create( sfg::Box::Vertical );
	m_boxbuttonsv->SetSpacing( 5.f );

	sfg::Entry::Ptr username_entry( sfg::Entry::Create() );
	username_entry->SetMaximumLength( 8 );

	m_table = sfg::Table::Create();
	m_table->Attach( sfg::Label::Create( L"Please login using your username and password (span example)." ), sf::Rect<sf::Uint32>( 0, 0, 2, 1 ), sfg::Table::FILL, sfg::Table::FILL | sfg::Table::EXPAND );
	m_table->Attach( sfg::Label::Create( L"Username:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( username_entry, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Label::Create( L"Password:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( password, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Button::Create( L"Login" ), sf::Rect<sf::Uint32>( 2, 1, 1, 2 ), sfg::Table::FILL, sfg::Table::FILL );
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
	m_scrolled_window->AddWithViewport( m_scrolled_window_box );

	sfg::Scrollbar::Ptr scrollbar( sfg::Scrollbar::Create() );
	scrollbar->SetRange( .0f, 100.f );

	sfg::Box::Ptr  boxmain( sfg::Box::Create( sfg::Box::Vertical ) );
	boxmain->SetSpacing( 5.f );
	boxmain->Pack( scrollbar, false );
	boxmain->Pack( boxtoolbar, false );
	boxmain->Pack( m_boxbuttonsh, false );
	boxmain->Pack( m_boxbuttonsv, false );
	boxmain->Pack( m_table, true );
	boxmain->Pack( m_scrolled_window );

	m_wndmain->Add( boxmain );

	// Signals.
	btnaddbuttonh->OnClick.Connect( &SampleApp::OnAddButtonHClick, this );
	btnaddbuttonv->OnClick.Connect( &SampleApp::OnAddButtonVClick, this );
	m_titlebar_toggle->OnClick.Connect( &SampleApp::OnToggleTitlebarClick, this );
	btnhidewindow->OnClick.Connect( &SampleApp::OnHideWindowClicked, this );
	btntogglespace->OnClick.Connect( &SampleApp::OnToggleSpaceClick, this );
	m_limit_check->OnToggle.Connect( &SampleApp::OnLimitCharsToggle, this );

	m_wndmain->SetPosition( sf::Vector2f( 100.f, 100.f ) );

	// Another window
	sfg::Window::Ptr second_window( sfg::Window::Create() );
	second_window->SetBorderWidth( 10.f );
	second_window->SetTitle( "Another window" );
	sfg::Box::Ptr box( sfg::Box::Create( sfg::Box::Vertical, 5.f ) );
	box->Pack( sfg::Label::Create( "Aliquam sed pretium lacus." ), false );
	box->Pack( sfg::Label::Create( "Nullam placerat mauris vel nulla sagittis pellentesque." ), false );
	box->Pack( sfg::Label::Create( "Suspendisse in justo dui." ), false );
	box->Pack( sfg::Label::Create( "Ut dolor massa, gravida eu facilisis convallis, convallis sed odio." ), false );
	box->Pack( sfg::Label::Create( "Nunc placerat consequat vehicula." ), false );
	second_window->Add( box );
	second_window->SetPosition( sf::Vector2f( 10.f, 10.f ) );
	m_desktop.Add( second_window );

	// Add window to desktop
	m_desktop.Add( m_wndmain );

	m_fps_counter = 0;
	m_fps_clock.Reset();

	while( window.IsOpened() ) {
		while( window.PollEvent( event ) ) {
			m_desktop.HandleEvent( event );

			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
		}

		if( m_fps_clock.GetElapsedTime() >= 1000 ) {
			m_fps_clock.Reset();

			std::stringstream sstr;
			sstr << m_fps_counter;
			window.SetTitle( std::string( "SFGUI test -- FPS: " ) + sstr.str() );

			m_fps_counter = 0;
		}

		++m_fps_counter;

		window.Clear( sf::Color( 80, 80, 80 ) );
		m_desktop.Expose( window );
		window.Display();
	}
}

void SampleApp::OnAddButtonHClick() {
	sfg::Button::Ptr  button( sfg::Button::Create( L"New ->" ) );

	Ouchy::Ptr ouchy( new Ouchy( button ) );
	Ouchy::m_ouchies.push_back( ouchy );

	button->OnClick.Connect( &Ouchy::DoOuch, ouchy.get() );

	m_boxbuttonsh->Pack( button, true );
}

void SampleApp::OnAddButtonVClick() {
	sfg::Button::Ptr  button( sfg::Button::Create( L"<- New" ) );

	Ouchy::Ptr ouchy( new Ouchy( button ) );
	Ouchy::m_ouchies.push_back( ouchy );

	button->OnClick.Connect( &Ouchy::DoOuch, ouchy.get() );

	m_boxbuttonsv->Pack( button, false );
}

void SampleApp::OnToggleTitlebarClick() {
	m_wndmain->SetStyle( m_wndmain->GetStyle() ^ sfg::Window::Titlebar );
}

void SampleApp::OnHideWindowClicked() {
	m_wndmain->Show( !m_wndmain->IsVisible() );
}

void SampleApp::OnToggleSpaceClick() {
	if( m_scrolled_window_box->GetSpacing() > .0f ) {
		m_scrolled_window_box->SetSpacing( .0f );
		m_scrolled_window_box->SetBorderWidth( .0f );
	}
	else {
		m_scrolled_window_box->SetSpacing( 10.f );
		m_scrolled_window_box->SetBorderWidth( 30.f );
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

int main() {
	SampleApp  app;
	app.Run();
	return 0;
}
