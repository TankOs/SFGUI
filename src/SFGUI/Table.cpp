#include <SFGUI/Table.hpp>

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

	AllocateChildrenSizes();

	return size;
}

void Table::Attach( Widget::Ptr widget, const sf::Rect<sf::Uint32>& rect, int x_options, int y_options, const sf::Vector2f& padding ) {
	assert( rect.Width > 0 );
	assert( rect.Height > 0 );

	// Store widget in a table cell object.
	priv::TableCell cell( widget, rect, x_options, y_options, padding );
	m_cells.push_back( cell );

	// Check if we need to enlarge rows/columns.
	if( rect.Left + rect.Width >= m_columns.size() ) {
		std::size_t old_size( m_columns.size() );
		m_columns.resize( rect.Left + rect.Width );

		// Set default spacings.
		for( std::size_t col_index = old_size; col_index < m_columns.size(); ++col_index ) {
			m_columns[col_index].spacing = m_general_spacings.x;
		}
	}

	if( rect.Top + rect.Height >= m_rows.size() ) {
		std::size_t old_size( m_rows.size() );
		m_rows.resize( rect.Top + rect.Height );

		// Set default spacings.
		for( std::size_t row_index = old_size; row_index < m_rows.size(); ++row_index ) {
			m_rows[row_index].spacing = m_general_spacings.y;
		}
	}

	// Add widget to container.
	Add( widget );

	// Request new size.
	RequestSize();
}


void Table::HandleSizeAllocate( const sf::FloatRect& /*old_allocation*/ ) const {
	AllocateChildrenSizes();
}

void Table::UpdateRequisitions() const {
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

	// Iterate over children, but process only single-spanning ones. Calculates
	// required requisition and sets expand flag.
	TableCellList::const_iterator cell_iter( m_cells.begin() );
	TableCellList::const_iterator cell_iter_end( m_cells.end() );

	for( ; cell_iter != cell_iter_end; ++cell_iter ) {
		if( cell_iter->rect.Width == 1 ) {
			m_columns[cell_iter->rect.Left].requisition = std::max(
				m_columns[cell_iter->rect.Left].requisition,
				(
				cell_iter->child->GetRequisition().x +
				2.f * cell_iter->padding.x +
				(cell_iter->rect.Left + 1 < m_columns.size() ? m_columns[cell_iter->rect.Left].spacing : 0.f)
				)
			);

			if( (cell_iter->x_options & EXPAND) == EXPAND ) {
				m_columns[cell_iter->rect.Left].expand = true;
			}
		}

		if( cell_iter->rect.Height == 1 ) {
			m_rows[cell_iter->rect.Top].requisition = std::max(
				m_rows[cell_iter->rect.Top].requisition,
				(
				cell_iter->child->GetRequisition().y +
				2.f * cell_iter->padding.y +
				(cell_iter->rect.Top + 1 < m_rows.size() ? m_rows[cell_iter->rect.Top].spacing : 0.f)
				)
			);

			if( (cell_iter->y_options & EXPAND) == EXPAND ) {
				m_rows[cell_iter->rect.Top].expand = true;
			}
		}
	}

	// Now iterate again to process multi-spanning children. We need to do it
	// separately so we have expand flags available which are prioritized in
	// enlarging the requisition.
	for( cell_iter = m_cells.begin(); cell_iter != cell_iter_end; ++cell_iter ) {
		if( cell_iter->rect.Width > 1 ) {
			// Check if there's already enough space.
			float width( 0.f );
			uint32_t num_expand( 0 );

			for( uint32_t col_index = cell_iter->rect.Left; col_index < cell_iter->rect.Left + cell_iter->rect.Width; ++col_index ) {
				width += m_columns[col_index].requisition;

				// If not the last column, add spacing too.
				if( col_index + 1 < m_columns.size() ) {
					width += m_columns[col_index].spacing;
				}

				if( m_columns[col_index].expand ) {
					++num_expand;
				}
			}

			// Check for space, include padding here.
			const float required_width( cell_iter->child->GetRequisition().x + 2.f * cell_iter->padding.x );

			if( width < required_width ) {
				// Not enough space, so add missing width to columns. Prioritize
				// expanded columns because they might get larger anyways.
				float width_missing( required_width - width );

				// If there're no expandable columns, mark every column as being
				// expandable.  Results in homogeneous distribution of extra width.
				bool force_expand( false );

				if( num_expand == 0 ) {
					num_expand = cell_iter->rect.Width;
					force_expand = true;
				}

				for( uint32_t col_index = cell_iter->rect.Left; col_index < cell_iter->rect.Left + cell_iter->rect.Width && num_expand > 0; ++col_index ) {
					if( force_expand || m_columns[col_index].expand ) {
						float extra( width_missing / static_cast<float>( num_expand ) );

						m_columns[col_index].requisition += extra;
						width_missing -= extra;

						--num_expand;
					}
				}
			}
		}

		if( cell_iter->rect.Height > 1 ) {
			// Check if there's already enough space.
			float height( 0.f );
			uint32_t num_expand( 0 );

			for( uint32_t row_index = cell_iter->rect.Top; row_index < cell_iter->rect.Top + cell_iter->rect.Height; ++row_index ) {
				height += m_rows[row_index].requisition;

				// If not the last row, add spacing too.
				if( row_index + 1 < m_rows.size() ) {
					height += m_rows[row_index].spacing;
				}

				if( m_rows[row_index].expand ) {
					++num_expand;
				}
			}

			// Check for space, include padding here.
			const float required_height( cell_iter->child->GetRequisition().y + 2.f * cell_iter->padding.y );

			if( height < required_height ) {
				// Not enough space, so add missing height to rows. Prioritize expanded
				// rows because they might get larger anyways.
				float height_missing( required_height - height );

				// If there're no expandable rows, mark every row as being expandable.
				// Results in homogeneous distribution of extra height.
				bool force_expand( false );

				if( num_expand == 0 ) {
					num_expand = cell_iter->rect.Height;
					force_expand = true;
				}

				for( uint32_t row_index = cell_iter->rect.Top; row_index < cell_iter->rect.Top + cell_iter->rect.Height && num_expand > 0; ++row_index ) {
					if( force_expand || m_rows[row_index].expand ) {
						float extra( height_missing / static_cast<float>( num_expand ) );

						m_rows[row_index].requisition += extra;
						height_missing -= extra;

						--num_expand;
					}
				}
			}
		}
	}
}

void Table::AllocateChildrenSizes() const {
	// Process columns.
	float width( 0.f );
	std::size_t num_expand( 0 );

	// Calculate "normal" width of columns and count expanded columns.
	for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
		// Every allocaction will be at least as wide as the requisition.
		m_columns[column_index].allocation = m_columns[column_index].requisition;

		// Calc position.
		if( column_index == 0 ) {
			m_columns[column_index].position = 0.f;
		}
		else {
			m_columns[column_index].position = m_columns[column_index - 1].position + m_columns[column_index - 1].allocation;
		}

		// Count expanded columns.
		if( m_columns[column_index].expand ) {
			++num_expand;
		}

		width += m_columns[column_index].requisition;
	}

	// If there're expanded columns, we need to set the proper allocation for them.
	if( num_expand > 0 ) {
		float extra( (GetAllocation().Width - width) / static_cast<float>( num_expand ) );

		for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
			if( !m_columns[column_index].expand ) {
				continue;
			}

			m_columns[column_index].allocation += extra;

			// Position of next columns must be increased.
			for( std::size_t next_col_index = column_index + 1; next_col_index < m_columns.size(); ++next_col_index ) {
				m_columns[next_col_index].position += extra;
			}
		}
	}

	// Process rows.
	float height( 0.f );
	num_expand = 0;

	// Calculate "normal" height of rows and count expanded rows.
	for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
		// Every allocaction will be at least as wide as the requisition.
		m_rows[row_index].allocation = m_rows[row_index].requisition;

		// Calc position.
		if( row_index == 0 ) {
			m_rows[row_index].position = 0.f;
		}
		else {
			m_rows[row_index].position = m_rows[row_index - 1].position + m_rows[row_index - 1].allocation;
		}

		// Count expanded rows.
		if( m_rows[row_index].expand ) {
			++num_expand;
		}

		height += m_rows[row_index].requisition;
	}

	// If there're expanded rows, we need to set the proper allocation for them.
	if( num_expand > 0 ) {
		float extra( (GetAllocation().Height - height) / static_cast<float>( num_expand ) );

		for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
			if( !m_rows[row_index].expand ) {
				continue;
			}

			m_rows[row_index].allocation += extra;

			// Position of next rows must be increased.
			for( std::size_t next_row_index = row_index + 1; next_row_index < m_rows.size(); ++next_row_index ) {
				m_rows[next_row_index].position += extra;
			}
		}
	}

	// Allocate children sizes.
	TableCellList::const_iterator cell_iter( m_cells.begin() );
	TableCellList::const_iterator cell_iter_end( m_cells.end() );

	for( ; cell_iter != cell_iter_end; ++cell_iter ) {
		// Calc actual allocation both for single- and multi-spanned cells.
		sf::Vector2f allocation( 0.f, 0.f );

		if( (cell_iter->x_options & FILL) == FILL ) {
			for( uint32_t col_index = cell_iter->rect.Left; col_index < cell_iter->rect.Left + cell_iter->rect.Width; ++col_index ) {
				allocation.x += m_columns[col_index].allocation - cell_iter->padding.x * 2.f;

				if( col_index + 1 < m_columns.size() ) {
					allocation.x -= m_columns[col_index].spacing;
				}
			}

			allocation.x -= cell_iter->padding.x * 2.f;
		}
		else {
			allocation.x = cell_iter->child->GetRequisition().x - 2.f * cell_iter->padding.x;
		}

		if( (cell_iter->y_options & FILL) == FILL ) {
			for( uint32_t row_index = cell_iter->rect.Top; row_index < cell_iter->rect.Top + cell_iter->rect.Height; ++row_index ) {
				allocation.y += m_rows[row_index].allocation - cell_iter->padding.y * 2.f;

				if( row_index + 1 < m_rows.size() && row_index + 1 == cell_iter->rect.Top + cell_iter->rect.Height ) {
					allocation.y -= m_rows[row_index].spacing;
				}
			}

			allocation.y -= cell_iter->padding.y * 2.f;
		}
		else {
			allocation.y = cell_iter->child->GetRequisition().y - 2.f * cell_iter->padding.y;
		}

		// Shortcuts.
		const priv::TableOptions& column( m_columns[cell_iter->rect.Left] );
		const priv::TableOptions& row( m_rows[cell_iter->rect.Top] );

		// Check for FILL flag.
		cell_iter->child->AllocateSize(
			sf::FloatRect(
				column.position + cell_iter->padding.x,
				row.position + cell_iter->padding.y,
				allocation.x,
				allocation.y
			)
		);
	}
}

void Table::SetColumnSpacings( float spacing ) {
	for( std::size_t column_index = 0; column_index < m_columns.size(); ++column_index ) {
		m_columns[column_index].spacing = spacing;
	}

	m_general_spacings.x = spacing;

	RequestSize();
}

void Table::SetRowSpacings( float spacing ) {
	for( std::size_t row_index = 0; row_index < m_rows.size(); ++row_index ) {
		m_rows[row_index].spacing = spacing;
	}

	m_general_spacings.y = spacing;

	RequestSize();
}

void Table::SetColumnSpacing( std::size_t index, float spacing ) {
	if( index >= m_columns.size() ) {
		return;
	}

	m_columns[index].spacing = spacing;
	RequestSize();
}

void Table::SetRowSpacing( std::size_t index, float spacing ) {
	if( index >= m_rows.size() ) {
		return;
	}

	m_rows[index].spacing = spacing;
	RequestSize();
}

const std::string& Table::GetName() const {
	static const std::string name( "Table" );
	return name;
}

}
