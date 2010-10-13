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
	sfg::Window::Ptr  wndsecond( sfg::Window::Create() );
	sfg::Box::Ptr     boxtest( sfg::Box::Create( sfg::Box::Horizontal ) );
	sfg::Button::Ptr  btntest( sfg::Button::Create( L"Test" ) );
	sfg::Button::Ptr  btnquit( sfg::Button::Create( L"Quit" ) );

	// Create an instance of the "BREW" (Basic Rendering Engine for Widgets)
	// rendering engine.
	sfg::eng::BREW    engine;

	// Properties will later be loaded from a theme.
	engine.SetProperty( "Window.background-color", "#888888" );
	engine.SetProperty( "Window.border-width", "2" );
	engine.SetProperty( "Window.border-color-light", "#bbbbbb" );
	engine.SetProperty( "Window.border-color-dark", "#444444" );
	engine.SetProperty( "Window.title-background-color", "#aaaaaa" );
	engine.SetProperty( "Window.title-size", "20" );
	engine.SetProperty( "Window.title-font-size", "14" );
	engine.SetProperty( "Window.shadow-distance", "4" );
	engine.SetProperty( "Window.shadow-alpha", "50" );

	// Setting the engine manually is perfectly fine. However, it'll be managed
	// later by sfg::GUI.
	wndmain->SetRenderEngine( engine );
	wndsecond->SetRenderEngine( engine );
	btntest->SetRenderEngine( engine );
	btnquit->SetRenderEngine( engine );

	wndmain->SetTitle( L"Hello world..." );
	wndsecond->SetTitle( L"...from BREW!" );

	wndmain->Add( boxtest );
	boxtest->Pack( btntest );
	boxtest->Pack( btnquit, false );

	for( unsigned int num = 0; num < 4; ++num ) {
		sfg::Button::Ptr  button( sfg::Button::Create( L"A button" ) );
		button->SetRenderEngine( engine );
		button->SetCaption( L"" );
		boxtest->Pack( button, false );
	}


	while( window.IsOpened() ) {
		while( window.GetEvent( event ) ) {
			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
			else if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::M ) {
				// Wondering about the method name? ;-) The next state of SFGUI will
				// highly make use of so called "sizers" that take care of properly
				// layouting your GUI. See the concepts of GTK to read more about it.
				wndmain->AllocateSize( sf::FloatRect( 200, 200, 400, 250 ) );
			}
		}

		window.Clear( sf::Color( 80, 80, 80 ) );

		// Again, manually querying the widgets to render is done by sfg::GUI
		// later.
		wndmain->Expose( window );
		wndsecond->Expose( window );

		window.Display();
	}
}
