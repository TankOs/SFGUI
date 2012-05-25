#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/ToggleButton.hpp>

#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Check Button.
 */
class SFGUI_API CheckButton : public ToggleButton {
	public:
		typedef SharedPtr<CheckButton> Ptr; //!< Shared pointer.
		typedef SharedPtr<const CheckButton> PtrConst; //!< Shared pointer to const.

		/** Create check button.
		 * @param label Label.
		 */
		static Ptr Create( const sf::String& label );

		/** Dtor.
		 */
		virtual ~CheckButton();

		virtual const std::string& GetName() const;

	protected:
		CheckButton();

		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();
		void HandleSizeChange();
};

}
