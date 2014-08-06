#pragma once

#include <SFGUI/Widget.hpp>

#include <memory>

namespace sfg {

/** Progress Bar.
 */
class SFGUI_API ProgressBar : public Widget {
	public:
		typedef std::shared_ptr<ProgressBar> Ptr; ///< Shared pointer.
		typedef std::shared_ptr<const ProgressBar> PtrConst; ///< Shared pointer to const.

		/** Orientation.
		 */
		enum class Orientation : char {
			HORIZONTAL = 0, ///< Horizontal.
			VERTICAL ///< Vertical.
		};

		/** Create progress bar.
		 * @param orientation Orientation.
		 */
		static Ptr Create( Orientation orientation = Orientation::HORIZONTAL );

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

		const std::string& GetName() const override;

	private:
		ProgressBar( Orientation orientation = Orientation::HORIZONTAL );

		sf::Vector2f CalculateRequisition() override;
		std::unique_ptr<RenderQueue> InvalidateImpl() const override;

		float m_fraction;
		Orientation m_orientation;
};

}
