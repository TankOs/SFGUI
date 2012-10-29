#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Primitive.hpp>
#include <SFGUI/NonCopyable.hpp>

#include <map>

namespace sf {
class Image;
}

namespace sfg {
namespace eng {
namespace bob {

/** A class to manage our textures.
 * This class is necessary to manage our textures,
 * since we don't want to reload it all the time.
 */
class SFGUI_API TextureManager : public NonCopyable {
	public:
		/** Ctor.
		 * Creates an empty TextureManager.
		 */
		TextureManager();

		/** Dtor.
		 */
		~TextureManager();

		/** Get a texture from a given image.
		 * @param  image Image to load.
		 * @return SharedPtr to the loaded texture.
		 */
		SharedPtr< Primitive::Texture > GetTexture( const sf::Image *image );

		/** Unload texture from a given loaded image.
		 * @param  image Image to Unload.
		 */
		void UnloadTexture( const sf::Image *image );

		/** Swap (never throws).
		 * @param  rhs Other TextureManager.
		 */
		void swap( TextureManager& rhs );

	private:
		typedef SharedPtr< Primitive::Texture > TextureHandle;
		typedef std::map< const sf::Image *, TextureHandle > TextureMap;

		TextureMap m_textures;
};

}
}
}

/** Swap (never throws).
* @param rhs First TextureManager to swap.
* @param lhs Second TextureManager to swap.
*/
void swap( sfg::eng::bob::TextureManager& rhs, sfg::eng::bob::TextureManager& lhs );
