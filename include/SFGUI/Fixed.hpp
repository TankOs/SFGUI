#pragma once

#include <SFGUI/Container.hpp>

#include <memory>
#include <map>

namespace sfg {

/** Fixed.
 */
class SFGUI_API Fixed : public Container {
	public:
		typedef std::shared_ptr<Fixed> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Fixed> PtrConst; //!< Shared pointer.

		/** Create fixed.
		 * @return Fixed.
		 */
		static Ptr Create();

		const std::string& GetName() const override;

		/** Add a widget to this fixed at given position.
		 * @param widget Widget to add.
		 * @param position Position to place the widget at.
		 */
		void Put( Widget::Ptr widget, const sf::Vector2f& position );

		/** Move a child widget to the given position.
		 * @param widget Widget to move.
		 * @param position Position to move the widget to.
		 */
		void Move( Widget::Ptr widget, const sf::Vector2f& position );

	protected:
		/** Ctor.
		 */
		Fixed() = default;

		sf::Vector2f CalculateRequisition() override;

	private:
		typedef std::map<Widget::Ptr, sf::Vector2f> ChildrenPositionMap;

		bool HandleAdd( Widget::Ptr child ) override;
		void HandleRemove( Widget::Ptr child ) override;

		ChildrenPositionMap m_children_position_map;
};

}
