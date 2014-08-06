#pragma once

#include <SFGUI/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <memory>

namespace sfg {

/** Renderer viewport.
 */
class RendererViewport {
	public:
		typedef std::shared_ptr<RendererViewport> Ptr; //!< Shared pointer.

		/** Ctor.
		 */
		RendererViewport();

		/** Set origin of the source rectangle.
		 * @param origin Origin of the source rectangle.
		 */
		void SetSourceOrigin( const sf::Vector2f& origin );

		/** Get origin of the source rectangle.
		 * @return Origin of the source rectangle.
		 */
		const sf::Vector2f& GetSourceOrigin() const;

		/** Set origin of the destination rectangle.
		 * @param origin Origin of the destination rectangle.
		 */
		void SetDestinationOrigin( const sf::Vector2f& origin );

		/** Get origin of the destination rectangle.
		 * @return Origin of the destination rectangle.
		 */
		const sf::Vector2f& GetDestinationOrigin() const;

		/** Set size of the viewport rectangle.
		 * @param size Size of the viewport rectangle.
		 */
		void SetSize( const sf::Vector2f& size );

		/** Get size of the viewport rectangle.
		 * @return Size of the viewport rectangle.
		 */
		const sf::Vector2f& GetSize() const;

		/** Equality operator.
		 * @param other RendererViewport to compare with.
		 * @return true if both RendererViewports represent the same viewport area.
		 */
		bool operator==( const RendererViewport& other ) const;

		/** Inequality operator.
		 * @param other RendererViewport to compare with.
		 * @return false if both RendererViewports represent the same viewport area.
		 */
		bool operator!=( const RendererViewport& other ) const;

	private:
		sf::Vector2f m_source_origin;
		sf::Vector2f m_destination_origin;
		sf::Vector2f m_size;

		std::size_t m_id;
};

}
