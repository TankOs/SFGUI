#pragma once

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

		const std::string& GetName() const override;

	protected:
		/** Ctor.
		 */
		CheckButton() = default;

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;
		void HandleSizeChange() override;
};

}
