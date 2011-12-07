#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System.hpp>

namespace sfg {

/** Interface for all widgets with alignment.
 */
class SFGUI_API Misc {
	public:
		/** Dtor.
		 */
		~Misc();

		/** Set alignment
		 * @param alignment Alignment (0..1 for x and y).
		 */
		void SetAlignment( const sf::Vector2f& alignment );

		/** Get alignment.
		 * @return Alignment.
		 */
		const sf::Vector2f& GetAlignment() const;

	protected:
		/** Ctor.
		 */
		Misc();

		virtual void HandleAlignmentChange( const sf::Vector2f& old_alignment );

	private:
		sf::Vector2f m_alignment;
};

}
