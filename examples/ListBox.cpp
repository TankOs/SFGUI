// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	sfg::SFGUI sfgui;
	sf::RenderWindow window(sf::VideoMode(800, 600), "ListBox Example");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    sfg::Desktop desktop;

	auto window1 = sfg::Window::Create();
	window1->SetTitle( "ListBox with ItemTextPolicy::RESIZE_LISTBOX" );

	auto box1 = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	box1->SetSpacing( 5.f );
	box1->PackEnd( sfg::Label::Create( "The minimum width\nof this ListBox\ncorresponds to the largest\nitem's text width." ), false, true );

	auto listbox1 = sfg::ListBox::Create();
	listbox1->AppendItem( "This is the first item" );
	listbox1->AppendItem( "Second item" );
	listbox1->AppendItem( "Third item which is a bit large" );
	listbox1->AppendItem( "Fourth item" );
	listbox1->AppendItem( "Fifth item" );
	listbox1->AppendItem( "Sixth item" );
	listbox1->AppendItem( "Last one !" );
	box1->PackEnd( listbox1 );

	window1->Add( box1 );

	auto window2 = sfg::Window::Create();
	window2->SetTitle( "ListBox with ItemTextPolicy::SHRINK" );

	auto box2 = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	box2->SetSpacing( 5.f );
	auto label2 = sfg::Label::Create( "The items' texts\nare shrinked if the\nListBox is not big\nenough." );
	box2->PackEnd( label2, false, true );

	auto listbox2 = sfg::ListBox::Create();
	listbox2->AppendItem( "This is the first item (long text)" );
	listbox2->AppendItem( "Second item" );
	listbox2->AppendItem( "Third item which is very long !" );
	listbox2->AppendItem( "Fourth item" );
	listbox2->AppendItem( "Fifth item" );
	listbox2->AppendItem( "Sixth item, again it's too large !" );
	listbox2->AppendItem( "Last one !" );
	listbox2->SetItemTextPolicy( sfg::ListBox::ItemTextPolicy::SHRINK );
	box2->PackEnd( listbox2 );

	window2->Add( box2 );

	auto window3 = sfg::Window::Create();
	window3->SetTitle( "ListBox with ItemTextPolicy::SHRINK" );

	auto box3 = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
	box3->SetSpacing( 5.f );
	auto label3 = sfg::Label::Create( "You can select multiple\nitems in this ListBox." );
	box3->PackEnd( label3, false, true );

	auto listbox3 = sfg::ListBox::Create();
	listbox3->AppendItem( "First item" );
	listbox3->AppendItem( "Second item" );
	listbox3->AppendItem( "Third item" );
	listbox3->AppendItem( "Fourth item" );
	listbox3->AppendItem( "Fifth item" );
	listbox3->AppendItem( "Sixth item" );
	listbox3->AppendItem( "Last one !" );
	listbox3->SetSelectionMode( sfg::ListBox::SelectionMode::MULTI_SELECTION );
	listbox3->SetSelection( {1, 3, 4, 5} );
	box3->PackEnd( listbox3 );

	window3->Add( box3 );

	desktop.Add( window1 );
	desktop.Add( window2 );
	desktop.Add( window3 );

	sf::Vector2f windowSize( static_cast<float>( window.getSize().x ), static_cast<float>( window.getSize().y ) );

	window2->SetPosition(sf::Vector2f(windowSize.x/2.f - window2->GetRequisition().x/2.f, windowSize.y/2.f - window2->GetRequisition().y/2.f));
	window3->SetPosition(sf::Vector2f(windowSize.x - window3->GetRequisition().x - 100.f, windowSize.y - window3->GetRequisition().y - 100.f));

    sf::Event event;
    sf::Clock clock;

    window.resetGLStates();

	int i = 0;

    while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                desktop.HandleEvent( event );
                switch(event.type)
                {
                    case sf::Event::Closed:
                        window.close();
                    	break;
					case sf::Event::KeyPressed:
						if( event.key.code == sf::Keyboard::R ) {
							listbox3->RemoveItem(2);
						} else if( event.key.code == sf::Keyboard::I ) {
							listbox3->InsertItem(3, "Inserted item #" + std::to_string(i));
							++i;
						} else if( event.key.code == sf::Keyboard::A) {
							listbox3->AppendItem("Appended item #" + std::to_string(i));
							++i;
						} else if( event.key.code == sf::Keyboard::P) {
							listbox3->PrependItem("Prepended item #" + std::to_string(i));
							++i;
						}
						break;
					default:
						break;
                }
            }
            desktop.Update( clock.restart().asSeconds() );
            window.clear();
            sfgui.Display( window );
            window.display();
        }

    return 0;
}
