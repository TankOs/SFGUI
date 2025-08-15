#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/UI/XMLLoader.hpp>

// to compile this examples need pass -rdynamic flag in compilation
// -rdynamic grant the access to extern function in executable file

extern "C" void button_click_event(sfg::Widget::Ptr widget){
    puts("Clicked");
}

extern "C" void on_tab_change_event(sfg::Notebook::Ptr notebook){
    puts("Tab changed!");
}

int main() {
    sf::RenderWindow appWindow(sf::VideoMode({800, 600}), "Loading GUI from XML");
    appWindow.setFramerateLimit(60);
    appWindow.resetGLStates();

    sfg::SFGUI sfgui;
    sfg::Desktop desktop;

    sfg::ui::XMLLoader::Ptr loader = sfg::ui::XMLLoader::Create();
    loader->loadFromFile("data/window.xml");
    desktop.Add(std::dynamic_pointer_cast<sfg::Window>(loader->getWidget()));

    desktop.LoadThemeFromFile("data/xmltheme.theme");

    while (appWindow.isOpen()) {
        while (auto event = appWindow.pollEvent()) {
            desktop.HandleEvent(*event);

            if (event->is<sf::Event::Closed>()) {
                appWindow.close();
            }
        }

        desktop.Update(0.0f);

        appWindow.clear(sf::Color(0xffffffff));
        sfgui.Display(appWindow);

        appWindow.display();
    }

    return 0;
}