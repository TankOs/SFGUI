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
		class RadioButtonGroup {
			public:
				typedef SharedPtr<RadioButtonGroup> Ptr; //!< Shared pointer.
				typedef SharedPtr<const RadioButtonGroup> PtrConst; //!< Shared pointer to const.

				/** Create radio button group.
				 */
				static Ptr Create();

				/** Get the members of this radio button group.
				 * @return members of this radio button group.
				 */
				std::set< WeakPtr<RadioButton> >& GetMembers();

			private:
				RadioButtonGroup();

				std::set< WeakPtr<RadioButton> > m_members;
		};

		/** Create radio button.
		 * @param label Label.
		 */
		static Ptr Create( const sf::String& label, const RadioButtonGroup::Ptr& group = RadioButtonGroup::Ptr() );

		/** Dtor.
		 */
		virtual ~RadioButton();

		/** Get the group of this RadioButton.
		 * @return Group of this RadioButton.
		 */
		const RadioButtonGroup::Ptr& GetGroup() const;

		/** Set the group of this RadioButton.
		 * @param group New group.
		 */
		void SetGroup( const RadioButtonGroup::Ptr& group );

		/** Set active.
		 * @param active Active.
		 */
		virtual void SetActive( bool active );

		virtual const std::string& GetName() const;

	protected:
		virtual void HandleMouseClick( sf::Mouse::Button button, int x, int y );

	private:
		RadioButton();

		RadioButtonGroup::Ptr m_group;
};

}
