#pragma once

// This header CAN be used for convenience to include all widgets and some
// other classes SFGUI provides.

#include <SFGUI/Bin.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Fixed.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Range.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/CheckButton.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/Viewport.hpp>
#include <SFGUI/ProgressBar.hpp>
#include <SFGUI/Separator.hpp>
#include <SFGUI/Frame.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/Alignment.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Spinner.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/SpinButton.hpp>
#include <SFGUI/Renderer.hpp>

namespace sfg {

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

	private:
		static bool m_alive;
};

}
