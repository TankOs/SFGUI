#pragma once

#include <SFGUI/Container.hpp>
#include <SFGUI/TableOptions.hpp>
#include <SFGUI/TableCell.hpp>

#include <list>
#include <vector>
#include <memory>
#include <cstdint>

namespace sfg {

/** Table layout widget.
 * The table layout widget arranges widges in rows and columns, where cells are
 * aligned. Child widgets may span across several rows and/or cells.
 */
class SFGUI_API Table : public Container {
	public:
		typedef std::shared_ptr<Table> Ptr; ///< Shared pointer shortcut.
		typedef std::shared_ptr<const Table> PtrConst; ///< Shared pointer shortcut.

		/** Attach options.
		 */
		enum AttachOption : std::uint8_t {
			EXPAND = 1 << 0, ///< Expand child to maximum possible size.
			FILL = 1 << 1 ///< Fill child to available size.
		};

		/** Create table widget.
		 * @return Table.
		 */
		static Ptr Create();

		const std::string& GetName() const override;

		/** Attach widget to table.
		 * @param widget Widget to attach.
		 * @param rect Rect to attach widget to (Left = column, Top = row, Width = column span, Height = row span).
		 * @param x_options Pack options for x axis.
		 * @param y_options Pack options for y axis.
		 * @param padding Padding.
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

	protected:
		/** Ctor.
		 */
		Table() = default;

	private:
		typedef std::list<priv::TableCell> TableCellList;
		typedef std::vector<priv::TableOptions> TableOptionsArray;

		sf::Vector2f CalculateRequisition() override;
		void UpdateRequisitions();
		void AllocateChildren();

		void HandleSizeChange() override;
		void HandleRequisitionChange() override;
		void HandleRemove( Widget::Ptr child ) override;

		TableCellList m_cells;
		TableOptionsArray m_columns;
		TableOptionsArray m_rows;

		sf::Vector2f m_general_spacings;
};

}
