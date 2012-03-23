#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Misc.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

/** Frame.
 */
class SFGUI_API Frame : public Bin, public Misc {
	public:
		typedef SharedPtr<Frame> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Frame> PtrConst; //!< Shared pointer.

		/** Create frame.
		 * @param label Label.
		 * @return Frame.
		 */
		static Ptr Create( const sf::String& label = L"" );

		virtual const std::string& GetName() const;

		/** Set label.
		 * @param label Label.
		 */
		void SetLabel( const sf::String& label );

		/** Get label.
		 * @return Label.
		 */
		const sf::String& GetLabel() const;

	protected:
		/** Ctor.
		 */
		Frame();

		virtual RenderQueue* InvalidateImpl() const;
		virtual sf::Vector2f CalculateRequisition();

	private:
		void HandleSizeChange();

		sf::String m_label;
		float m_alignment;
};

}
