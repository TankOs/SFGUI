#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/UI/XMLLoader.hpp>

extern "C" void button_click_event(sfg::Widget::Ptr widget, sf::RenderWindow* render){
    puts("Clicked");
}

extern "C" void on_tab_change_event(sfg::Notebook::Ptr notebook, sf::RenderWindow* render){
    puts("Tab changed!");
}

extern "C" void config_windo_load_event_for_widget(sfg::Notebook::Ptr notebook, sf::RenderWindow* render){
    puts("Window as created!");
}

int main() {
    sf::RenderWindow appWindow(sf::VideoMode({800, 600}), "Exemplo SFGUI");
    appWindow.setFramerateLimit(60);
    appWindow.resetGLStates();

    sfg::SFGUI sfgui;
    sfg::Desktop desktop;

    sfg::ui::XMLLoader::Ptr widget = sfg::ui::XMLLoader::Create();
    widget->loadFromFile("data/window.xml");
    desktop.Add(std::dynamic_pointer_cast<sfg::Window>(widget->getWidget()));

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