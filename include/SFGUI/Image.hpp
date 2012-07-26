#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Misc.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <SFML/Graphics/Texture.hpp>

namespace sfg {

/** Image.
 */
class SFGUI_API Image : public Widget, public Misc {
	public:
		typedef SharedPtr<Image> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Image> PtrConst; //!< Shared pointer.

		/** Create image.
		 * @param image sf::Image.
		 * @return Image.
		 */
		static Ptr Create( const sf::Image& image = sf::Image() );

		virtual const std::string& GetName() const;

		/** Dtor.
		 */
		~Image();

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

		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		sf::Image m_image;
		mutable sf::Vector2f m_texture_offset;
};

}
