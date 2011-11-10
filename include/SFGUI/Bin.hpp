#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Container.hpp>

namespace sfg {

/** Base class for containers with only one child.
 */
class SFGUI_API Bin : public Container {
	public:
		typedef std::shared_ptr<Bin>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Bin>  PtrConst; //!< Shared pointer.

		/** Dtor.
		 */
		virtual ~Bin();

		/** Get child widget.
		 * @return Child widget.
		 */
		Widget::Ptr GetChild() const;

	protected:
		/** Constructor.
		 */
		Bin();

		virtual void HandleAdd( Widget::Ptr child );
};

}
