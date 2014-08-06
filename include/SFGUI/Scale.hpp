#pragma once

#include <SFGUI/Range.hpp>

#include <memory>

namespace sfg {

/** Scale widget.
 */
class SFGUI_API Scale : public Range {
	public:
		typedef std::shared_ptr<Scale> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Scale> PtrConst; //!< Shared pointer.

		/** Create scale widget.
		 * @param orientation Orientation.
		 * @return Scale widget.
		 */
		static Ptr Create( Orientation orientation = Orientation::HORIZONTAL );

		const std::string& GetName() const override;

		/** Create scale widget.
		 * @param min Minimum value.
		 * @param max Maximum value.
		 * @param step Increment.
		 * @param orientation Orientation.
		 * @return Scale widget.
		 */
		static Ptr Create( float min, float max, float step, Orientation orientation = Orientation::HORIZONTAL );

		/** Get slider rectangle ( position and dimensions )
		 * @return slider rect
		 */
		const sf::FloatRect GetSliderRect() const;

	protected:
		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		/** Ctor.
		 */
		Scale( Orientation orientation = Orientation::HORIZONTAL );

		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void HandleMouseMoveEvent( int x, int y ) override;

		std::unique_ptr<sf::Vector2f> m_drag_offset;
		bool m_dragging;
};

}
