#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Pushbutton.
 */
class SFGUI_API Button : public Bin {
	public:
		typedef SharedPtr<Button> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Button> PtrConst; //!< Shared pointer.

		/** Create button.
		 * @param label Label.
		 * @return Button.
		 */
		static Ptr Create( const sf::String& label = L"" );

		/** Dtor.
		 */
		virtual ~Button();

		virtual const std::string& GetName() const;

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
		void SetImage( const Image::Ptr& image );

		/** Get contained Image.
		 * @return Contained Image.
		 */
		const Image::Ptr GetImage() const;

		/** Clear Image of the button.
		 */
		void ClearImage();

	protected:
		/** Ctor.
		 */
		Button();

		virtual RenderQueue* InvalidateImpl() const;
		virtual sf::Vector2f CalculateRequisition();

		virtual void HandleAdd( const Widget::Ptr& child );

		void HandleSizeChange();

		virtual void HandleStateChange( State old_state );

	private:
		void HandleMouseEnter( int x, int y );
		void HandleMouseLeave( int x, int y );
		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );
		void AllocateChild();

		sf::String m_label;
};

}
