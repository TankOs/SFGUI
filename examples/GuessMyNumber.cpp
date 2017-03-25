#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

int main() {
	sf::RenderWindow render_window( sf::VideoMode( 1024, 768, 32 ), "Guess My Number (SFGUI)", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	render_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	auto current_number_entry = sfg::Entry::Create();
	auto tries_label = sfg::Label::Create();
	auto hint_label = sfg::Label::Create();
	auto guess_button = sfg::Button::Create( "Guess" );

	unsigned char current_number = 0;
	auto tries = 0u;

	// Custom properties.
	sfg::Context::Get().GetEngine().SetProperties(
		"Button#guess {"
		"	BackgroundColor: #006400FF;"
		"	BorderColor: #006400FF;"
		"}"
		"Button#guess:Prelight {"
		"	BackgroundColor: #008200FF;"
		"	BorderColor: #008200FF;"
		"}"
		"Button#guess > Label {"
		"	FontSize: 20;"
		"}"
	);

	auto update_ui = [&tries, &tries_label] {
		std::stringstream sstr;
		sstr << tries;
		tries_label->SetText( sstr.str() );
	};

	auto reset_game = [update_ui, &tries, &current_number, &hint_label, &guess_button] {
		tries = 0;
		current_number = static_cast<unsigned char>( std::rand() % 100 + 1 );

		hint_label->SetText( "-" );

		update_ui();
		guess_button->Show( true );
	};

	// Create widgets.
	auto window = sfg::Window::Create();
	window->SetTitle( "Guess My Number (SFGUI)" );

	auto new_game_button = sfg::Button::Create( "New game" );
	new_game_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [reset_game] { reset_game(); } );

	guess_button->SetId( "guess" );
	guess_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&current_number_entry, &hint_label, &tries, &update_ui, &current_number, &guess_button] {
		// Validate number.
		unsigned int buf_number( 0 );

		std::stringstream sstr( static_cast<std::string>( current_number_entry->GetText() ) );
		sstr >> buf_number;

		if( buf_number < 1 || buf_number > 100 ) {
			hint_label->SetText( "Enter a number from 1 to 100." );
			return;
		}

		++tries;
		update_ui();

		unsigned char number( static_cast<unsigned char>( buf_number ) );
		if( number < current_number ) {
			hint_label->SetText( "My number is higher." );
		}
		else if( number > current_number ) {
			hint_label->SetText( "My number is lower." );
		}
		else {
			hint_label->SetText( "Correct!" );
			guess_button->Show( false );
		}

		current_number_entry->SetText( "" );
	} );

	// Layout.
	auto table = sfg::Table::Create();
	table->Attach( sfg::Label::Create( "Your guess:" ), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( current_number_entry, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ) );
	table->Attach( sfg::Label::Create( "Tries:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( tries_label, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ) );
	table->Attach( sfg::Label::Create( "Hint:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( hint_label, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ) );

	table->SetColumnSpacings( 5.f );
	table->SetRowSpacings( 5.f );

	auto buttons_box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.f );
	buttons_box->Pack( new_game_button );
	buttons_box->Pack( guess_button );

	auto content_vbox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );
	content_vbox->Pack( sfg::Label::Create( "Guess my number, it's from 1 to 100!" ) );
	content_vbox->Pack( table );
	content_vbox->Pack( buttons_box );

	window->Add( content_vbox );

	reset_game();

	window->SetPosition(
		sf::Vector2f(
			static_cast<float>( render_window.getSize().x / 2 ) - window->GetAllocation().width / 2.f,
			static_cast<float>( render_window.getSize().y / 2 ) - window->GetAllocation().height / 2.f
		)
	);

	// Make sure all properties are applied.
	window->Refresh();

	sf::Event event;

	while( render_window.isOpen() ) {
		while( render_window.pollEvent( event ) ) {
			if(
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ||
				event.type == sf::Event::Closed
			) {
				return 0;
			}

			window->HandleEvent( event );
		}

		window->Update( 0.f );
		render_window.clear();
		sfgui.Display( render_window );
		render_window.display();
	}

	return 0;
}
