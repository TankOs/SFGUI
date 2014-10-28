#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

class GuessMyNumber {
	public:
		GuessMyNumber();

		void Run();

	private:
		static const std::string TITLE;

		void ResetGame();
		void UpdateUI();

		void OnGuessClick();
		void OnNewGameClick();

		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		unsigned char m_number;
		unsigned int m_tries;

		sfg::Entry::Ptr m_number_entry;
		sfg::Label::Ptr m_tries_label;
		sfg::Label::Ptr m_hint_label;
		sfg::Button::Ptr m_guess_button;
};

int main() {
	GuessMyNumber game;
	game.Run();

	return 0;
}

const std::string GuessMyNumber::TITLE = "Guess My Number (SFGUI)";

GuessMyNumber::GuessMyNumber() :
	m_number_entry( sfg::Entry::Create() ),
	m_tries_label( sfg::Label::Create() ),
	m_hint_label( sfg::Label::Create() ),
	m_guess_button( sfg::Button::Create( "Guess" ) )
{
}

void GuessMyNumber::ResetGame() {
	m_tries = 0;
	m_number = static_cast<unsigned char>( std::rand() % 100 + 1 );

	m_hint_label->SetText( "-" );

	UpdateUI();
	m_guess_button->Show( true );
}

void GuessMyNumber::UpdateUI() {
	std::stringstream sstr;
	sstr << m_tries;
	m_tries_label->SetText( sstr.str() );
}

void GuessMyNumber::OnNewGameClick() {
	ResetGame();
}

void GuessMyNumber::OnGuessClick() {
	// Validate number.
	unsigned int buf_number( 0 );

	std::stringstream sstr( static_cast<std::string>( m_number_entry->GetText() ) );
	sstr >> buf_number;

	if( buf_number < 1 || buf_number > 100 ) {
		m_hint_label->SetText( "Enter a number from 1 to 100." );
		return;
	}

	++m_tries;
	UpdateUI();

	unsigned char number( static_cast<unsigned char>( buf_number ) );
	if( number < m_number ) {
		m_hint_label->SetText( "My number is higher." );
	}
	else if( number > m_number ) {
		m_hint_label->SetText( "My number is lower." );
	}
	else {
		m_hint_label->SetText( "Correct!" );
		m_guess_button->Show( false );
	}

	m_number_entry->SetText( "" );
}

void GuessMyNumber::Run() {
	sf::RenderWindow render_window( sf::VideoMode( 1024, 768, 32 ), TITLE, sf::Style::Titlebar | sf::Style::Close );
	sf::Event event;

	// We have to do this because we don't use SFML to draw.
	render_window.resetGLStates();

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

	// Create widgets.
	auto window = sfg::Window::Create();
	window->SetTitle( TITLE );

	auto new_game_button = sfg::Button::Create( "New game" );
	new_game_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &GuessMyNumber::OnNewGameClick, this ) );

	m_guess_button->SetId( "guess" );
	m_guess_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &GuessMyNumber::OnGuessClick, this ) );

	// Layout.
	auto table = sfg::Table::Create();
	table->Attach( sfg::Label::Create( "Your guess:" ), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( m_number_entry, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ) );
	table->Attach( sfg::Label::Create( "Tries:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( m_tries_label, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ) );
	table->Attach( sfg::Label::Create( "Hint:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( m_hint_label, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ) );

	table->SetColumnSpacings( 5.f );
	table->SetRowSpacings( 5.f );

	auto buttons_box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.f );
	buttons_box->Pack( new_game_button );
	buttons_box->Pack( m_guess_button );

	auto content_vbox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );
	content_vbox->Pack( sfg::Label::Create( "Guess my number, it's from 1 to 100!" ) );
	content_vbox->Pack( table );
	content_vbox->Pack( buttons_box );

	window->Add( content_vbox );

	ResetGame();

	window->SetPosition(
		sf::Vector2f(
			static_cast<float>( render_window.getSize().x / 2 ) - window->GetAllocation().width / 2.f,
			static_cast<float>( render_window.getSize().y / 2 ) - window->GetAllocation().height / 2.f
		)
	);

	// Make sure all properties are applied.
	window->Refresh();

	while( render_window.isOpen() ) {
		while( render_window.pollEvent( event ) ) {
			if(
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ||
				event.type == sf::Event::Closed
			) {
				render_window.close();
			}

			window->HandleEvent( event );
		}

		window->Update( 0.f );
		render_window.clear();
		m_sfgui.Display( render_window );
		render_window.display();
	}
}
