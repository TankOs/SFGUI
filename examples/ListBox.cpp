// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	sfg::SFGUI sfgui;
	sf::RenderWindow window(sf::VideoMode(640, 480), "ListBox Example");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    sfg::Desktop desktop;

	auto sfg_window = sfg::Window::Create();
	sfg_window->SetTitle( "ListBoxExample" );

	auto box_outer = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 15.0f );
	auto box_inner1 = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	auto box_inner2 = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );

	auto label1 = sfg::Label::Create("I'm single-select list.");
	auto label2 = sfg::Label::Create("I'm multi-select list.");

	auto input1 = sfg::Entry::Create("");
	auto input2 = sfg::Entry::Create("");

    auto list1 = sfg::ListBox::Create();
        list1->AppendItem( "Item1" );
        list1->AppendItem( "Item2" );
        list1->AppendItem( "Item3" );
    list1->GetSignal( sfg::ListBox::OnSelect ).Connect( std::bind( [list1, input1](){ if(list1->GetSelectedItemsCount()) input1->SetText(list1->GetSelectedItemText()); else input1->SetText(""); } ) );

    auto list2 = sfg::ListBox::Create();
        list2->AppendItem( "Item1" );
        list2->AppendItem( "Item2" );
        list2->AppendItem( "Item3" );
    list2->multiselect = true;
    list2->GetSignal( sfg::ListBox::OnSelect ).Connect( std::bind( [list2, input2](){
        std::string str = "";
        for(unsigned i=0; i<list2->GetSelectedItemsCount(); i++)
        {
            str += list2->GetSelectedItemText(i);
            str += ' ';
        }
        input2->SetText(str);
    } ) );

	box_outer->Pack(box_inner1);
	box_outer->Pack(box_inner2);

	box_inner1->Pack(label1);
	box_inner1->Pack(list1);
	box_inner1->Pack(input1);

	box_inner2->Pack(label2);
	box_inner2->Pack(list2);
	box_inner2->Pack(input2);

	sfg_window->Add( box_outer );
	desktop.Add( sfg_window );

	sfg_window->SetPosition(sf::Vector2f(window.getSize().x/2-sfg_window->GetRequisition().x/2, window.getSize().y/2-sfg_window->GetRequisition().y/2));

    sf::Event event;
    sf::Clock clock;

    window.resetGLStates();

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
                }
            }
            desktop.Update( clock.restart().asSeconds() );
            window.clear();
            sfgui.Display( window );
            window.display();
        }

    return 0;
}
