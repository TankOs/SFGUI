#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Range.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Scale widget.
 */
class SFGUI_API Scale : public Range {
	public:
		typedef SharedPtr<Scale> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Scale> PtrConst; //!< Shared pointer.

		/** Dtor.
		 */
		virtual ~Scale();

		/** Create scale widget.
		 * @param orientation Orientation.
		 * @return Scale widget.
		 */
		static Ptr Create( Orientation orientation = HORIZONTAL );

		virtual const std::string& GetName() const;

		/** Create scale widget.
		 * @param min Minimum value.
		 * @param max Maximum value.
		 * @param step Increment.
		 * @param orientation Orientation.
		 * @return Scale widget.
		 */
		static Ptr Create( float min, float max, float step, Orientation orientation = HORIZONTAL );

		/** Get slider rectangle ( position and dimensions )
		 * @return slider rect
		 */
		const sf::FloatRect GetSliderRect() const;

	protected:
		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		/** Ctor.
		 */
		Scale( Orientation orientation = HORIZONTAL );

		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		virtual void HandleMouseMoveEvent( int x, int y );

		sf::Vector2f* m_drag_offset;
		bool m_dragging;
};

}
