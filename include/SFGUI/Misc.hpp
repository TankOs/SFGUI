#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Base class for all widgets with alignment.
 */
class SFGUI_API Misc : public Widget {
	public:
		typedef SharedPtr<Misc> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Misc> PtrConst; //!< Shared pointer.

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

	private:
		sf::Vector2f m_alignment;
};

}
