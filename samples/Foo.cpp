#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <SFML/Graphics.hpp>

class SampleApp {
	public:
		void Run();

	private:
		void OnAddButtonHClick( sfg::Widget::Ptr widget );
		void OnAddButtonVClick( sfg::Widget::Ptr widget );
		void OnNewButtonClick( sfg::Widget::Ptr widget );

		sfg::Box::Ptr  m_boxbuttonsh;
		sfg::Box::Ptr  m_boxbuttonsv;
};

void SampleApp::Run() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event  event;

	window.UseVerticalSync( true );

	// Create widgets.
	sfg::Window::Ptr  wndmain( sfg::Window::Create() );
	wndmain->SetTitle( L"Example application" );
	wndmain->SetBorderWidth( 10.f );

	sfg::Button::Ptr  btnaddbuttonh( sfg::Button::Create( L"Add button horizontally" ) );
	sfg::Button::Ptr  btnaddbuttonv( sfg::Button::Create( L"Add button vertically" ) );

	// Layout.
	sfg::Box::Ptr  boxtoolbar( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxtoolbar->SetPadding( 5.f );
	boxtoolbar->Pack( btnaddbuttonh, false );
	boxtoolbar->Pack( btnaddbuttonv, false );

	m_boxbuttonsh = sfg::Box::Create( sfg::Box::Horizontal );
	m_boxbuttonsh->SetPadding( 5.f );

	m_boxbuttonsv = sfg::Box::Create( sfg::Box::Vertical );
	m_boxbuttonsv->SetPadding( 5.f );

	sfg::Box::Ptr  boxmain( sfg::Box::Create( sfg::Box::Vertical ) );
	boxmain->SetPadding( 5.f );
	boxmain->Pack( boxtoolbar, false );
	boxmain->Pack( m_boxbuttonsh );
	boxmain->Pack( m_boxbuttonsv );

	wndmain->Add( boxmain );

	// Signals.
	btnaddbuttonh->OnClick.Connect( &SampleApp::OnAddButtonHClick, this );
	btnaddbuttonv->OnClick.Connect( &SampleApp::OnAddButtonVClick, this );

	while( window.IsOpened() ) {
		while( window.GetEvent( event ) ) {
			if( wndmain->HandleEvent( event ) == sfg::Widget::EatEvent ) {
				continue;
			}

			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
		}

		window.Clear( sf::Color( 80, 80, 80 ) );
		wndmain->Expose( window );
		window.Display();
	}
}

void SampleApp::OnAddButtonHClick( sfg::Widget::Ptr /*widget*/ ) {
	sfg::Button::Ptr  button( sfg::Button::Create( L"<- New ->" ) );

	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );
	m_boxbuttonsh->Pack( button, true );
}

void SampleApp::OnAddButtonVClick( sfg::Widget::Ptr /*widget*/ ) {
	sfg::Button::Ptr  button( sfg::Button::Create( L"New" ) );

	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );
	m_boxbuttonsv->Pack( button, false );
}

void SampleApp::OnNewButtonClick( sfg::Widget::Ptr widget ) {
	sfg::Button::Ptr  button( boost::shared_dynamic_cast<sfg::Button>( widget ) );

	button->SetCaption( "Ouch" );
}

int main() {
	SampleApp  app;
	app.Run();
	return 0;
}
