#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

/** Culling RenderTarget.
 * Prevents sf::Drawables outside the visible viewing region
 * from being rendered.
 */
class SFGUI_API CullingTarget {
	public:
		/** Ctor.
		 * @param target SFML RenderTarget.
		 */
		CullingTarget( sf::RenderTarget& target );

		/** Set the viewport of this target.
		 * @param view Viewport.
		 */
		void SetView( const sf::View& view );

		/** Get the viewport of this target.
		 * @return Viewport.
		 */
		const sf::View& GetView() const;

		/** Get the width of this target.
		 * @return Width.
		 */
		unsigned int GetWidth() const;

		/** Get the height of this target.
		 * @return Height.
		 */
		unsigned int GetHeight() const;

		/** Get the keep/discard cull counters.
		 * @return keep/discard cull counters.
		 */
		const std::pair<std::size_t, std::size_t>& GetCount() const;

		/** Reset cull counters.
		 */
		void ResetCount();

		/** Draw a RenderQueue
		 * @param queue RenderQueue.
		 */
		void Draw( const RenderQueue& queue );

		/** Enable/Disable culling.
		 * @param enable true to enable culling, false to disable (default: true);
		 */
		void Cull( bool enable = true );

	private:
		sf::RenderTarget& m_real_target;

		std::pair<std::size_t, std::size_t> m_cull_count;

		sf::FloatRect m_view_rect;
		sf::FloatRect m_viewport;

		bool m_cull;
};

}
