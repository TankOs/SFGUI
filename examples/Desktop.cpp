#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

class DesktopExample {
	public:
		DesktopExample();

		void Run();

	private:
		static const unsigned int SCREEN_WIDTH;
		static const unsigned int SCREEN_HEIGHT;

		void OnCreateWindowClick();
		void OnDestroyWindowClick();
		void OnFrontClick();

		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		sfg::Desktop m_desktop;
		sfg::Window::Ptr m_window;
		unsigned int m_count;
};

const unsigned int DesktopExample::SCREEN_WIDTH = 800;
const unsigned int DesktopExample::SCREEN_HEIGHT = 600;

int main() {
	DesktopExample app;
	app.Run();

	return 0;
}

DesktopExample::DesktopExample() :
	m_desktop(),
	m_window( sfg::Window::Create() ),
	m_count( 0 )
{
}

void DesktopExample::Run() {
	sf::RenderWindow render_window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "SFGUI Desktop Example" );
	sf::Event event;

	// We have to do this because we don't use SFML to draw.
	render_window.resetGLStates();

	// Init.
	m_desktop.SetProperty( "Button#create_window", "FontSize", 18.f );

	//// Main window ////
	// Widgets.
	m_window->SetTitle( "SFGUI Desktop Example" );

	auto intro_label = sfg::Label::Create( "Click on \"Create window\" to create any number of new windows." );
	auto create_window_button = sfg::Button::Create( "Create window" );
	create_window_button->SetId( "create_window" );

	// Layout.
	auto main_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );
	main_box->Pack( intro_label, false );
	main_box->Pack( create_window_button, false );

	m_window->Add( main_box );
	m_desktop.Add( m_window );

	// Signals.
	create_window_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &DesktopExample::OnCreateWindowClick, this ) );

	while( render_window.isOpen() ) {
		while( render_window.pollEvent( event ) ) {
			if(
				(event.type == sf::Event::Closed) ||
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			) {
				render_window.close();
			}
			else {
				m_desktop.HandleEvent( event );
			}
		}

		m_desktop.Update( 0.f );
		render_window.clear();
		m_sfgui.Display( render_window );
		render_window.display();
	}
}

void DesktopExample::OnCreateWindowClick() {
	++m_count;

	// Create a new window.
	auto window = sfg::Window::Create();

	std::stringstream sstr;
	sstr << "A new window (" << m_count << ")";
	window->SetTitle( sstr.str() );

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
	m_desktop.Add( window );

	// Signals.
	destroy_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &DesktopExample::OnDestroyWindowClick, this ) );
	front_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &DesktopExample::OnFrontClick, this ) );
}

void DesktopExample::OnDestroyWindowClick() {
	// Obtain parent window.
	auto widget = sfg::Context::Get().GetActiveWidget();

	while( widget->GetName() != "Window" ) {
		widget = widget->GetParent();
	}

	// Remove window from desktop.
	m_desktop.Remove( widget );
}

void DesktopExample::OnFrontClick() {
	m_desktop.BringToFront( m_window );
}
