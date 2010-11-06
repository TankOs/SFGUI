#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Text label.
 */
class SFGUI_API Label : public Widget {
	public:
		typedef boost::shared_ptr<Label>  Ptr; //!< Shared pointer.

		/** Create label.
		 * @param text Text.
		 * @return Label.
		 */
		static Ptr Create( const sf::String& text = L"" );

		/** Ctor.
		 * @param text Text.
		 */
		Label( const sf::String& text = "" );

		/** Dtor.
		 */
		~Label();

		/** Set text.
		 * @param text New text.
		 */
		void SetText( const sf::String& text );

		/** Get text.
		 * @return Text.
		 */
		const sf::String& GetText() const;

		sf::Vector2f GetRequisition() const;

	protected:
		sf::Drawable* InvalidateImpl( const sf::RenderTarget& target );

	private:
		sf::String  m_text;
};

}
