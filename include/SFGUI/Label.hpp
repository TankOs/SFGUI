#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Text label.
 */
class SFGUI_API Label : public Widget {
	public:
		typedef std::shared_ptr<Label>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Label>  PtrConst; //!< Shared pointer.

		/** Create label.
		 * @param text Text.
		 * @return Label.
		 */
		static Ptr Create( const sf::String& text = L"" );

		virtual const std::string& GetName() const;

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

		/** Set alignment
		 * @param alignment Alignment (0..1 for x and y).
		 */
		void SetAlignment( const sf::Vector2f& alignment );

		/** Get alignment.
		 * @return Alignment.
		 */
		const sf::Vector2f& GetAlignment() const;

	protected:
		RenderQueue* InvalidateImpl() const;
		sf::Vector2f GetRequisitionImpl() const;

	private:
		sf::String  m_text;
		sf::Vector2f m_alignment;
};

}
