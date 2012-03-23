#pragma once
#include <SFGUI/Container.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <map>

namespace sfg {

/** Fixed.
 */
class SFGUI_API Fixed : public Container {
	public:
		typedef SharedPtr<Fixed> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Fixed> PtrConst; //!< Shared pointer.

		/** Create fixed.
		 * @return Fixed.
		 */
		static Ptr Create();

		virtual const std::string& GetName() const;

		/** Add a widget to this fixed at given position.
		 * @param widget Widget to add.
		 * @param position Position to place the widget at.
		 */
		void Put( const Widget::Ptr& widget, const sf::Vector2f& position );

		/** Move a child widget to the given position.
		 * @param widget Widget to move.
		 * @param position Position to move the widget to.
		 */
		void Move( const Widget::Ptr& widget, const sf::Vector2f& position );

	protected:
		/** Ctor.
		 */
		Fixed();

		sf::Vector2f CalculateRequisition();

	private:
		typedef std::map<Widget::Ptr, sf::Vector2f> ChildrenPositionMap;

		void HandleAdd( const Widget::Ptr& child );
		void HandleRemove( const Widget::Ptr& child );

		ChildrenPositionMap m_children_position_map;
};

}
