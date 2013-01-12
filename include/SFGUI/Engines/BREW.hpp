#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Engine.hpp>

namespace sf {
class ConvexShape;
}

namespace sfg {
class RenderQueue;

namespace eng {

/** BREW -- Basic Rendering Engine for Widget (reference implementation).
 */
class SFGUI_API BREW : public Engine {
	public:
		/** Ctor.
		 */
		BREW();

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

	private:
		static RenderQueue* CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );
		static RenderQueue* CreateSlider( const sf::FloatRect& rect, sf::Color& background_color, float border_width, const sf::Color& border_color, int border_color_shift );
		static RenderQueue* CreateStepper( const sf::FloatRect& rect, sf::Color& background_color, float border_width, const sf::Color& border_color, int border_color_shift, bool pressed = false );
};

}
}
