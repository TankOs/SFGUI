#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <SFML/System/String.hpp>

namespace sfg {

/** Progress Bar.
 */
class SFGUI_API ProgressBar : public Widget {
	public:
		typedef SharedPtr<ProgressBar> Ptr; ///< Shared pointer.
		typedef SharedPtr<const ProgressBar> PtrConst; ///< Shared pointer to const.

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
		 * @param fraction Fraction (from 0.0f to 1.0f).
		 */
		void SetFraction( float fraction );

		/** Get fraction.
		 * @return Fraction (from 0.0f to 1.0f).
		 */
		float GetFraction() const;

		virtual const std::string& GetName() const;

	private:
		ProgressBar( Orientation orientation = HORIZONTAL );

		sf::Vector2f CalculateRequisition();
		RenderQueue* InvalidateImpl() const;

		float m_fraction;
		unsigned char m_orientation;
};

}
