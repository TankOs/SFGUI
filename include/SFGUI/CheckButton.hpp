#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/ToggleButton.hpp>

#include <memory>

namespace sfg {

/** Check Button.
 */
class SFGUI_API CheckButton : public ToggleButton {
	public:
		typedef std::shared_ptr<CheckButton> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const CheckButton> PtrConst; //!< Shared pointer to const.

		/** Create check button.
		 * @param label Label.
		 */
		static Ptr Create( const sf::String& label );

		virtual const std::string& GetName() const override;

	protected:
		std::unique_ptr<RenderQueue> InvalidateImpl() const;
		virtual sf::Vector2f CalculateRequisition() override;
		void HandleSizeChange();
};

}
