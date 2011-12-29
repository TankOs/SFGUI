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

	private:
		enum Edge {
			TOP = 0,
			RIGHT,
			BOTTOM,
			LEFT
		};

		static sf::Shape* CreateBackground( const sf::FloatRect& rect, const sf::Color& color );
		static sf::Shape* CreateLine( Edge where, const sf::Vector2f& from, const sf::Vector2f& to, const sf::Color& color, float thickness = 1.f );

		static RenderQueue* CreateBorder( const sf::FloatRect& rect, float border_width, const sf::Color& light_color, const sf::Color& dark_color );
		static RenderQueue* CreateSlider( const sf::FloatRect& rect, sf::Color& background, float border_width, const sf::Color& light_color, const sf::Color& dark_color );
		static RenderQueue* CreateStepper( const sf::FloatRect& rect, sf::Color& background, float border_width, const sf::Color& light_color, const sf::Color& dark_color, bool pressed = false );
};

}
}
