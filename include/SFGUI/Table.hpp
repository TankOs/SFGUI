#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/TableCell.hpp>
#include <SFGUI/TableOptions.hpp>
#include <SFGUI/Container.hpp>

#include <list>
#include <vector>

namespace sfg {

/** Table layout widget.
 * The table layout widget arranges widges in rows and columns, where cells are
 * aligned. Child widgets may span across several rows and/or cells.
 */
class SFGUI_API Table : public Container {
	public:
		typedef boost::shared_ptr<Table> Ptr; ///< Shared pointer shortcut.

		/** Ctor.
		 */
		Table();

		/** Create table widget.
		 * @return Table.
		 */
		static Ptr Create();

		/** Attach widget to table.
		 * @param widget Widget to attach.
		 * @param left Left column index to attach widget to.
		 * @param right Right column index + 1 to attach widget to.
		 * @param top Top row index to attach widget to.
		 * @param bottom Bottom row index + 1 to attach widget to.
		 * @param x_options Pack options for x axis.
		 * @param y_options Pack options for y axis.
		 */
		void Attach( Widget::Ptr widget, sf::Uint32 left, sf::Uint32 right, sf::Uint32 top, sf::Uint32 bottom, int x_options = EXPAND | FILL, int y_options = EXPAND | FILL );

	private:
		typedef std::list<priv::TableCell> TableCellList;
		typedef std::vector<priv::TableOptions> TableOptionsArray;

		sf::Vector2f GetRequisitionImpl() const;
		void UpdateRequisitions() const;
		void AllocateChildrenSizes();
		void HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& rect );

		TableCellList m_cells;
		mutable TableOptionsArray m_columns;
		mutable TableOptionsArray m_rows;
};

}
