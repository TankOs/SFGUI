#pragma once

#include <SFGUI/Config.hpp>

#include <memory>
#include <set>

namespace sfg {

class RadioButton;

/** Radio Button group.
 */
class SFGUI_API RadioButtonGroup {
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

	protected:
		/** Ctor.
		 */
		RadioButtonGroup() = default;

	private:
		ContainerType m_members;
};

}
