#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/CheckButton.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <set>

namespace sfg {

/** Radio Button.
 */
class SFGUI_API RadioButton : public CheckButton {
	public:
		typedef SharedPtr<RadioButton> Ptr; //!< Shared pointer.
		typedef SharedPtr<const RadioButton> PtrConst; //!< Shared pointer to const.

		/** Radio Button group.
		 */
		struct RadioButtonGroup {
			std::set< WeakPtr<RadioButton> > members;
		};

		/** Create radio button.
		 * @param label Label.
		 */
		static Ptr Create( const sf::String& label, const SharedPtr<RadioButtonGroup>& group = SharedPtr<RadioButtonGroup>() );

		/** Dtor.
		 */
		virtual ~RadioButton();

		/** Get the group of this RadioButton.
		 * @return Group of this RadioButton.
		 */
		const SharedPtr<RadioButtonGroup>& GetGroup() const;

		/** Set the group of this RadioButton.
		 * @param group New group.
		 */
		void SetGroup( const SharedPtr<RadioButtonGroup>& group );

		/** Set active.
		 * @param active Active.
		 */
		virtual void SetActive( bool active );

		virtual const std::string& GetName() const;

	protected:
		virtual void HandleMouseClick( sf::Mouse::Button button, int x, int y );

	private:
		RadioButton();

		SharedPtr<RadioButtonGroup> m_group;
};

}
