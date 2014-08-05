// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Table Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our table.
	auto table = sfg::Table::Create();

	// Some demonstration buttons ;)
	auto foo = sfg::Button::Create( "Foo" );
	auto bar = sfg::Button::Create( "Bar" );
	auto baz = sfg::Button::Create( "Baz" );
	auto column_span = sfg::Button::Create( "Column Span" );
	auto row_span = sfg::Button::Create( "Row Span" );

	// Attach a widget to the table.
	// The first parameter is the widget to attach.
	//
	// The second parameter tells the table how to place the widget.
	// It is a 4-tuple (ignore the fact that it is a rect) containing
	// in order: column index, row index, column span, row span.
	// ( 0, 0, 1, 1 ) would mean 0th column, 0th row, occupy 1 column and 1 row.
	//
	// Similar to boxes you have optional packing options such as FILL and EXPAND.
	// With tables you have 2 directions to pack, the first is the horizontal
	// packing options and the second the vertical packing options.
	//
	// The last option is the padding you want to apply to the cell.
	table->Attach( foo, sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );

	// Do the same for the other 4 widgets.
	table->Attach( bar, sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
	table->Attach( baz, sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
	table->Attach( column_span, sf::Rect<sf::Uint32>( 0, 3, 2, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
	table->Attach( row_span, sf::Rect<sf::Uint32>( 1, 0, 1, 3 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );

	// Because we told our cells to only expand horizontally,
	// they will only resize if the size of the window changes horizontally.

	// Add our table to the window
	window->Add( table );

	// Start the game loop
	while ( app_window.isOpen() ) {
		// Process events
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			// Handle events
			window->HandleEvent( event );

			// Close window : exit
			if ( event.type == sf::Event::Closed ) {
				app_window.close();
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		window->Update( 0.f );

		// Clear screen
		app_window.clear();

		// Draw the GUI
		sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}
