#pragma once

#include <SFGUI/Bin.hpp>

#include <SFML/System/String.hpp>
#include <memory>
#include <vector>

namespace sfg {

class SFGUI_API ListBox : public Widget {
	public:
		typedef std::shared_ptr<ListBox> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const ListBox> PtrConst; //!< Shared pointer.

		/** Create button.
		 * @return ListBox.
		 */
		static Ptr Create(  );

		const std::string& GetName() const override;

        unsigned GetSelectedItemsCount();
		unsigned GetSelectedItemIndex(unsigned n=0);
		const std::string& GetSelectedItemText(unsigned n=0);

		void AppendItem(std::string str);
		void PrependItem(std::string str);
		void RemoveItem(unsigned index);
		void Clear();

        // Signals.
		static Signal::SignalID OnSelect; //!< Fired when an entry is selected.

		bool resize_automatically = true;
		bool multiselect = false;

	protected:
		/** Ctor.
		 */
		ListBox() = default;

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
        void HandleMouseEnter( int x, int y ) override;
		void HandleMouseLeave( int x, int y ) override;
		void HandleMouseMoveEvent( int x, int y ) override;
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;

		std::vector<std::string> m_entries;
		int selection_begin = -1, selection_end = -1;
		int hovered_element = -1; bool pressed_on_widget = false;
		std::vector<bool> selection_odds;

		bool widget_hover = false;
};

}
