#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Misc.hpp>
#include <memory>
#include <SFML/System/String.hpp>

namespace sfg {

/** Frame.
 */
class SFGUI_API Frame : public Bin, public Misc {
	public:
		typedef std::shared_ptr<Frame> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Frame> PtrConst; //!< Shared pointer.

		/** Create frame.
		 * @param label Label.
		 * @return Frame.
		 */
		static Ptr Create( const sf::String& label = L"" );

		virtual const std::string& GetName() const override;

		/** Set label.
		 * @param label Label.
		 */
		void SetLabel( const sf::String& label );

		/** Get label.
		 * @return Label.
		 */
		const sf::String& GetLabel() const;

	protected:
		virtual std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		virtual sf::Vector2f CalculateRequisition() override;

	private:
		void HandleSizeChange();

		sf::String m_label;
};

}
