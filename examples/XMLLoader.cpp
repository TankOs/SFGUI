#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/UI/XMLLoader.hpp>

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