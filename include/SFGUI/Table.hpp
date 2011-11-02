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
		typedef boost::shared_ptr<const Table> PtrConst; ///< Shared pointer shortcut.

		/** Ctor.
		 */
		Table();

		/** Create table widget.
		 * @return Table.
		 */
		static Ptr Create();

		virtual const std::string& GetName() const;

		/** Attach widget to table.
		 * @param widget Widget to attach.
		 * @param rect Rect to attach widget to (Left = column, Top = row, Width = column span, Height = row span).
		 * @param x_options Pack options for x axis.
		 * @param y_options Pack options for y axis.
		 */
		void Attach( Widget::Ptr widget, const sf::Rect<sf::Uint32>& rect, int x_options = EXPAND | FILL, int y_options = EXPAND | FILL, const sf::Vector2f& padding = sf::Vector2f( 0.f, 0.f ) );

		/** Set column spacing.
		 * @param index Column index.
		 * @param spacing Spacing.
		 */
		void SetColumnSpacing( std::size_t index, float spacing );

		/** Set column spacings (for all columns).
		 * @param spacing Spacing.
		 */
		void SetColumnSpacings( float spacing );

		/** Set row spacing.
		 * @param index Row index.
		 * @param spacing Spacing.
		 */
		void SetRowSpacing( std::size_t index, float spacing );

		/** Set row spacings (for all rows).
		 * @param spacing Spacing.
		 */
		void SetRowSpacings( float spacing );

	private:
		typedef std::list<priv::TableCell> TableCellList;
		typedef std::vector<priv::TableOptions> TableOptionsArray;

		sf::Vector2f GetRequisitionImpl() const;
		void UpdateRequisitions() const;
		void AllocateChildrenSizes();
		virtual bool HandleSizeAllocate( const sf::FloatRect& old_allocation );

		TableCellList m_cells;
		mutable TableOptionsArray m_columns;
		mutable TableOptionsArray m_rows;
};

}
