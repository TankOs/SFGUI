#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/XMLLoader.hpp>

int main() {
    sf::RenderWindow appWindow(sf::VideoMode({800, 600}), "Loading GUI from XML");
    appWindow.setFramerateLimit(60);
    appWindow.resetGLStates();

    sfg::SFGUI sfgui;
    sfg::Desktop desktop;

    sfg::XMLWidget widget;
    widget.loadFromFile("data/window.xml");
    desktop.Add(std::dynamic_pointer_cast<sfg::Window>(static_cast<sfg::Widget::Ptr>(widget)));

    desktop.LoadThemeFromFile("data/theme.css");

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