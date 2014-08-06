#pragma once

#include <SFGUI/Widget.hpp>
#include <SFGUI/Misc.hpp>

#include <SFML/System/String.hpp>
#include <memory>

namespace sfg {

/** Text label.
 */
class SFGUI_API Label : public Widget, public Misc {
	public:
		typedef std::shared_ptr<Label> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Label> PtrConst; //!< Shared pointer.

		/** Create label.
		 * @param text Text.
		 * @return Label.
		 */
		static Ptr Create( const sf::String& text = L"" );

		const std::string& GetName() const override;

		/** Set text.
		 * @param text New text.
		 */
		void SetText( const sf::String& text );

		/** Get text.
		 * @return Text.
		 */
		const sf::String& GetText() const;

		/** Enable or disable line wrapping of the text.
		 * Be sure to use a custom requisition in x direction when enabling this.
		 * @param wrap True to wrap text.
		 */
		void SetLineWrap( bool wrap );

		/** Check if line wrapping is enabled for this label.
		 * @return True when line wrapping is enabled.
		 */
		bool GetLineWrap() const;

		/** Get text after wrapping has been performed and newlines inserted.
		 * @return Wrapped text.
		 */
		sf::String GetWrappedText() const;

	protected:
		/** Ctor.
		 * @param text Text.
		 */
		Label( const sf::String& text = "" );

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;
		void HandleRequisitionChange() override;
		void HandleSizeChange() override;

	private:
		void WrapText();

		sf::String m_text;
		sf::String m_wrapped_text;

		bool m_wrap;
};

}
