#pragma once

#include <SFGUI/Config.hpp>

#include <map>
#include <string>

namespace sfg {

/** Load key/value pairs from a file.
 * Abstract class.
 */
class SFGUI_API KeyValueLoader {
	public:
		/** Key/value observer called by load routine.
		 */
		struct Observer {
			/** Called by loader.
			 * @param key Key.
			 * @param value Value.
			 */
			virtual void operator()( const std::string& key, const std::string& value ) = 0;
		};

		/** Ctor.
		 */
		KeyValueLoader();

		/** Dtor.
		 */
		virtual ~KeyValueLoader();

		/** Load file.
		 * @param filename Filename.
		 * @return true on success.
		 */
		bool LoadFromFile( const std::string& filename );

		/** Visit key/value pairs and call an observer object.
		 * @param observer Observer.
		 */
		void Visit( Observer& observer ) const;

	protected:
		/** Add key/value pair.
		 * @param key Key.
		 * @param value Value.
		 */
		void AddValue( const std::string& key, const std::string& value );

		/** Load routine implementation.
		 * This method must feed the m_values member with loaded key/value pairs.
		 * @param filename Filename.
		 * @return true on success.
		 */
		virtual bool LoadImpl( const std::string& filename ) = 0;

	private:
		typedef std::map<const std::string, const std::string> KeyValueMap;

		KeyValueMap m_values;
};

}
