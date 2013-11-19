#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/CheckButton.hpp>
#include <memory>

#include <set>

namespace sfg {

/** Radio Button.
 */
class SFGUI_API RadioButton : public CheckButton {
	public:
		typedef std::shared_ptr<RadioButton> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const RadioButton> PtrConst; //!< Shared pointer to const.

		/** Radio Button group.
		 */
		class RadioButtonGroup {
			public:
				typedef std::shared_ptr<RadioButtonGroup> Ptr; //!< Shared pointer.
				typedef std::shared_ptr<const RadioButtonGroup> PtrConst; //!< Shared pointer to const.
				typedef std::set<std::weak_ptr<RadioButton>, std::owner_less<std::weak_ptr<RadioButton>>> ContainerType; //!< Container type.

				/** Create radio button group.
				 */
				static Ptr Create();

				/** Get the members of this radio button group.
				 * @return members of this radio button group.
				 */
				ContainerType& GetMembers();

			private:
				ContainerType m_members;
		};

		/** Create radio button.
		 * @param label Label.
		 * @param group Group to add this RadioButton to. Defaults to creating a new group.
		 */
		static Ptr Create( const sf::String& label, RadioButtonGroup::Ptr group = RadioButtonGroup::Ptr() );

		/** Get the group of this RadioButton.
		 * @return Group of this RadioButton.
		 */
		RadioButtonGroup::Ptr GetGroup() const;

		/** Set the group of this RadioButton.
		 * @param group New group.
		 */
		void SetGroup( RadioButtonGroup::Ptr group );

		/** Set active.
		 * @param active Active.
		 */
		virtual void SetActive( bool active );

		virtual const std::string& GetName() const override;

	protected:
		virtual void HandleMouseClick( sf::Mouse::Button button, int x, int y ) override;

	private:
		RadioButtonGroup::Ptr m_group;
};

}
