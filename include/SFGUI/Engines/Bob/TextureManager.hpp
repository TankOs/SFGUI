#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Primitive.hpp>

#include <map>
#include <memory>

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
class SFGUI_API TextureManager{
	public:
		/** Ctor.
		 * Creates an empty TextureManager.
		 */
		TextureManager();

		/** Dtor.
		 */
		~TextureManager();

		/// @cond
		// Fix for VS2013 not supporting = default move members.
#if defined( _MSC_VER )
		/** Deleted Copy Ctor.
		 */
		TextureManager( const TextureManager& ) = delete;

		/** Deleted Copy Assignment.
		 */
		TextureManager& operator=( const TextureManager& ) = delete;
#else
		/** Move Ctor.
		 */
		TextureManager( TextureManager&& ) = default;

		/** Move Assignment.
		 */
		TextureManager& operator=( TextureManager&& ) = default;
#endif
		/// @endcond

		/** Get a texture from a given image.
		 * @param  image Image to load.
		 * @return SharedPtr to the loaded texture.
		 */
		std::shared_ptr< Primitive::Texture > GetTexture( std::shared_ptr<const sf::Image> image );

		/** Unload texture from a given loaded image.
		 * @param  image Image to Unload.
		 */
		void UnloadTexture( std::shared_ptr<const sf::Image> image );

		/** Swap (never throws).
		 * @param  rhs Other TextureManager.
		 */
		void swap( TextureManager& rhs );

	private:
		typedef std::shared_ptr< Primitive::Texture > TextureHandle;
		typedef std::map< std::shared_ptr<const sf::Image>, TextureHandle > TextureMap;

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
