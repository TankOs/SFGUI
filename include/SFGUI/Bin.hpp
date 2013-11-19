#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Container.hpp>
#include <memory>

namespace sfg {

/** Base class for containers with only one child.
 */
class SFGUI_API Bin : public Container {
	public:
		typedef std::shared_ptr<Bin> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Bin> PtrConst; //!< Shared pointer.

		/** Get child widget.
		 * @return Child widget.
		 */
		Widget::Ptr GetChild() const;

	protected:
		virtual void HandleAdd( Widget::Ptr child ) override;
};

}
