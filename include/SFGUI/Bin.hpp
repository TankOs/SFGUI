#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Base class for containers with only one child.
 */
class SFGUI_API Bin : public Container {
	public:
		typedef SharedPtr<Bin> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Bin> PtrConst; //!< Shared pointer.

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

		virtual void HandleAdd( const Widget::Ptr& child );
};

}
