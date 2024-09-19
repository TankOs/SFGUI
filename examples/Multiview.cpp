// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( { 800, 600 } ), "SFGUI Multiview Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Try dragging me to my cousins." );
	window->SetPosition( sf::Vector2f( 100.f, 100.f ) );

	// Create the button itself.
	auto button = sfg::Button::Create();

	// Set the label of the button.
	button->SetLabel( "Foo" );

	// Make the button nice and big
	button->SetRequisition( sf::Vector2f( 200.f, 20.f ) );

	// Add the button to the window
	window->Add( button );

	// So that our button has a meaningful purpose
	// (besides just looking awesome :P) we need to tell it to connect
	// to a callback of our choosing to notify us when it is clicked.
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&button] {
		// When the button is clicked it's label should change.
		button->SetLabel( "Bar" );
	} );

	// If attempting to connect to a class method you need to provide
	// a pointer to it as the second parameter after the function address.

	// Our sf::RenderTexture to draw the GUI on.
	sf::RenderTexture render_texture( sf::Vector2u( 800, 600 ) );
	render_texture.resetGLStates();

	// Our 4 viewport Sprites.
	sf::Sprite sprite0( render_texture.getTexture() );
	sf::Sprite sprite1( render_texture.getTexture() );
	sf::Sprite sprite2( render_texture.getTexture() );
	sf::Sprite sprite3( render_texture.getTexture() );

	sprite0.setPosition( { 0.f, 0.f } );
	sprite1.setPosition( { 400.f, 0.f } );
	sprite2.setPosition( { 0.f, 300.f } );
	sprite3.setPosition( { 400.f, 300.f } );

	// Rectangle to clear the RenderTexture.
	sf::RectangleShape clear_rect( sf::Vector2f( 400.f, 300.f ) );
	clear_rect.setFillColor( sf::Color::Black );

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		while ( std::optional event = app_window.pollEvent() ) {
			// Handle events
			// Because we are doing fancy stuff with multiple
			// viewports, we need to take care of translating
			// the SFML window coordinates into our local
			// viewport coordinates.
			if( const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>() ) {
				auto altered_event = *mouseButtonPressed;
				altered_event.position.x %= 400;
				altered_event.position.y %= 300;
				event = altered_event;
			}
			else if( const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>() ) {
				auto altered_event = *mouseButtonReleased;
				altered_event.position.x %= 400;
				altered_event.position.y %= 300;
				event = altered_event;
			}
			else if( const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>() ) {
				auto altered_event = *mouseMoved;
				altered_event.position.x %= 400;
				altered_event.position.y %= 300;
				event = altered_event;
			}

			window->HandleEvent( *event );

			// Close window : exit
			if ( event->is<sf::Event::Closed>() ) {
				return EXIT_SUCCESS;
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		window->Update( 0.f );

		// Clear RenderTexture
		render_texture.clear();

		// This is a hack to clear the RenderTexture
		// since calling clear() obviously doesn't work.
		render_texture.draw( clear_rect );

		// Draw the GUI onto the RenderTexture.
		sfgui.Display( render_texture );

		// Display the RenderTexture.
		render_texture.display();

		// Clear screen
		app_window.clear();

		// Draw the Sprites using the RenderTexture.
		app_window.draw( sprite0 );
		app_window.draw( sprite1 );
		app_window.draw( sprite2 );
		app_window.draw( sprite3 );

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
