#pragma once

#include <SFGUI/Bin.hpp>
#include <SFGUI/Misc.hpp>

#include <memory>

namespace sfg {

/** Alignment.
 */
class SFGUI_API Alignment : public Bin, public Misc {
	public:
		typedef std::shared_ptr<Alignment> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Alignment> PtrConst; //!< Shared pointer.

		/** Create alignment.
		 * @return Alignment.
		 */
		static Ptr Create();

		const std::string& GetName() const override;

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

		sf::Vector2f CalculateRequisition() override;

	private:
		void HandleSizeChange() override;
		void HandleAlignmentChange( const sf::Vector2f& old_alignment ) override;

		void UpdateChild();

		sf::Vector2f m_scale;
};

}
