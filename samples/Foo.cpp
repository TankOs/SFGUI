#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Engines/BREW.hpp>
#include <SFML/Graphics.hpp>
#include <list>

typedef std::list<sfg::Button::Ptr>  ButtonsList;

int main() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event         event;

	window.UseVerticalSync( true );

	// Two top-level windows.
	sfg::Window::Ptr  wndmain( sfg::Window::Create() );
	sfg::Box::Ptr     boxtest( sfg::Box::Create( sfg::Box::Vertical ) );
	sfg::Button::Ptr  btntest( sfg::Button::Create( L"Test" ) );
	sfg::Button::Ptr  btnquit( sfg::Button::Create( L"Quit" ) );

	wndmain->SetTitle( L"Hello world..." );

	btntest->SetName( "btntest" );
	btnquit->SetName( "btnquit" );
	boxtest->SetName( "boxtest" );

	//wndmain->AllocateSize( sf::FloatRect( 100.f, 100.f, 300.f, 300.f ) );
	wndmain->Add( boxtest );
	boxtest->Pack( btntest, false );
	boxtest->Pack( btnquit, true );

	/*for( unsigned int num = 0; num < 4; ++num ) {
		sfg::Button::Ptr  button( sfg::Button::Create( L"A button" ) );
		button->SetCaption( L"A button" );
		boxtest->Pack( button, false );
	}*/

	while( window.IsOpened() ) {
		while( window.GetEvent( event ) ) {
			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
			else if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::M ) {
				// Wondering about the method name? ;-) The next state of SFGUI will
				// highly make use of so called "sizers" that take care of properly
				// layouting your GUI. See the concepts of GTK to read more about it.
				wndmain->AllocateSize( sf::FloatRect( 200, 200, 800, 300 ) );
			}
			else if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::A ) {
				sfg::Button::Ptr  button( sfg::Button::Create( L"New button" ) );
				button->SetCaption( L"New button" );
				boxtest->Pack( button, false );
			}
		}

		window.Clear( sf::Color( 80, 80, 80 ) );

		// Again, manually querying the widgets to render is done by sfg::GUI
		// later.
		wndmain->Expose( window );

		window.Display();
	}
}
