#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Misc.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** Alignment.
 */
class SFGUI_API Alignment : public Bin, public Misc {
	public:
		typedef SharedPtr<Alignment> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Alignment> PtrConst; //!< Shared pointer.

		/** Create alignment.
		 * @return Alignment.
		 */
		static Ptr Create();

		virtual const std::string& GetName() const;

		/** Set scale
		 * @param scale Scale (0..1 for x and y).
		 */
		void SetScale( const sf::Vector2f& scale );

		/** Get scale.
		 * @return Scale.
		 */
		const sf::Vector2f& GetScale() const;

	protected:
		/** Ctor.
		 */
		Alignment();

		sf::Vector2f CalculateRequisition();

	private:
		void HandleSizeChange();
		void HandleAlignmentChange( const sf::Vector2f& old_alignment );

		void UpdateChild();

		sf::Vector2f m_scale;
};

}
