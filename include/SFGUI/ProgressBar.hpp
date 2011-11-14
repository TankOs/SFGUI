#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>

#include <SFML/System/String.hpp>

namespace sfg {

class SFGUI_API ProgressBar : public Widget {
	public:
		typedef std::shared_ptr<ProgressBar> Ptr; ///< Shared pointer.
		typedef std::shared_ptr<const ProgressBar> PtrConst; ///< Shared pointer to const.

		/** Orientation.
		 */
		enum Orientation {
			HORIZONTAL = 0, ///< Horizontal.
			VERTICAL ///< Vertical.
		};

		/** Create progress bar.
		 * @param orientation Orientation.
		 */
		static Ptr Create( Orientation orientation = HORIZONTAL );

		/** Set orientation.
		 * @param orientation Orientation.
		 */
		void SetOrientation( Orientation orientation );

		/** Get orientation.
		 * @return Orientation.
		 */
		Orientation GetOrientation() const;

		/** Set fraction.
		 * @param fraction Fraction.
		 */
		void SetFraction( float fraction );

		/** Get fraction.
		 * @return Fraction.
		 */
		float GetFraction() const;

		const std::string& GetName() const;

	private:
		ProgressBar( Orientation orientation = HORIZONTAL );

		sf::Vector2f GetRequisitionImpl() const;
		RenderQueue* InvalidateImpl() const;

		Orientation m_orientation;
		float m_fraction;
};

}
