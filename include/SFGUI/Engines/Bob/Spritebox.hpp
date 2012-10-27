#pragma once

#include <SFGUI/Config.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFGUI/Primitive.hpp>

namespace sfg {
namespace eng {
namespace bob {

/** The Spritebox class renders scalable boxes.
 * A box consists of 9 graphics: 4 corners, 4 sides and the centre.
 */
class SFGUI_API Spritebox {
	public:
		/** Default constructor.
		 * Creates an empty Spritebox. Before drawing the Spritebox a texture
		 * must be set, else you won't see anything.
		 * @param dimension Dimension of the Spritebox.
		 */
		Spritebox( sf::Vector2i dimension = sf::Vector2i( 0, 0 ) );

		/** Create a Spritebox from a given texture.
		 * @param texture Texture.
		 * @param dimension Dimension of the Spritebox.
		 */
		Spritebox( SharedPtr< Primitive::Texture > texture, sf::Vector2i dimension = sf::Vector2i( 0, 0 ) );


		/** Set the texture of the Spritebox.
		 * @param texture SharedPtr to the new texture.
		 * @param adjustSubRect  If true, the subrect will be changed to the size of the new texture.
		 */
		void SetTexture( SharedPtr< Primitive::Texture > texture, bool adjustSubRect = false );

		/** Get the texture of the Spritebox.
		 * If no texture is set or the texture was deleted, a NULL pointer is returned.
		 * @return Pointer to the texture.
		 */
		SharedPtr< Primitive::Texture > GetTexture() const;

		/** Set the subrect inside the texture.
		 * @param subRect subrect.
		 */
		void SetSubRect( const sf::IntRect &subRect );

		/** Get the current subrect of the Spritebox.
		 * @return subrect.
		 */
		const sf::IntRect &GetSubRect() const;

		/** Set the dimension of the Spritebox.
		 * Note that it's not the actual size of the box(if you scale the drawable).
		 * Scaling the drawable won't affect the set dimension.
		 * @param vec New dimension.
		 */
		void SetDimension( const sf::Vector2i &vec );


		/** Get the dimension of the spritebox.
		 * @return Dimension of the box.
		 * @see SetDimension()
		 */
		const sf::Vector2i &GetDimension() const;

		/** Get the bounding rectangle of the Spritebox.
		 * The returned rectangle is in global coordinates.
		 * @return Bounding rectangle of the text
		 */
		sf::FloatRect GetRect() const;

		/** Construct a Primitive.
		 * @return shared_ptr to the Primitive.
		 */
		Primitive::Ptr ConstructPrimitive() const;

	private:
		sf::IntRect   m_subRect;
		sf::Vector2i  m_dimension;

		SharedPtr< Primitive::Texture > m_texture;
};

}
}
}
