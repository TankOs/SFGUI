#pragma once

#include <SFGUI/Config.hpp>

namespace sf {
class Window;
class RenderWindow;
class RenderTexture;
}

namespace sfg {

class Renderer;

/** A class to shield us from evil SFML and MSVC.
 */
class SFGUI_API SFGUI {
	public:
		/** Constructor.
		 */
		SFGUI();

		/** Dtor.
		 */
		~SFGUI();

		/** Draw the GUI to an sf::Window.
		 * Just a wrapper for the renderer.
		 * @param target sf::Window to draw to.
		 */
		void Display( sf::Window& target );

		/** Draw the GUI to an sf::RenderWindow.
		 * Just a wrapper for the renderer.
		 * @param target sf::RenderWindow to draw to.
		 */
		void Display( sf::RenderWindow& target );

		/** Draw the GUI to an sf::RenderTexture.
		 * Just a wrapper for the renderer.
		 * @param target sf::RenderTexture to draw to.
		 */
		void Display( sf::RenderTexture& target );

		/** Gets a reference to the currently active Renderer.
		 * Just a wrapper for Renderer::Get().
		 * @return Reference to the currently active Renderer.
		 */
		Renderer& GetRenderer();

		/** Check if an SFGUI object was created.
		 * @return true if an SFGUI object was created.
		 */
		static bool IsAlive();
};

}
