#pragma once

#include <SFGUI/Bin.hpp>

#include <SFML/System/String.hpp>
#include <memory>

namespace sfg {

class Image;

/** Pushbutton.
 */
class SFGUI_API Button : public Bin {
	public:
		typedef std::shared_ptr<Button> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Button> PtrConst; //!< Shared pointer.

		/** Create button.
		 * @param label Label.
		 * @return Button.
		 */
		static Ptr Create( const sf::String& label = L"" );

		const std::string& GetName() const override;

		/** Set label.
		 * @param label Label.
		 */
		void SetLabel( const sf::String& label );

		/** Get label.
		 * @return Label.
		 */
		const sf::String& GetLabel() const;

		/** Set Image of the button.
		 * @param image Image of the button.
		 */
		void SetImage( std::shared_ptr<Image> image );

		/** Get contained Image.
		 * @return Contained Image.
		 */
		std::shared_ptr<const Image> GetImage() const;

		/** Clear Image of the button.
		 */
		void ClearImage();

	protected:
		/** Ctor.
		 */
		Button() = default;

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

		bool HandleAdd( Widget::Ptr child ) override;

		void HandleSizeChange() override;

		void HandleStateChange( State old_state ) override;

	private:
		void HandleMouseEnter( int x, int y ) override;
		void HandleMouseLeave( int x, int y ) override;
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override;
		void AllocateChild();

		sf::String m_label;
};

}
