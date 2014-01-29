#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Engines/Bob/TextureManager.hpp>
#include <SFGUI/Engines/Bob/StreamOperators.hpp>

namespace sfg {
class RenderQueue;

namespace eng {

typedef sf::Rect< unsigned int > UintRect;

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

	protected:
		Primitive::Ptr CreateArrow( const sf::FloatRect& rect, unsigned int rotation, std::shared_ptr<const sf::Image> image ) const;
		Primitive::Ptr CreateSpritebox( const sf::FloatRect& rect, std::shared_ptr<const sf::Image> image, UintRect sub_rect ) const;

	private:
		mutable bob::TextureManager m_texture_manager;
};

}
}
