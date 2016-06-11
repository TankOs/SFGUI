#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/Config.hpp>

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

		/** Add a required character set to the character sets that SFGUI will take into consideration for new fonts.
		 * This is required if using a script whose glyphs are not smaller than codepoint 0x370
		 * such as Greek, Cyrillic, Hebrew, Arabic etc.
		 * By default, only the Latin script and its extensions are taken into consideration.
		 * @param low_bound Lower boundary of the character set, i.e. the glyph with the smallest codepoint.
		 * @param high_bound Higher boundary of the character set, i.e. the glyph with the largest codepoint.
		 */
		void AddCharacterSet( sf::Uint32 low_bound, sf::Uint32 high_bound );
};

}
