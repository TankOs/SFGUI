#pragma once

#include <SFGUI/CheckButton.hpp>

#include <memory>

namespace sfg {

class RadioButtonGroup;

/** Radio Button.
 */
class SFGUI_API RadioButton : public CheckButton {
	public:
		typedef std::shared_ptr<RadioButton> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const RadioButton> PtrConst; //!< Shared pointer to const.

		/** Create radio button.
		 * @param label Label.
		 * @param group Group to add this RadioButton to. Defaults to creating a new group.
		 */
		static Ptr Create( const sf::String& label, std::shared_ptr<RadioButtonGroup> group = std::shared_ptr<RadioButtonGroup>() );

		/** Get the group of this RadioButton.
		 * @return Group of this RadioButton.
		 */
		std::shared_ptr<RadioButtonGroup> GetGroup() const;

		/** Set the group of this RadioButton.
		 * @param group New group.
		 */
		void SetGroup( std::shared_ptr<RadioButtonGroup> group );

		/** Set active.
		 * @param active Active.
		 */
		void SetActive( bool active );

		const std::string& GetName() const override;

	protected:
		/** Ctor.
		 */
		RadioButton() = default;

		void HandleMouseClick( sf::Mouse::Button button, int x, int y ) override;

	private:
		std::shared_ptr<RadioButtonGroup> m_group;
};

}
