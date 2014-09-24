#pragma once

#include <SFGUI/Widget.hpp>

#include <memory>
#include <cstdint>

namespace sfg {

/** Separator.
 */
class SFGUI_API Separator : public Widget {
	public:
		typedef std::shared_ptr<Separator> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Separator> PtrConst; //!< Shared pointer.

		/** Separator orientation.
		 */
		enum class Orientation : std::uint8_t {
			HORIZONTAL = 0, //!< Horizontal separator.
			VERTICAL //!< Vertical separator.
		};

		/** Create separator widget.
		 * @param orientation Orientation.
		 * @return Separator widget.
		 */
		static Ptr Create( Orientation orientation = Orientation::HORIZONTAL );

		 const std::string& GetName() const override;

		/** Get the orientation of this separator widget
		 * @return orientation of this separator widget
		 */
		Orientation GetOrientation() const;

	protected:
		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		/** Ctor.
		 */
		Separator( Orientation orientation = Orientation::HORIZONTAL );

		Orientation m_orientation;
};

}
