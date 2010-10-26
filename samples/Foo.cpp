#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <SFML/Graphics.hpp>


int main() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event  event;

	window.UseVerticalSync( true );

	// Create widgets.
	sfg::Window::Ptr  wndmain( sfg::Window::Create() );
	wndmain->SetName( "wndmain" );
	wndmain->SetTitle( L"Hello world..." );
	wndmain->SetBorderWidth( 10.f );

	sfg::Button::Ptr  btntest( sfg::Button::Create( L"Test" ) );
	sfg::Button::Ptr  btnquit( sfg::Button::Create( L"Quit" ) );

	// Layout.
	sfg::Box::Ptr  boxinside( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxinside->SetName( "boxinside" );
	boxinside->SetPadding( 3.f );
	boxinside->Pack( sfg::Button::Create( L"to work" ), false );
	boxinside->Pack( sfg::Button::Create( L"like it's" ), true, false );
	boxinside->Pack( sfg::Button::Create( L"supposed" ), true );
	boxinside->Pack( sfg::Button::Create( L"to do!" ), false );

	sfg::Box::Ptr  boxsecond( sfg::Box::Create( sfg::Box::Vertical ) );
	boxsecond->SetName( "boxsecond" );
	boxsecond->SetPadding( 3.f );
	boxsecond->Pack( sfg::Button::Create( L"Supi dupi" ), false );
	boxsecond->Pack( boxinside, true );
	boxsecond->Pack( sfg::Button::Create( L"This seems" ), false );

	sfg::Box::Ptr  boxtest( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxtest->SetName( "boxtest" );
	boxtest->SetPadding( 3.f );
	boxtest->Pack( btntest, false );
	boxtest->Pack( btnquit, false );
	boxtest->Pack( boxsecond, true );

	wndmain->Add( boxtest );

	while( window.IsOpened() ) {
		while( window.GetEvent( event ) ) {
			if( wndmain->HandleEvent( event ) ) {
				continue;
			}

			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
			else if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::M ) {
				wndmain->AllocateSize( sf::FloatRect( 0, 0, 800, 600 ) );
			}
			else if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Num1 ) {
				sfg::Button::Ptr  button( sfg::Button::Create( L"New" ) );
				boxinside->Pack( button, false );
			}
			else if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Num2 ) {
				sfg::Button::Ptr  button( sfg::Button::Create( L"New" ) );
				boxsecond->Pack( button, false );
			}
			else if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Num3 ) {
				sfg::Button::Ptr  button( sfg::Button::Create( L"New" ) );
				boxtest->Pack( button, false );
			}
		}

		window.Clear( sf::Color( 80, 80, 80 ) );
		wndmain->Expose( window );
		window.Display();
	}
}
