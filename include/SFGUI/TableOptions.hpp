#pragma once

namespace sfg {
namespace priv {

/** Options for a table row or column.
 */
class TableOptions {
	public:
		/** Ctor.
		 */
		TableOptions();

		bool expand; ///< Expand row/column?
		float position; ///< Position.
		float requisition; ///< Requisition (width or height).
		float allocation; ///< Allocation (width or height).
		float spacing; ///< Spacing.
};

}
}
