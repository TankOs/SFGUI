#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Separator.
 */
class SFGUI_API Separator : public Widget {
	public:
		typedef SharedPtr<Separator> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Separator> PtrConst; //!< Shared pointer.

		/** Separator orientation.
		 */
		enum Orientation {
			HORIZONTAL = 0, //!< Horizontal separator.
			VERTICAL //!< Vertical separator.
		};

		/** Create separator widget.
		 * @param orientation Orientation.
		 * @return Separator widget.
		 */
		static Ptr Create( Orientation orientation = HORIZONTAL );

		virtual const std::string& GetName() const;

		/** Get the orientation of this separator widget
		 * @return orientation of this separator widget
		 */
		Orientation GetOrientation() const;

	protected:
		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		/** Ctor.
		 */
		Separator( Orientation orientation = HORIZONTAL );

		Orientation m_orientation;
};

}
