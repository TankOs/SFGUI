#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <memory>

namespace sfg {

/** Interface for all widgets with alignment.
 */
class SFGUI_API Misc {
	public:
		/** Dtor.
		 */
		virtual ~Misc() = default;

		/** Widget Justification
		*/
		enum class Justify :char {
			CENTRE = 0, /*!< No Justification. */
			LEFT, /*!< Left justificiation. */
			RIGHT /*!< RIght justification. */
		};

		/** Alignment Structure
		* Alignment sf::Vector2f
		* Justification char
		*/

		struct Alignment {
			sf::Vector2f position;
			Justify justification;
		};

		/** Set alignment
		 * @param alignment Alignment (0..1 for x and y).
		 */
		void SetAlignment( const sf::Vector2f position, const Justify justification = Misc::Justify::CENTRE );

		/** Get alignment.
		 * @return Alignment.
		 */
		const Alignment& GetAlignment() const;

		/** Equal operator
		*@return bool
		*/
		friend bool operator== ( Alignment &a1, Alignment &a2 );

		/** Not equal operator
		* @return bool
		*/
		friend bool operator!= ( Alignment &a1, Alignment &a2 );

	protected:
		virtual void HandleAlignmentChange( const Alignment& old_alignment );

	private:
		Alignment m_alignment;
};

}
