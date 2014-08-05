// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

// This is only done here for demonstration purposes.
// DO NOT DECLARE STATIC OR GLOBAL WIDGETS IN YOUR PROGRAMS.
// We simply cannot guarantee that it will work in every case.
// We have to reset this std::shared_ptr at the end of the program.
sfg::Window::Ptr window;

// This is the first possibility you have of connecting a signal handler.
// You just pass it the address of a normal function:
// button->OnLeftClick.Connect( &Foo );
void Foo() {
	window->SetTitle( "Foo" );
}

class Application {
	public:
		void Run();

		// The second possibility would be defining the function as a class
		// method in the same class as you call Connect() from, in this case
		// Application because Connect() is called from Application::Run.
		// You would pass it the address of the method and the this pointer:
		// button->OnLeftClick.Connect( &Application::Bar, this );
		void Bar();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;
};

// The definition of the second possibility
void Application::Bar() {
	window->SetTitle( "Bar" );
}

// This is the third possibility. You create instances of a class and
// define their behavior on a per instance basis. Depending on the pointer
// of object you pass to Connect() the same method gets called on different
// objects leading to different behavior.
// You would pass Connect() the method address and a pointer to the object:
// button->OnLeftClick.Connect( &BazClass::Baz, &baz1 );
class BazClass {
	public:
		BazClass( int m_type );
		void Baz();

		int m_type;
};

// In this example we tell the object how it is to behave by setting
// it's type in the constructor.
BazClass::BazClass( int type ) :
	m_type( type )
{
}

void BazClass::Baz() {
	// This is where the object picks how it is going to behave
	// based on it's type.
	switch( m_type ) {
		case 1:
			window->SetTitle( "Baz1" );
			break;
		case 2:
			window->SetTitle( "Baz2" );
			break;
		case 3:
			window->SetTitle( "Baz3" );
			break;
		default:
			break;
	}
}

void Application::Run() {
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Button Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	window->Add( box );

	// Possibility 1, normal function
	auto button1 = sfg::Button::Create();
	button1->SetLabel( "Clicky 1" );
	button1->GetSignal( sfg::Widget::OnLeftClick ).Connect( &Foo );
	box->Pack( button1, false );

	// Possibility 2, this class
	auto button2 = sfg::Button::Create();
	button2->SetLabel( "Clicky 2" );
	button2->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Application::Bar, this ) );
	box->Pack( button2, false );

	// Possibility 3, objects
	BazClass baz_array[3] = { BazClass( 1 ), BazClass( 2 ), BazClass( 3 ) };

	for( int i = 0; i < 3; i++ ) {
		std::stringstream sstr;
		sstr << "Clicky " << i + 3;
		auto button = sfg::Button::Create();
		button->SetLabel( sstr.str() );
		// This is just a more complicated way of passing a pointer to a
		// BazClass to Connect() when the BazClass object is part of an array.
		// Passing normal pointers such as &baz1 would also work.
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &BazClass::Baz, &( baz_array[i] ) ) );
		box->Pack( button, false );
	}

	// Notice that with possibility 3 you can do very advanced things. The tricky
	// part of implementing it this way is that the method address has to be
	// known at compile time, which means that only the instanciated object itself
	// is able to pick how it will behave when that method is called on it. This
	// way you can also connect signals to dynamically determined behavior.

	// For further reading on this topic refer to Design Patterns and as
	// specialized cases similar to the one in this example the
	// Factory Method Pattern and Abstract Factory Pattern.

	while ( app_window.isOpen() ) {
		sf::Event event;

		while ( app_window.pollEvent( event ) ) {
			window->HandleEvent( event );

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
		m_sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}

	// If you have any global or static widgets,
	// you need to reset their pointers before your
	// application exits.
	window.reset();
}

int main() {
	Application app;
	app.Run();

	return EXIT_SUCCESS;
}
