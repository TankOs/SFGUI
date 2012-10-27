#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {
class RenderQueue;

namespace eng {

/** Bob (Blitter OBject) -- Bitmap Rendering Engine.
 */
class SFGUI_API Bob : public Engine {
	public:
		/** Ctor.
		 */
		Bob();

		/** Reset properties to defaults.
		 */
		void ResetProperties();

		RenderQueue* CreateWindowDrawable( SharedPtr<const Window> window ) const;
		RenderQueue* CreateButtonDrawable( SharedPtr<const Button> button ) const;
		RenderQueue* CreateToggleButtonDrawable( SharedPtr<const ToggleButton> button ) const;
		RenderQueue* CreateCheckButtonDrawable( SharedPtr<const CheckButton> check ) const;
		RenderQueue* CreateLabelDrawable( SharedPtr<const Label> label ) const;
		RenderQueue* CreateEntryDrawable( SharedPtr<const Entry> entry ) const;
		RenderQueue* CreateScaleDrawable( SharedPtr<const Scale> scale ) const;
		RenderQueue* CreateScrollbarDrawable( SharedPtr<const Scrollbar> scrollbar ) const;
		RenderQueue* CreateScrolledWindowDrawable( SharedPtr<const ScrolledWindow> scrolled_window ) const;
		RenderQueue* CreateProgressBarDrawable( SharedPtr<const ProgressBar> progress_bar ) const;
		RenderQueue* CreateSeparatorDrawable( SharedPtr<const Separator> seperator ) const;
		RenderQueue* CreateFrameDrawable( SharedPtr<const Frame> frame ) const;
		RenderQueue* CreateImageDrawable( SharedPtr<const Image> image ) const;
		RenderQueue* CreateNotebookDrawable( SharedPtr<const Notebook> notebook ) const;
		RenderQueue* CreateSpinnerDrawable( SharedPtr<const Spinner> spinner ) const;
		RenderQueue* CreateComboBoxDrawable( SharedPtr<const ComboBox> combo_box ) const;
		RenderQueue* CreateSpinButtonDrawable( SharedPtr<const SpinButton> spinbutton ) const;
};

}
}
