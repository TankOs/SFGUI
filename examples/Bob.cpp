#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Engines/Bob.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

class DesktopExample {
	public:
		DesktopExample();

		void Run();

	private:
		static const unsigned int SCREEN_WIDTH;
		static const unsigned int SCREEN_HEIGHT;

		std::string m_engine_name;

		void OnButtonClick();

		sfg::SFGUI m_sfgui;

		sfg::Desktop m_desktop;
		sfg::Window::Ptr m_window;
		sfg::Button::Ptr m_button;
		sfg::Entry::Ptr m_entry;
		sfg::Scrollbar::Ptr m_scrollbar;
		sfg::ScrolledWindow::Ptr m_scrolled_window;
};

const unsigned int DesktopExample::SCREEN_WIDTH = 800;
const unsigned int DesktopExample::SCREEN_HEIGHT = 600;

int main() {
	DesktopExample app;
	app.Run();

	return 0;
}

DesktopExample::DesktopExample() :
	m_engine_name("Bob"),
	m_desktop(),
	m_window( sfg::Window::Create() )
{
	//The desktop manages the engine
	m_desktop.UseEngine<sfg::eng::Bob>();

	//Load a theme, or else you won't see anything
	m_desktop.LoadThemeFromFile( "data/bob/grey.theme" );
}

void DesktopExample::Run() {
	sf::RenderWindow render_window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "SFGUI Desktop Example" );
	sf::Event event;

	// We have to do this because we don't use SFML to draw.
	render_window.resetGLStates();

	//// Main window ////
	// Widgets.
	m_window->SetTitle( "SFGUI Bob Example" );

	sfg::Label::Ptr intro_label( sfg::Label::Create( "The brand new Bob-Engine!" ) );
	m_button = sfg::Button::Create( "Switch Engines" );

	m_entry = sfg::Entry::Create( L"Type Something" );
	m_entry->SetRequisition( sf::Vector2f( 100.f, .0f ) );

	m_scrollbar = sfg::Scrollbar::Create();
	m_scrollbar->SetRange( .0f, 100.f );

	sfg::Box::Ptr m_scrolled_window_box = sfg::Box::Create( sfg::Box::VERTICAL );
	for( int i = 0; i < 10; i++ ) {
		sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::HORIZONTAL );
		for( int j = 0; j < 20; j++ ) {
			box->Pack( sfg::Button::Create( L"One button among many" ), true );
		}

		m_scrolled_window_box->Pack( box, false );
	}

	m_scrolled_window = sfg::ScrolledWindow::Create();
	m_scrolled_window->SetRequisition( sf::Vector2f( 250.f, 100.f ) );
	m_scrolled_window->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	m_scrolled_window->SetPlacement( sfg::ScrolledWindow::TOP_LEFT );
	m_scrolled_window->AddWithViewport( m_scrolled_window_box );

	// Layout.
	sfg::Box::Ptr widget_box( sfg::Box::Create( sfg::Box::HORIZONTAL, 5.f) );
	widget_box->Pack( m_button, true );
	widget_box->Pack( m_entry, true );

	sfg::Box::Ptr main_box( sfg::Box::Create( sfg::Box::VERTICAL, 5.f ) );
	main_box->Pack( intro_label, false );
	main_box->Pack( m_scrollbar, false );
	main_box->Pack( widget_box, false );
	main_box->Pack( m_scrolled_window );


	m_window->Add( main_box );
	m_desktop.Add( m_window );

	m_window->SetPosition( sf::Vector2f( 100.f, 100.f) );

	// Signals.
	m_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &DesktopExample::OnButtonClick, this );

	sf::Clock clock;

	while( render_window.isOpen() ) {
		while( render_window.pollEvent( event ) ) {
			if(
				(event.type == sf::Event::Closed) ||
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			) {
				render_window.close();
			}
			else if( event.type == sf::Event::Resized ) {
				m_desktop.UpdateViewRect(
					sf::FloatRect(
						0,
						0,
						static_cast<float>( render_window.getSize().x ),
						static_cast<float>( render_window.getSize().y )
					)
				);
			}
			else {
				m_desktop.HandleEvent( event );
			}
		}

		float elapsed_time = clock.getElapsedTime().asSeconds();
		clock.restart();
		m_desktop.Update( elapsed_time );

		render_window.clear();

		m_sfgui.Display( render_window );

		render_window.display();
	}
}

void DesktopExample::OnButtonClick() {
	if( m_engine_name == "Bob" ){
		m_engine_name = "Brew";
		m_desktop.UseEngine< sfg::eng::BREW >();
	}
	else {
		m_engine_name = "Bob";
		m_desktop.UseEngine< sfg::eng::Bob >();

		//Don't forget to load a theme again,
		//since all properties are reset
		m_desktop.LoadThemeFromFile( "data/bob/grey.theme" );
	}
}
