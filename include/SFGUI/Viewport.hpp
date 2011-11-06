#pragma once
#include <SFGUI/Bin.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace sfg {

/** Viewport.
 */
class SFGUI_API Viewport : public Bin {
	public:
		typedef std::shared_ptr<Viewport>  Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Viewport>  PtrConst; //!< Shared pointer.

		/** Create viewport.
		 * @param viewport Viewport.
		 * @return Viewport.
		 */
		static Ptr Create();

		/** Expose.
		 * Render widget to given target.
		 * @param target SFML render target.
		 */
		void Expose( sf::RenderTarget& target );

		/** Get absolute position on screen.
		 * @return Absolute position.
		 */
		virtual sf::Vector2f GetAbsolutePosition() const;

		/** Set the horizontal adjustment of this Viewport
		 * @param horizontal_adjustment Horizontal Adjustment
		 */
		void SetHorizontalAdjustment( Adjustment::Ptr horizontal_adjustment );

		/** Set the vertical adjustment of this Viewport
		 * @param vertical_adjustment Vertical Adjustment
		 */
		void SetVerticalAdjustment( Adjustment::Ptr vertical_adjustment );

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals.
		 * @return true when event has been processed (eaten).
		 */
		virtual void HandleEvent( const sf::Event& event );

		/** Used to inform parent that a child has been invalidated
		 * @param child Widget that was invalidated.
		 */
		virtual void HandleChildInvalidate( Widget::Ptr child );

		virtual const std::string& GetName() const;

	protected:
		sf::Vector2f GetRequisitionImpl() const;

		virtual void HandleSizeAllocate( const sf::FloatRect& old_allocation );

		void HandleAdjustmentChange();

	private:
		Viewport();
		void RecreateRenderTexture();

		Adjustment::Ptr m_horizontal_adjustment;
		Adjustment::Ptr m_vertical_adjustment;

		sf::RenderTexture m_render_texture;
		sf::Sprite m_sprite;

		bool m_rebuild_child;
};

}
