#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Engines/BREW.hpp>
//#include <SFGUI/Loaders/YAML.hpp>
#include <SFGUI/ThemeLoader.hpp>
#include <SFGUI/Context.hpp>

#include <SFML/Graphics.hpp>

class SampleApp {
	public:
		void Run();

	private:
		void OnAddButtonHClick( sfg::Widget::Ptr widget );
		void OnAddButtonVClick( sfg::Widget::Ptr widget );
		void OnNewButtonClick( sfg::Widget::Ptr widget );
		void OnToggleTitlebarClick( sfg::Widget::Ptr widget );
		void OnHideWindowClicked( sfg::Widget::Ptr widget );

		sfg::Window::Ptr m_wndmain;
		sfg::Box::Ptr m_boxbuttonsh;
		sfg::Box::Ptr m_boxbuttonsv;
		sfg::Entry::Ptr m_entry;
};

void SampleApp::Run() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event  event;

	window.SetFramerateLimit( 60 );
	//window.UseVerticalSync( true );

	// Create widgets.
	m_wndmain = sfg::Window::Create();
	m_wndmain->SetName( "wndmain" );
	m_wndmain->SetTitle( L"Example application" );
	m_wndmain->SetBorderWidth( 10.f );

	sfg::Button::Ptr  btnaddbuttonh( sfg::Button::Create( L"Add button horizontally" ) );
	sfg::Button::Ptr  btnaddbuttonv( sfg::Button::Create( L"Add button vertically" ) );
	sfg::Button::Ptr  btntoggletitlebar( sfg::Button::Create( L"Toggle titlebar" ) );
	sfg::Button::Ptr  btnhidewindow( sfg::Button::Create( L"Close window" ) );

	m_entry = sfg::Entry::Create();

	btnaddbuttonh->SetProperty( "Button.Normal.BackgroundColor", sf::Color( 0xFF, 0x00, 0x00 ) );
	btnaddbuttonh->SetProperty( "Button.Hover.BackgroundColor", sf::Color( 0xFF, 0x99, 0x99 ) );
	btnaddbuttonv->SetProperty( "Button.Normal.BackgroundColor", sf::Color( 0x00, 0x00, 0x55 ) );
	btnaddbuttonv->SetProperty( "Button.Hover.BackgroundColor", sf::Color( 0x55, 0x55, 0xBB ) );

	// Layout.
	sfg::Box::Ptr  boxtoolbar( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxtoolbar->SetName( "boxtoolbar" );
	boxtoolbar->SetSpacing( 5.f );
	boxtoolbar->Pack( btnaddbuttonh, false );
	boxtoolbar->Pack( btnaddbuttonv, false );
	boxtoolbar->Pack( btntoggletitlebar, false );
	boxtoolbar->Pack( btnhidewindow, false );
	boxtoolbar->Pack( m_entry, true );

	m_boxbuttonsh = sfg::Box::Create( sfg::Box::Horizontal );
	m_boxbuttonsh->SetSpacing( 5.f );

	m_boxbuttonsv = sfg::Box::Create( sfg::Box::Vertical );
	m_boxbuttonsv->SetSpacing( 5.f );

	sfg::Box::Ptr  boxmain( sfg::Box::Create( sfg::Box::Vertical ) );
	boxmain->SetSpacing( 5.f );
	boxmain->Pack( boxtoolbar, false );
	boxmain->Pack( m_boxbuttonsh, false );
	boxmain->Pack( m_boxbuttonsv, false );

	m_wndmain->Add( boxmain );

	// Signals.
	btnaddbuttonh->OnClick.Connect( &SampleApp::OnAddButtonHClick, this );
	btnaddbuttonv->OnClick.Connect( &SampleApp::OnAddButtonVClick, this );
	btntoggletitlebar->OnClick.Connect( &SampleApp::OnToggleTitlebarClick, this );
	btnhidewindow->OnClick.Connect( &SampleApp::OnHideWindowClicked, this );

	// Load theme.
	//sfg::ThemeLoader::LoadFromFile<sfg::loaders::YAML>( "data/default.yaml", sfg::Context::Get().GetEngine() );
	// TODO: Reinvalidate widgets when loading theme.

	while( window.IsOpened() ) {
		while( window.PollEvent( event ) ) {
			if( m_wndmain->HandleEvent( event ) == sfg::Widget::EatEvent ) {
				continue;
			}

			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
		}

		window.Clear( sf::Color( 80, 80, 80 ) );
		m_wndmain->Expose( window );
		window.Display();
	}
}

void SampleApp::OnAddButtonHClick( sfg::Widget::Ptr /*widget*/ ) {
	sfg::Button::Ptr  button( sfg::Button::Create( L"New ->" ) );

	boost::shared_dynamic_cast<sfg::Label>( button->GetChild() )->SetAlignment( sf::Vector2f( 1.f, .5f ) );
	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );

	m_boxbuttonsh->Pack( button, true );
}

void SampleApp::OnAddButtonVClick( sfg::Widget::Ptr /*widget*/ ) {
	sfg::Button::Ptr  button( sfg::Button::Create( L"<- New" ) );

	boost::shared_dynamic_cast<sfg::Label>( button->GetChild() )->SetAlignment( sf::Vector2f( 0.f, .5f ) );
	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );

	m_boxbuttonsv->Pack( button, false );
}

void SampleApp::OnNewButtonClick( sfg::Widget::Ptr widget ) {
	sfg::Button::Ptr  button( boost::shared_dynamic_cast<sfg::Button>( widget ) );

	button->SetLabel( "Ouch" );
}

void SampleApp::OnToggleTitlebarClick( sfg::Widget::Ptr /*widget*/ ) {
	m_wndmain->SetStyle( m_wndmain->GetStyle() ^ sfg::Window::Titlebar );
}

void SampleApp::OnHideWindowClicked( sfg::Widget::Ptr /*widget*/ ) {
	m_wndmain->Show( !m_wndmain->IsVisible() );
}

int main() {
	SampleApp  app;
	app.Run();
	return 0;
}
