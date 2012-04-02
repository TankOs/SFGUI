#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Misc.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Text label.
 */
class SFGUI_API Label : public Widget, public Misc {
	public:
		typedef SharedPtr<Label> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Label> PtrConst; //!< Shared pointer.

		/** Create label.
		 * @param text Text.
		 * @return Label.
		 */
		static Ptr Create( const sf::String& text = L"" );

		virtual const std::string& GetName() const;

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

	protected:
		/** Ctor.
		 * @param text Text.
		 */
		Label( const sf::String& text = "" );

		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		sf::String m_text;
};

}
