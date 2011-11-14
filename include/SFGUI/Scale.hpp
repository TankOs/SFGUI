#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Range.hpp>

namespace sfg {

/** Scale widget.
 */
class SFGUI_API Scale : public Range {
	public:
		typedef std::shared_ptr<Scale>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Scale>  PtrConst; //!< Shared pointer.

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

		virtual const std::string& GetName() const;

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
		Orientation GetOrientation() const;

		/** Get slider rectangle ( position and dimensions )
		 * @return slider rect
		 */
		const sf::FloatRect GetSliderRect() const;

	protected:
		RenderQueue* InvalidateImpl() const;
		sf::Vector2f GetRequisitionImpl() const;

	private:
		/** Ctor.
		 */
		Scale( Orientation orientation = Horizontal );

		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		virtual void HandleMouseMoveEvent( int x, int y );

		Orientation m_orientation;

		bool m_dragging;
};

}
