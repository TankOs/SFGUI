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

		std::unique_ptr<RenderQueue> CreateWindowDrawable( std::shared_ptr<const Window> window ) const;
		std::unique_ptr<RenderQueue> CreateButtonDrawable( std::shared_ptr<const Button> button ) const;
		std::unique_ptr<RenderQueue> CreateToggleButtonDrawable( std::shared_ptr<const ToggleButton> button ) const;
		std::unique_ptr<RenderQueue> CreateCheckButtonDrawable( std::shared_ptr<const CheckButton> check ) const;
		std::unique_ptr<RenderQueue> CreateLabelDrawable( std::shared_ptr<const Label> label ) const;
		std::unique_ptr<RenderQueue> CreateEntryDrawable( std::shared_ptr<const Entry> entry ) const;
		std::unique_ptr<RenderQueue> CreateScaleDrawable( std::shared_ptr<const Scale> scale ) const;
		std::unique_ptr<RenderQueue> CreateScrollbarDrawable( std::shared_ptr<const Scrollbar> scrollbar ) const;
		std::unique_ptr<RenderQueue> CreateScrolledWindowDrawable( std::shared_ptr<const ScrolledWindow> scrolled_window ) const;
		std::unique_ptr<RenderQueue> CreateProgressBarDrawable( std::shared_ptr<const ProgressBar> progress_bar ) const;
		std::unique_ptr<RenderQueue> CreateSeparatorDrawable( std::shared_ptr<const Separator> seperator ) const;
		std::unique_ptr<RenderQueue> CreateFrameDrawable( std::shared_ptr<const Frame> frame ) const;
		std::unique_ptr<RenderQueue> CreateImageDrawable( std::shared_ptr<const Image> image ) const;
		std::unique_ptr<RenderQueue> CreateNotebookDrawable( std::shared_ptr<const Notebook> notebook ) const;
		std::unique_ptr<RenderQueue> CreateSpinnerDrawable( std::shared_ptr<const Spinner> spinner ) const;
		std::unique_ptr<RenderQueue> CreateComboBoxDrawable( std::shared_ptr<const ComboBox> combo_box ) const;
		std::unique_ptr<RenderQueue> CreateSpinButtonDrawable( std::shared_ptr<const SpinButton> spinbutton ) const;

	private:
		static std::unique_ptr<RenderQueue> CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );
		static std::unique_ptr<RenderQueue> CreateSlider( const sf::FloatRect& rect, sf::Color& background_color, float border_width, const sf::Color& border_color, int border_color_shift );
		static std::unique_ptr<RenderQueue> CreateStepper( const sf::FloatRect& rect, sf::Color& background_color, float border_width, const sf::Color& border_color, int border_color_shift, bool pressed = false );
};

}
}
