#pragma once
#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/RendererViewport.hpp>
#include <SFGUI/SharedPtr.hpp>

namespace sfg {

/** GLCanvas.
 */
class SFGUI_API GLCanvas : public Widget {
	public:
		typedef SharedPtr<GLCanvas> Ptr; //!< Shared pointer.
		typedef SharedPtr<const GLCanvas> PtrConst; //!< Shared pointer.

		/** Create GLCanvas.
		 * @return GLCanvas.
		 */
		static Ptr Create();

		/** Get the signal containing the callbacks to custom draw routines.
		 * @return Signal containing the callbacks to custom draw routines.
		 */
		Signal& GetCustomDrawCallbackSignal();

		virtual const std::string& GetName() const;

		/** Handle changing of absolute position
		 */
		virtual void HandleAbsolutePositionChange();

		/** Set the minimum desired refresh rate in FPS that the canvas should update at.
		 * @param rate Refresh rate in FPS. 0 to disable forced refreshing.
		 */
		void SetDesiredRefreshRate( float rate );

		/// @cond

		/** Reset the internal update timer.
		 */
		void ResetUpdateTime();

		/// @endcond

		/** Force a redraw of the canvas.
		 */
		void Redraw();

	protected:
		sf::Vector2f CalculateRequisition();

		virtual void HandleSizeChange();

		virtual RenderQueue* InvalidateImpl() const;

	private:
		GLCanvas();

		virtual void HandleUpdate( float seconds );

		SharedPtr<Signal> m_custom_draw_callback;
		SharedPtr<RendererViewport> m_custom_viewport;

		float m_last_update;
		float m_desired_refresh_rate;
};

}
