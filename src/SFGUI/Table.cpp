#include <SFGUI/Table.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

#include <set>
#include <cassert>

namespace sfg {

Table::Table() :
	m_general_spacings( 0.f, 0.f )
{
}

Table::Ptr Table::Create() {
	Ptr table( new Table );
	return Ptr( table );
}

sf::Vector2f Table::CalculateRequisition() {
	float gap( Context::Get().GetEngine().GetProperty<float>( "Gap", shared_from_this() ) );
	sf::Vector2f size( 2 * gap, 2 * gap );

	UpdateRequisitions();

	// Count requisitions of columns and rows.
	for( std::size_t col_idx = 0; col_idx < m_columns.size(); ++col_idx ) {
		size.x += m_columns[col_idx].requisition;
	}

	for( std::size_t row_idx = 0; row_idx < m_rows.size(); ++row_idx ) {
		size.y += m_rows[row_idx].requisition;
	}

	return size;
}

void Table::Attach( const Widget::Ptr& widget, const sf::Rect<sf::Uint32>& rect, int x_options, int y_options, const sf::Vector2f& padding ) {
	assert( rect.width > 0 );
	assert( rect.height > 0 );

	// Store widget in a table cell object.
	priv::TableCell cell( widget, rect, x_options, y_options, padding );
	m_cells.push_back( cell );

	// Check if we need to enlarge rows/columns.
	if( rect.left + rect.width >= m_columns.size() ) {
		std::size_t old_size( m_columns.size() );
		m_columns.resize( rect.left + rect.width );

		// Set default spacings.
		for( std::size_t col_index = old_size; col_index < m_columns.size(); ++col_index ) {
			m_columns[col_index].spacing = m_general_spacings.x;
		}
	}

	if( rect.top + rect.height >= m_rows.size() ) {
		std::size_t old_size( m_rows.size() );
		m_rows.resize( rect.top + rect.height );

		// Set default spacings.
		for( std::size_t row_index = old_size; row_index < m_rows.size(); ++row_index ) {
			m_rows[row_index].spacing = m_general_spacings.y;
		}
	}

	// Add widget to container.
	Add( widget );

	// Request new size.
	RequestResize();
}


void Table::HandleSizeChange() {
	AllocateChildren();
}

void Table::UpdateRequisitions() {
	// Reset requisitions and expand flags, at first.
	for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
		m_columns[column_index].requisition = 0.f;
		m_columns[column_index].allocation = 0.f;
		m_columns[column_index].expand = false;
	}

	for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
		m_rows[row_index].requisition = 0.f;
		m_rows[row_index].allocation = 0.f;
		m_rows[row_index].expand = false;
	}

	// Iterate over children and add requisitions to columns and rows.
	TableCellList::iterator cell_iter( m_cells.begin() );
	TableCellList::iterator cell_iter_end( m_cells.end() );

	for( ; cell_iter != cell_iter_end; ++cell_iter ) {
		float col_requisition = (
			cell_iter->child->GetRequisition().x / static_cast<float>( cell_iter->rect.width ) +
			2 * cell_iter->padding.x
		);
		sf::Uint32 col_bound = cell_iter->rect.left + cell_iter->rect.width;

		for( sf::Uint32 col_idx = cell_iter->rect.left; col_idx < col_bound; ++col_idx ) {
			m_columns[col_idx].requisition = std::max(
				m_columns[col_idx].requisition,
				col_requisition + (col_idx + 1 < m_columns.size() ? m_columns[col_idx].spacing : 0) // Add spacing if not last column.
			);

			// Set expand flag.
			if( (cell_iter->x_options & EXPAND) == EXPAND ) {
				m_columns[col_idx].expand = true;
			}
		}

		float row_requisition = (
			cell_iter->child->GetRequisition().y / static_cast<float>( cell_iter->rect.height ) +
			2 * cell_iter->padding.y
		);
		sf::Uint32 row_bound = cell_iter->rect.top + cell_iter->rect.height;

		for( sf::Uint32 row_idx = cell_iter->rect.top; row_idx < row_bound; ++row_idx ) {
			m_rows[row_idx].requisition = std::max(
				m_rows[row_idx].requisition,
				row_requisition + (row_idx + 1 < m_rows.size() ? m_rows[row_idx].spacing : 0) // Add spacing if not last row.
			);

			// Set expand flag.
			if( (cell_iter->y_options & EXPAND) == EXPAND ) {
				m_rows[row_idx].expand = true;
			}
		}
	}

	AllocateChildren();
}

void Table::AllocateChildren() {
	float gap( Context::Get().GetEngine().GetProperty<float>( "Gap", shared_from_this() ) );

	// Calculate column allocations.
	float total_width = GetAllocation().width - 2 * gap;
	std::size_t num_expand = 0;

	// First step is counting number of expandable columns and setting allocation
	// to requisition.
	for( std::size_t col_idx = 0; col_idx < m_columns.size(); ++col_idx ) {
		priv::TableOptions& col = m_columns[col_idx];

		if( col.expand ) {
			++num_expand;
		}

		col.allocation = col.requisition;
		total_width -= col.allocation;
	}

	// Next step is distribution of remaining width (i.e. extra width given by
	// parent) to expandable columns. Also calculate column positions.
	float extra_width = (num_expand > 0 ? total_width / static_cast<float>( num_expand ) : 0 );

	for( std::size_t col_idx = 0; col_idx < m_columns.size(); ++col_idx ) {
		priv::TableOptions& col = m_columns[col_idx];

		if( col.expand ) {
			col.allocation += extra_width;
		}

		if( col_idx == 0 ) {
			col.position = 0;
		}
		else {
			col.position = m_columns[col_idx - 1].position + m_columns[col_idx - 1].allocation;
		}
	}

	// Calculate row allocations.
	float total_height = 2 * gap + GetAllocation().height;
	num_expand = 0;

	// First step is counting number of expandable rows and setting allocation
	// to requisition.
	for( std::size_t row_idx = 0; row_idx < m_rows.size(); ++row_idx ) {
		priv::TableOptions& row = m_rows[row_idx];

		if( row.expand ) {
			++num_expand;
		}

		row.allocation = row.requisition;
		total_height -= row.allocation;
	}

	// Next step is distribution of remaining height (i.e. extra height given by
	// parent) to expandable rows. Also calculate rows positions.
	float extra_height = (num_expand > 0 ? total_height / static_cast<float>( num_expand ) : 0 );

	for( std::size_t row_idx = 0; row_idx < m_rows.size(); ++row_idx ) {
		priv::TableOptions& row = m_rows[row_idx];

		if( row.expand ) {
			row.allocation += extra_height;
		}

		if( row_idx == 0 ) {
			row.position = 0;
		}
		else {
			row.position = m_rows[row_idx - 1].position + m_rows[row_idx - 1].allocation;
		}
	}

	// Last step: Allocate children.
	TableCellList::iterator cell_iter( m_cells.begin() );
	TableCellList::iterator cell_iter_end( m_cells.end() );
	std::size_t bound = 0;

	for( ; cell_iter != cell_iter_end; ++cell_iter ) {
		sf::FloatRect allocation(
			m_columns[cell_iter->rect.left].position,
			m_rows[cell_iter->rect.top].position,
			0,
			0
		);

		bound = cell_iter->rect.left + cell_iter->rect.width;

		for( std::size_t col_idx = cell_iter->rect.left; col_idx < bound; ++col_idx ) {
			allocation.width += m_columns[col_idx].allocation;

			if( col_idx + 1 == bound && col_idx + 1 < m_columns.size() ) {
				allocation.width -= m_columns[col_idx].spacing;
			}
		}

		bound = cell_iter->rect.top + cell_iter->rect.height;

		for( std::size_t row_idx = cell_iter->rect.top; row_idx < bound; ++row_idx ) {
			allocation.height += m_rows[row_idx].allocation;

			if( row_idx + 1 == bound && row_idx + 1 < m_rows.size() ) {
				allocation.height -= m_rows[row_idx].spacing;
			}
		}

		// Limit size if FILL is not set.
		if( (cell_iter->x_options & FILL) != FILL ) {
			allocation.width = std::min( allocation.width, cell_iter->child->GetRequisition().x );
		}

		if( (cell_iter->y_options & FILL) != FILL ) {
			allocation.height = std::min( allocation.height, cell_iter->child->GetRequisition().y );
		}

		cell_iter->child->SetAllocation( allocation );
	}
}

void Table::SetColumnSpacings( float spacing ) {
	for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
		m_columns[column_index].spacing = spacing;
	}

	m_general_spacings.x = spacing;

	UpdateRequisitions();
	RequestResize();
}

void Table::SetRowSpacings( float spacing ) {
	for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
		m_rows[row_index].spacing = spacing;
	}

	m_general_spacings.y = spacing;

	UpdateRequisitions();
	RequestResize();
}

void Table::SetColumnSpacing( std::size_t index, float spacing ) {
	if( index >= m_columns.size() ) {
		return;
	}

	m_columns[index].spacing = spacing;

	UpdateRequisitions();
	RequestResize();
}

void Table::SetRowSpacing( std::size_t index, float spacing ) {
	if( index >= m_rows.size() ) {
		return;
	}

	m_rows[index].spacing = spacing;

	UpdateRequisitions();
	RequestResize();
}

const std::string& Table::GetName() const {
	static const std::string name( "Table" );
	return name;
}

void Table::HandleRequisitionChange() {
	AllocateChildren();
}

}
