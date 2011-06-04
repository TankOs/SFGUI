#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Range.hpp>

namespace sfg {

/** Scale widget.
 */
class SFGUI_API Scale : public Range {
	public:
		typedef boost::shared_ptr<Scale>  Ptr; //!< Shared pointer.

		/** Scale orientation.
		 */
		enum Orientation {
			Horizontal = 0, //!< Horizontal scale.
			Vertical //!< Vertical scale.
		};

		/** Create scale widget.
		 * @param orientation Orientation.
		 * @return Scale widget.
		 */
		static Ptr Create( Orientation orientation = Horizontal );

		/** Create scale widget.
		 * @param min Minimum value.
		 * @param max Maximum value.
		 * @param step Increment.
		 * @param orientation Orientation.
		 * @return Scale widget.
		 */
		static Ptr Create( float min, float max, float step, Orientation orientation = Horizontal );

		/** Get the orientation of this scale widget
		 * @return orientation of this scale widget
		 */
		const Orientation GetOrientation() const;

		/** Get slider rectangle ( position and dimensions )
		 * @return slider rect
		 */
		const sf::FloatRect GetSliderRect() const;

	protected:
		sf::Drawable* InvalidateImpl();
		sf::Vector2f GetRequisitionImpl() const;

	private:
		/** Ctor.
		 */
		Scale( Orientation orientation = Horizontal );

		bool HandleMouseButtonPress( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		bool HandleMouseButtonRelease( Widget::Ptr widget, int x, int y, sf::Mouse::Button button );
		void HandleMouseMove( Widget::Ptr widget, int x, int y );

		Orientation m_orientation;

		bool m_dragging;
};

}
