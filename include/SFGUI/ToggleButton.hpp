#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Button.hpp>

#include <memory>

namespace sfg {

/** Toggle button.
 * A toggle button can be raised or depressed.
 */
class SFGUI_API ToggleButton : public Button {
	public:
		typedef std::shared_ptr<ToggleButton> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const ToggleButton> PtrConst; //!< Shared pointer to const.

		/** Create toggle button.
		 * @param label Label.
		 */
		static Ptr Create( const sf::String& label = L"" );

		/** Dtor.
		 */
		virtual ~ToggleButton();

		virtual const std::string& GetName() const;

		/** Set active.
		 * @param active Active.
		 */
		void SetActive( bool active );

		/** Check if active.
		 * @return true if active.
		 */
		bool IsActive() const;

		Signal OnToggle; //<! Fired when toggled.

	protected:
		virtual RenderQueue* InvalidateImpl() const;

		virtual void HandleMouseClick( sf::Mouse::Button button, int x, int y );

		/** Ctor.
		 */
		ToggleButton();

	private:
		bool m_active;
};

}
