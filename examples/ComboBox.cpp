// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

class ComboBoxExample {
	public:
		void OnComboSelect();
		void OnAddItemClick();
		void OnRemoveItemClick();
		void OnClearClick();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		// Create our ComboBox smart pointer.
		sfg::ComboBox::Ptr m_combo_box;

		sfg::Label::Ptr m_sel_label;
};

void ComboBoxExample::OnComboSelect() {
	std::stringstream sstr;

	sstr << "Item " << m_combo_box->GetSelectedItem() << " selected with text \"" << static_cast<std::string>( m_combo_box->GetSelectedText() ) << "\"";
	m_sel_label->SetText( sstr.str() );
}

void ComboBoxExample::OnAddItemClick() {
	static int counter( 0 );

	std::stringstream sstr;
	sstr << "Item " << counter;
	m_combo_box->AppendItem( sstr.str() );

	++counter;
}

void ComboBoxExample::OnRemoveItemClick() {
	m_combo_box->RemoveItem( 0 );
}

void ComboBoxExample::OnClearClick() {
	m_combo_box->Clear();
}

void ComboBoxExample::Run() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Combo Box Example", sf::Style::Titlebar | sf::Style::Close );

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create our main SFGUI window
	auto window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create the combo box itself.
	m_combo_box = sfg::ComboBox::Create();

	// Set the entries of the combo box.
	m_combo_box->AppendItem( "Bar" );
	m_combo_box->PrependItem( "Foo" );

	m_sel_label = sfg::Label::Create( L"Please select an item!" );

	auto add_button = sfg::Button::Create( L"Add item" );
	auto remove_button = sfg::Button::Create( L"Remove first item" );
	auto clear_button = sfg::Button::Create( L"Clear items" );

	auto hbox = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5 );
	hbox->Pack( m_combo_box );
	hbox->Pack( add_button, false );
	hbox->Pack( remove_button, false );
	hbox->Pack( clear_button, false );

	auto vbox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5 );
	vbox->Pack( hbox, false );
	vbox->Pack( m_sel_label, true );

	// Add the combo box to the window
	window->Add( vbox );

	// So that our combo box has a meaningful purpose (besides just looking
	// awesome :P) we need to tell it to connect to a callback of our choosing to
	// notify us when it is clicked.
	m_combo_box->GetSignal( sfg::ComboBox::OnSelect ).Connect( std::bind( &ComboBoxExample::OnComboSelect, this ) );

	add_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &ComboBoxExample::OnAddItemClick, this ) );
	remove_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &ComboBoxExample::OnRemoveItemClick, this ) );
	clear_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &ComboBoxExample::OnClearClick, this ) );

	// If attempting to connect to a class method you need to provide
	// a pointer to it as the second parameter after the function address.

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
		m_sfgui.Display( app_window );

		// Update the window
		app_window.display();
	}
}

int main() {
	ComboBoxExample example;
	example.Run();

	return EXIT_SUCCESS;
}
