#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <SFML/Graphics.hpp>

class SampleApp {
	public:
		void Run();

	private:
		void OnAddButtonClick( sfg::Widget::Ptr widget );

		sfg::Box::Ptr  m_boxbuttons;
};

void SampleApp::Run() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event  event;

	window.UseVerticalSync( true );

	// Create widgets.
	sfg::Window::Ptr  wndmain( sfg::Window::Create() );
	wndmain->SetTitle( L"Example application" );
	wndmain->SetBorderWidth( 10.f );

	sfg::Button::Ptr  btnaddbutton( sfg::Button::Create( L"Add button" ) );
	sfg::Button::Ptr  btnremovebutton( sfg::Button::Create( L"Remove button" ) );

	// Layout.
	sfg::Box::Ptr  boxtoolbar( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxtoolbar->SetPadding( 5.f );
	boxtoolbar->Pack( btnaddbutton, false );
	boxtoolbar->Pack( btnremovebutton, false );

	m_boxbuttons = sfg::Box::Create( sfg::Box::Horizontal );
	m_boxbuttons->SetPadding( 5.f );

	sfg::Box::Ptr  boxmain( sfg::Box::Create( sfg::Box::Vertical ) );
	boxmain->SetPadding( 5.f );
	boxmain->Pack( boxtoolbar, false );
	boxmain->Pack( m_boxbuttons );

	wndmain->Add( boxmain );

	// Signals.
	btnaddbutton->OnClick.Connect( &SampleApp::OnAddButtonClick, this );

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

void SampleApp::OnAddButtonClick( sfg::Widget::Ptr /*widget*/ ) {
	sfg::Button::Ptr  button( sfg::Button::Create( L"New" ) );
	m_boxbuttons->Pack( button, false );
}

int main() {
	SampleApp  app;
	app.Run();
	return 0;
}
