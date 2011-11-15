#pragma once
#include <SFGUI/Bin.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Pushbutton.
 */
class SFGUI_API Button : public Bin {
	public:
		typedef std::shared_ptr<Button>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Button>  PtrConst; //!< Shared pointer.

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

		/* Get label.
		 * @return Label.
		 */
		const sf::String& GetLabel() const;

		Signal OnClick; //!< Fired when button clicked.

	protected:
		/** Ctor.
		 */
		Button();

		virtual RenderQueue* InvalidateImpl() const;
		virtual sf::Vector2f GetRequisitionImpl() const;

		void HandleMouseClick( sf::Mouse::Button button, int x, int y );

	private:
		void HandleMouseEnter( int x, int y );
		void HandleMouseLeave( int x, int y );
		virtual void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y );

		sf::String m_label;
};

}
