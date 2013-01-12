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
#include <SFGUI/GLCanvas.hpp>
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

		/** Draw the GUI.
		 * Just a wrapper for the renderer.
		 * @param target sf::RenderTarget to draw to.
		 */
		void Display( sf::RenderTarget& target );

		/** Enable and select alpha testing threshold.
		 * Just a wrapper for the renderer.
		 * @param alpha_threshold Threshold at which fragments will get discarded if their alpha value is less than or equal to. Set to 0.f to disable.
		 */
		void TuneAlphaThreshold( float alpha_threshold );

		/** Enable or disable CPU driven face culling.
		 * Just a wrapper for the renderer.
		 * @param enable true to enable, false to disable.
		 */
		void TuneCull( bool enable );

		/** Enable or disable FBO GUI caching.
		 * Just a wrapper for the renderer.
		 * @param enable true to enable, false to disable.
		 */
		void TuneUseFBO( bool enable );

		/** Check if an SFGUI object was created.
		 * @return true if an SFGUI object was created.
		 */
		static bool IsAlive();

	private:
		static bool m_alive;
};

}
