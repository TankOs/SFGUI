#pragma once

#include <SFGUI/Config.hpp>

namespace sfg {

class Engine;

/** GUI context.
 * A context saves several semi-global information for widgets, such as the
 * used rendering engine, theme, properties etc. 
 */
class SFGUI_API Context {
	public:
		/** Constructor.
		 * @param engine Rendering engine.
		 */
		Context( Engine& engine );

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

		/** Get render engine.
		 * @return Engine.
		 */
		Engine& GetEngine() const;

	private:
		static Context*  active_context;


		Engine&  m_renderengine;
};

}
