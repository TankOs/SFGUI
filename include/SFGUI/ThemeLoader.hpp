#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/KeyValueLoader.hpp>
#include <SFGUI/Engine.hpp>

#include <string>

namespace sfg {

/** YAML theme loader.
 */
class SFGUI_API ThemeLoader {
	public:
		/** Load theme from file.
		 * Specify KeyValueLoader class type as template argument.
		 * @param filename Filename.
		 * @param engine Engine that gets fed with theme values.
		 * @return true on success.
		 */
		template <class Loader>
		static bool LoadFromFile( const std::string& filename, Engine& engine );

	private:
		struct Observer : public KeyValueLoader::Observer {
			inline Observer( Engine& engine_ );
			inline void operator()( const std::string& key, const std::string& value );

			Engine& engine;
		};
};

}

#include "ThemeLoader.inl"
