// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

class MultiviewExample {
	public:
		void ButtonClick();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		// Create our button smart pointer.
		sfg::Button::Ptr m_button;
};

void MultiviewExample::ButtonClick() {
	// When the button is clicked it's label should change.
	m_button->SetLabel( "Bar" );
}

void MultiviewExample::Run() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Multiview Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Try dragging me to my cousins." );
	window->SetPosition( sf::Vector2f( 100.f, 100.f ) );

	// Create the button itself.
	m_button = sfg::Button::Create();

	// Set the label of the button.
	m_button->SetLabel( "Foo" );

	// Make the button nice and big
	m_button->SetRequisition( sf::Vector2f( 200.f, 20.f ) );

	// Add the button to the window
	window->Add( m_button );

	// So that our button has a meaningful purpose
	// (besides just looking awesome :P) we need to tell it to connect
	// to a callback of our choosing to notify us when it is clicked.
	m_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &MultiviewExample::ButtonClick, this ) );

	// If attempting to connect to a class method you need to provide
	// a pointer to it as the second parameter after the function address.

	// Our sf::RenderTexture to draw the GUI on.
	sf::RenderTexture render_texture;
	render_texture.create( 800, 600 );
	render_texture.resetGLStates();

	// Our 4 viewport Sprites.
	sf::Sprite sprite0;
	sf::Sprite sprite1;
	sf::Sprite sprite2;
	sf::Sprite sprite3;

	sprite0.setPosition( 0.f, 0.f );
	sprite1.setPosition( 400.f, 0.f );
	sprite2.setPosition( 0.f, 300.f );
	sprite3.setPosition( 400.f, 300.f );

	sprite0.setTexture( render_texture.getTexture() );
	sprite1.setTexture( render_texture.getTexture() );
	sprite2.setTexture( render_texture.getTexture() );
	sprite3.setTexture( render_texture.getTexture() );

	// Rectangle to clear the RenderTexture.
	sf::RectangleShape clear_rect( sf::Vector2f( 400.f, 300.f ) );
	clear_rect.setFillColor( sf::Color::Black );

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			// Because we are doing fancy stuff with multiple
			// viewports, we need to take care of translating
			// the SFML window coordinates into our local
			// viewport coordinates.
			if( event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased ) {
				event.mouseButton.x %= 400;
				event.mouseButton.y %= 300;
			}
			else if( event.type == sf::Event::MouseMoved ) {
				event.mouseMove.x %= 400;
				event.mouseMove.y %= 300;
			}

			window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
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
		m_sfgui.Display( render_texture );

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
}

int main() {
	MultiviewExample example;
	example.Run();

	return EXIT_SUCCESS;
}
