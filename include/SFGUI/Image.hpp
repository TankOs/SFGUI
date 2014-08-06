#pragma once

#include <SFGUI/Widget.hpp>
#include <SFGUI/Misc.hpp>

#include <SFML/Graphics/Image.hpp>
#include <memory>

namespace sfg {

/** Image.
 */
class SFGUI_API Image : public Widget, public Misc {
	public:
		typedef std::shared_ptr<Image> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Image> PtrConst; //!< Shared pointer.

		/** Create image.
		 * @param image sf::Image.
		 * @return Image.
		 */
		static Ptr Create( const sf::Image& image = sf::Image() );

		const std::string& GetName() const override;

		/** Set associated sf::Image.
		 * @param image New sf::Image.
		 */
		void SetImage( const sf::Image& image );

		/** Get associated sf::Image.
		 * @return sf::Image.
		 */
		const sf::Image& GetImage() const;

	protected:
		/** Ctor.
		 * @param image sf::Image.
		 */
		Image( const sf::Image& image = sf::Image() );

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		sf::Image m_image;
		mutable sf::Vector2f m_texture_offset;
};

}
