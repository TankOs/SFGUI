#include <SFGUI/Table.hpp>

#include <iostream> // XXX 

namespace sfg {

Table::Table() {
	OnSizeAllocate.Connect( &Table::HandleSizeAllocate, this );
}

Table::Ptr Table::Create() {
	return Ptr( new Table );
}

sf::Vector2f Table::GetRequisitionImpl() const {
	sf::Vector2f size( 0.f, 0.f );

	// Update requisitions.
	UpdateRequisitions();

	// Count allocations of columns.
	TableOptionsArray::const_iterator column_iter( m_columns.begin() );
	TableOptionsArray::const_iterator column_iter_end( m_columns.end() );

	for( ; column_iter != column_iter_end; ++column_iter ) {
		size.x += column_iter->requisition;
	}

	// Count allocations of rows.
	TableOptionsArray::const_iterator row_iter( m_rows.begin() );
	TableOptionsArray::const_iterator row_iter_end( m_rows.end() );

	for( ; row_iter != row_iter_end; ++row_iter ) {
		size.y += row_iter->requisition;
	}

	return size;
}

void Table::Attach( Widget::Ptr widget, sf::Uint32 left, sf::Uint32 right, sf::Uint32 top, sf::Uint32 bottom, int x_options, int y_options ) {
	assert( right > left );
	assert( bottom > top );

	// Store widget in a table cell object.
	m_cells.push_back( priv::TableCell( widget, sf::Rect<sf::Uint32>( left, top, right - left, bottom - top ), x_options, y_options ) );

	// Check if we need to enlarge rows/columns.
	if( left >= m_columns.size() ) {
		m_columns.resize( left + 1 );
	}

	if( top >= m_rows.size() ) {
		m_rows.resize( top + 1 );
	}

	// Add widget to container.
	Add( widget );

	// Request new size.
	RequestSize();
}


void Table::HandleSizeAllocate( Widget::Ptr widget, const sf::FloatRect& /*old_allocation*/ ) {
	AllocateChildrenSizes();
}

void Table::UpdateRequisitions() const {
	// Reset requisitions and expand flags, at first.
	for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
		m_columns[column_index].requisition = 0.f;
		m_columns[column_index].expand = false;
	}

	for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
		m_rows[row_index].requisition = 0.f;
		m_rows[row_index].expand = false;
	}

	// Iterate over children to get the maximum requisition for each column and
	// row and set expand flags.
	TableCellList::const_iterator cell_iter( m_cells.begin() );
	TableCellList::const_iterator cell_iter_end( m_cells.end() );

	for( ; cell_iter != cell_iter_end; ++cell_iter ) {
		m_columns[cell_iter->rect.Left].requisition = std::max( m_columns[cell_iter->rect.Left].requisition, cell_iter->child->GetRequisition().x );
		m_columns[cell_iter->rect.Left].expand = (cell_iter->x_options & EXPAND);
		m_rows[cell_iter->rect.Top].requisition = std::max( m_rows[cell_iter->rect.Top].requisition, cell_iter->child->GetRequisition().y );
		m_rows[cell_iter->rect.Top].expand = (cell_iter->y_options & EXPAND);
	}
}

void Table::AllocateChildrenSizes() {
	// First pass: Reset column and row allocations.
	for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
		m_columns[column_index].allocation = m_columns[column_index].requisition;
	}

	for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
		m_rows[row_index].allocation = m_rows[row_index].requisition;
	}

	// Second pass: Find out which columns and rows are set as being expanded.
	TableCellList::iterator cell_iter( m_cells.begin() );
	TableCellList::iterator cell_iter_end( m_cells.end() );
	sf::Vector2<std::size_t> num_expand( 0, 0 );

	for( ; cell_iter != cell_iter_end; ++cell_iter ) {
		// Shortcuts, for convenience.
		priv::TableOptions& column( m_columns[cell_iter->rect.Left] );
		priv::TableOptions& row( m_rows[cell_iter->rect.Top] );

		// Check if column or row expands and count if it does so.
		if( column.expand ) {
			++num_expand.x;
		}

		if( row.expand ) {
			++num_expand.y;
		}
	}

	// Calculate size per expanded column/row.
	sf::Vector2f expand_size(
		GetAllocation().Width / std::max( static_cast<float>( num_expand.x ), 1.f ),
		GetAllocation().Height / std::max( static_cast<float>( num_expand.y ), 1.f )
	);

	std::cout << expand_size.x << " * " << expand_size.y << std::endl;

	// Third pass: Adjust column/row allocations for those that are expanded.
	// Also update positions.
	for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
		priv::TableOptions& column( m_columns[column_index] );

		if( column.expand ) {
			column.allocation = expand_size.x;
		}

		if( column_index > 0 ) {
			column.position = m_columns[column_index - 1].position + m_columns[column_index - 1].allocation;
		}
	}

	for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
		priv::TableOptions& row( m_rows[row_index] );

		if( row.expand ) {
			row.allocation = expand_size.y;
		}

		if( row_index > 0 ) {
			row.position = m_rows[row_index - 1].position + m_rows[row_index - 1].allocation;
		}
	}

	// Fourth pass: Apply allocations to children.
	for( cell_iter = m_cells.begin(); cell_iter != cell_iter_end; ++cell_iter ) {
		cell_iter->child->AllocateSize(
			sf::FloatRect(
				GetAllocation().Left + m_columns[cell_iter->rect.Left].position,
				GetAllocation().Top + m_rows[cell_iter->rect.Top].position,
				m_columns[cell_iter->rect.Left].allocation,
				m_rows[cell_iter->rect.Top].allocation
			)
		);
	}
}

}
