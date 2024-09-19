#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow render_window( sf::VideoMode( { 800, 600 } ), "SFGUI Desktop Example" );

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// We have to do this because we don't use SFML to draw.
	render_window.resetGLStates();

	// Init.
	sfg::Desktop desktop;
	desktop.SetProperty( "Button#create_window", "FontSize", 18.f );

	//// Main window ////
	// Widgets.
	auto main_window = sfg::Window::Create();
	main_window->SetTitle( "SFGUI Desktop Example" );

	auto intro_label = sfg::Label::Create( "Click on \"Create window\" to create any number of new windows." );
	auto create_window_button = sfg::Button::Create( "Create window" );
	create_window_button->SetId( "create_window" );

	// Layout.
	auto main_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );
	main_box->Pack( intro_label, false );
	main_box->Pack( create_window_button, false );

	main_window->Add( main_box );
	desktop.Add( main_window );

	auto count = 0u;

	// Signals.
	create_window_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&count, &desktop, &main_window] {
		++count;

		// Create a new window.
		auto window = sfg::Window::Create();

		window->SetTitle( "A new window (" + std::to_string( count ) + ")" );

		// Widgets.
		auto destroy_button = sfg::Button::Create( "Destroy" );
		auto front_button = sfg::Button::Create( "Main window to front" );

		// Layout.
		auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );
		box->Pack( sfg::Label::Create( "This is a newly created window, from runtime, interactively." ), false );
		box->Pack( sfg::Label::Create( "You can move me around, try it!" ), false );
		box->Pack( sfg::Label::Create( "Or click the button below to destroy me. :-(" ), false );
		box->Pack( destroy_button, false );
		box->Pack( front_button, false );

		window->Add( box );
		desktop.Add( window );

		// Signals.
		destroy_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&desktop] {
			// Obtain parent window.
			auto widget = sfg::Context::Get().GetActiveWidget();

			while( widget->GetName() != "Window" ) {
				widget = widget->GetParent();
			}

			// Remove window from desktop.
			desktop.Remove( widget );
		} );

		front_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&desktop, &main_window] { desktop.BringToFront( main_window ); } );
	} );

	while( render_window.isOpen() ) {
		while( const std::optional event = render_window.pollEvent() ) {
			if(
				(event->is<sf::Event::Closed>()) ||
				(event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::Escape)
			) {
				return 0;
			}
			else {
				desktop.HandleEvent( *event );
			}
		}

		desktop.Update( 0.f );
		render_window.clear();
		sfgui.Display( render_window );
		render_window.display();
	}

	return 0;
}
