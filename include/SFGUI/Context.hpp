#pragma once

#include <SFGUI/Config.hpp>

#include <memory>

namespace sfg {

class Engine;
class Widget;

namespace eng {
class BREW;
}

/** GUI context.
 * A context saves several semi-global information for widgets, such as the
 * used rendering engine.
 */
class SFGUI_API Context {
	public:
		/** Ctor.
		 */
		Context();

		/** Get the active context.
		 * If no custom context has been activated, the default context is returned.
		 * @return Active context.
		 */
		static Context& Get();

		/** Activate a custom context.
		 * Make sure to call Deactivate() when finished using the context.
		 * @return false if custom context has already been activated.
		 */
		static bool Activate( Context& context );

		/** Deactivate previously activated context.
		 * @return false if no custom context has been activated.
		 */
		static bool Deactivate();

		/** Get default rendering engine (BREW).
		 * @return BREW.
		 */
		static Engine& GetDefaultEngine();

		/// @cond

		/** Destroy the default rendering engine instance.
		 */
		static void DestroyDefaultEngine();

		/// @endcond

		/** Get render engine.
		 * @return Engine.
		 */
		Engine& GetEngine() const;

		/** Set rendering engine.
		 * @param engine Engine.
		 */
		void SetEngine( Engine& engine );

		/** Set active widget.
		 * Called internally only.
		 * @param widget Widget.
		 */
		void SetActiveWidget( std::shared_ptr<Widget> widget );

		/** Get active widget.
		 * The active widget is the widget that's currently handling an event. You
		 * can use this method to obtain the processing widget when inside a signal
		 * handler. See examples/Desktop.cpp for an example.
		 * @return Active widget or Widget::Ptr().
		 */
		std::shared_ptr<Widget> GetActiveWidget() const;

	private:
		Engine* m_engine;
		std::weak_ptr<Widget> m_active_widget;
};

}
