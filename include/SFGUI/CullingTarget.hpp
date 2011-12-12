#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <deque>
#include <map>

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

		/** Push a new viewport onto this target.
		 * @param view Viewport.
		 */
		void PushView( const sf::View& view );

		/** Pop the current viewport from this target.
		 * @param view Viewport.
		 */
		void PopView();

		/** Get the current viewport of this target.
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
		void Draw( RenderQueue* queue );

		/** Enable/Disable culling.
		 * @param enable true to enable culling, false to disable (default: true);
		 */
		void Cull( bool enable = true );

		/** Display buffered queues to the final target.
		 */
		void Display();

	private:
		struct ViewCachePair {
			unsigned int id;
			sf::IntRect aabb;
		};

		void UpdateView();

		std::deque<RenderQueue*> m_frame_buffer;
		std::map<unsigned int, sf::View> m_view_map;

		std::vector<sf::View> m_view_stack;
		std::vector<ViewCachePair> m_view_cache;

		sf::FloatRect m_view_rect;
		sf::FloatRect m_viewport;

		sf::IntRect m_view_aabb;

		std::pair<std::size_t, std::size_t> m_cull_count;

		sf::RenderTarget& m_real_target;

		unsigned int m_current_view_id;
		unsigned int m_last_view_id;

		bool m_cull;

		bool m_out_of_view;
};

}
