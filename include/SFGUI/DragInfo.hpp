#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/System/Vector2.hpp>

namespace sfg {

/** Simple storage that holds information about a drag process.
 */
class SFGUI_API DragInfo {
	public:
		/** Constructor.
		 * @param start Start mouse position.
		 */
		DragInfo( const sf::Vector2f& start );

		/** Update info.
		 * @param position New/current position.
		 */
		void Update( const sf::Vector2f& position );

		/** Get delta position.
		 * @return Difference between last position and current one.
		 */
		const sf::Vector2f& GetDelta() const;

		/** Get start position.
		 * @return Start position.
		 */
		const sf::Vector2f& GetStartPosition() const;

	private:
		sf::Vector2f  m_start;
		sf::Vector2f  m_delta;
		sf::Vector2f  m_last_position;
};

}
