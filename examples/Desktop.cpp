#include <SFGUI/Desktop.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Context.hpp>

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

		sfg::Desktop m_desktop;
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
	m_desktop( sf::FloatRect( 0, 0, static_cast<float>( SCREEN_WIDTH ), static_cast<float>( SCREEN_HEIGHT ) ) ),
	m_count( 0 )
{
}

void DesktopExample::Run() {
	sf::RenderWindow render_window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "SFGUI Desktop Example" );
	sf::Event event;

	//// Main window ////
	// Widgets.
	sfg::Window::Ptr main_window( sfg::Window::Create() );
	main_window->SetTitle( "SFGUI Desktop Example" );

	sfg::Label::Ptr intro_label( sfg::Label::Create( "Click on \"Create window\" to create any number of new windows." ) );
	sfg::Button::Ptr create_window_button( sfg::Button::Create( "Create window" ) );
	create_window_button->SetId( "create_window" );

	// Layout.
	sfg::Box::Ptr main_box( sfg::Box::Create( sfg::Box::VERTICAL, 5.f ) );
	main_box->Pack( intro_label, false );
	main_box->Pack( create_window_button, false );

	main_window->Add( main_box );
	m_desktop.Add( main_window );

	// Signals.
	create_window_button->OnClick.Connect( &DesktopExample::OnCreateWindowClick, this );

	// Init.
	m_desktop.SetProperty( "Button#create_window > Label", "FontSize", 18.f );

	while( render_window.IsOpened() ) {
		while( render_window.PollEvent( event ) ) {
			if(
				(event.Type == sf::Event::Closed) ||
				(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Escape)
			) {
				render_window.Close();
			}
			else if( event.Type == sf::Event::Resized ) {
				m_desktop.UpdateViewRect(
					sf::FloatRect(
						0,
						0,
						static_cast<float>( render_window.GetWidth() ),
						static_cast<float>( render_window.GetHeight() )
					)
				);
			}
			else {
				m_desktop.HandleEvent( event );
			}
		}

		render_window.Clear();
		m_desktop.Expose( render_window );
		render_window.Display();
	}
}

void DesktopExample::OnCreateWindowClick() {
	++m_count;

	// Create a new window.
	sfg::Window::Ptr window( sfg::Window::Create() );

	std::stringstream sstr;
	sstr << "A new window (" << m_count << ")";
	window->SetTitle( sstr.str() );

	// Widgets.
	sfg::Button::Ptr destroy_button( sfg::Button::Create( "Destroy" ) );

	// Layout.
	sfg::Box::Ptr box( sfg::Box::Create( sfg::Box::VERTICAL, 5.f ) );
	box->Pack( sfg::Label::Create( "This is a newly created window, from runtime, interactively." ), false );
	box->Pack( sfg::Label::Create( "You can move me around, try it!" ), false );
	box->Pack( sfg::Label::Create( "Or click the button below to destroy me. :-(" ), false );
	box->Pack( destroy_button, false );

	window->Add( box );
	m_desktop.Add( window );

	// Signals.
	destroy_button->OnClick.Connect( &DesktopExample::OnDestroyWindowClick, this );
}

void DesktopExample::OnDestroyWindowClick() {
	// Obtain parent window.
	sfg::Widget::Ptr widget( sfg::Context::Get().GetActiveWidget() );

	while( widget->GetName() != "Window" ) {
		widget = widget->GetParent();
	}

	// Remove window from desktop.
	m_desktop.Remove( widget );
}
