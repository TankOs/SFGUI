#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/ToggleButton.hpp>

#include <memory>

namespace sfg {

class SFGUI_API CheckButton : public ToggleButton {
	public:
		typedef std::shared_ptr<CheckButton> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const CheckButton> PtrConst; //!< Shared pointer to const.

		/** Create check button.
		 * @param label Label.
		 */
		static Ptr Create( const sf::String& label );

		/** Dtor.
		 */
		virtual ~CheckButton();

		virtual const std::string& GetName() const;

	private:
		CheckButton();

		RenderQueue* InvalidateImpl() const;
		sf::Vector2f GetRequisitionImpl() const;
		void HandleSizeAllocate( const sf::FloatRect& old_allocation ) const;
};

}
