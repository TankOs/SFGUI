#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/ResourceManager.hpp>

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <memory>

namespace sf {
class String;
class Color;
class Font;

SFGUI_API std::ostream& operator<<( std::ostream& stream, const Color& color );
SFGUI_API std::istream& operator>>( std::istream& stream, Color& color );
}

namespace sfg {

class Widget;
class Window;
class Button;
class Label;
class Entry;
class Scale;
class Scrollbar;
class ScrolledWindow;
class ToggleButton;
class CheckButton;
class ProgressBar;
class Separator;
class Frame;
class Image;
class Notebook;
class Spinner;
class ComboBox;
class SpinButton;

class Selector;
class RenderQueue;

namespace parser {
namespace theme {
struct Rule;

typedef std::vector<Rule> Theme;
}
}

/** Abstract base class for widget rendering.
 */
class SFGUI_API Engine {
	public:
		typedef std::runtime_error BadValueException; //<! Thrown when value can't be converted to or from string.

		/** Dtor.
		 */
		virtual ~Engine() = default;

		/** Create drawable for window widgets.
		 * @param window Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateWindowDrawable( std::shared_ptr<const Window> window ) const = 0;

		/** Create drawable for button widgets.
		 * @param button Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateButtonDrawable( std::shared_ptr<const Button> button ) const = 0;

		/** Create drawable for toggle button widgets.
		 * @param button Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateToggleButtonDrawable( std::shared_ptr<const ToggleButton> button ) const = 0;

		/** Create drawable for check button widgets.
		 * @param check Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateCheckButtonDrawable( std::shared_ptr<const CheckButton> check ) const = 0;

		/** Create drawable for label widgets.
		 * @param label Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateLabelDrawable( std::shared_ptr<const Label> label ) const = 0;

		/** Create drawable for entry widgets.
		 * @param entry Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateEntryDrawable( std::shared_ptr<const Entry> entry ) const = 0;

		/** Create drawable for scale widgets.
		 * @param scale Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateScaleDrawable( std::shared_ptr<const Scale> scale ) const = 0;

		/** Create drawable for scrollbar widgets.
		 * @param scrollbar Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateScrollbarDrawable( std::shared_ptr<const Scrollbar> scrollbar ) const = 0;

		/** Create drawable for scrolled window widgets.
		 * @param scrolled_window Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateScrolledWindowDrawable( std::shared_ptr<const ScrolledWindow> scrolled_window ) const = 0;

		/** Create drawable for progress bar widgets.
		 * @param progress_bar Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateProgressBarDrawable( std::shared_ptr<const ProgressBar> progress_bar ) const = 0;

		/** Create drawable for seperator widgets.
		 * @param seperator Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateSeparatorDrawable( std::shared_ptr<const Separator> seperator ) const = 0;

		/** Create drawable for frame widgets.
		 * @param frame Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateFrameDrawable( std::shared_ptr<const Frame> frame ) const = 0;

		/** Create drawable for combo box widgets.
		 * @param combo_box Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateComboBoxDrawable( std::shared_ptr<const ComboBox> combo_box ) const = 0;

		/** Create drawable for image widgets.
		 * @param image Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateImageDrawable( std::shared_ptr<const Image> image ) const = 0;

		/** Create drawable for notebook widgets.
		 * @param notebook Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateNotebookDrawable( std::shared_ptr<const Notebook> notebook ) const = 0;

		/** Create drawable for spinner widgets.
		 * @param spinner Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateSpinnerDrawable( std::shared_ptr<const Spinner> spinner ) const = 0;

		/** Create drawable for spinbutton widgets.
		 * @param spinbutton Widget.
		 * @return New drawable object (unmanaged memory!).
		 */
		virtual std::unique_ptr<RenderQueue> CreateSpinButtonDrawable( std::shared_ptr<const SpinButton> spinbutton ) const = 0;

		/** Get maximum line height.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Line height.
		 */
		float GetFontLineHeight( const sf::Font& font, unsigned int font_size ) const;

		/** Get baseline offset.
		 * The offset needed to be applied to get from the text origin y to the baseline y.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Baseline offset.
		 */
		float GetFontBaselineOffset( const sf::Font& font, unsigned int font_size ) const;

		/** Get line spacing of a font. The space between 2 baselines.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Line spacing.
		 */
		float GetFontLineSpacing( const sf::Font& font, unsigned int font_size ) const;

		/** Get metrics of a text string.
		 * @param string String.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Metrics.
		 */
		sf::Vector2f GetTextStringMetrics( const std::basic_string<sf::Uint32>& string, const sf::Font& font, unsigned int font_size ) const;

		/** Get metrics of a text string.
		 * @param string String.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return Metrics.
		 */
		sf::Vector2f GetTextStringMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size ) const;

		/** Clear all properties.
		 */
		void ClearProperties();

		/** Set property.
		 * @param selector selector string (leave empty for all widgets).
		 * @param property Property.
		 * @param value Value.
		 * @return true on success, false when: Invalid selector or invalid property.
		 * @throws BadValue when value couldn't be converted to string.
		 */
		template <typename T>
		bool SetProperty( const std::string& selector, const std::string& property, const T& value );

		/** Set property.
		 * @param selector Valid selector object.
		 * @param property Property.
		 * @param value Value.
		 * @return true on success, false when: Invalid selector or invalid property.
		 * @throws BadValue when value couldn't be converted to string.
		 */
		template <typename T>
		bool SetProperty( std::shared_ptr<Selector> selector, const std::string& property, const T& value );

		/** Set property.
		 * @param selector Valid selector object.
		 * @param property Property.
		 * @param value Value.
		 * @return true on success, false when: Invalid selector or invalid property.
		 */
		bool SetProperty( std::shared_ptr<Selector> selector, const std::string& property, const std::string& value );

		/** Set multiple properties at once.
		 * @param properties CSS-like rule declarations.
		 * @return true on success, false when: rule could not be parsed.
		 */
		bool SetProperties( const std::string& properties );

		/** Get property.
		 * @param property Name of property.
		 * @param widget Widget to be used for building the property path.
		 * @return Value or T() in case property doesn't exist.
		 */
		template <typename T>
		T GetProperty( const std::string& property, std::shared_ptr<const Widget> widget = std::shared_ptr<const Widget>() ) const;

		/** Load a theme from file.
		 * @param filename Filename.
		 * @return true on success, false otherwise.
		 */
		bool LoadThemeFromFile( const std::string& filename );

		/** Load a theme from string.
		 * @param data Theme data.
		 * @return true on success, false otherwise.
		 */
		bool LoadThemeFromString( const std::string& data );

		/** Shift the given border colors to make them lighter and darker.
		 * @param light_color Color of the lighter border.
		 * @param dark_color Color of the darker border.
		 * @param offset Amount to shift by.
		 */
		void ShiftBorderColors( sf::Color& light_color, sf::Color& dark_color, int offset ) const;

		/** Get resource manager.
		 * @return Resource manager.
		 */
		ResourceManager& GetResourceManager() const;

	protected:
		/** Ctor.
		 */
		Engine();

		/** Call this to enable or disable automatic widget refreshing.
		 * @param enable true to enable automatic widget refreshing.
		 */
		void SetAutoRefresh( bool enable );

	private:
		typedef std::pair<std::shared_ptr<const Selector>, std::string> SelectorValuePair;
		typedef std::vector<SelectorValuePair> SelectorValueList;
		typedef std::map<const std::string, SelectorValueList> WidgetNameMap;
		typedef std::map<const std::string, WidgetNameMap> PropertyMap;

		const std::string* GetValue( const std::string& property, std::shared_ptr<const Widget> widget ) const;

		/** Get maximum line height and baseline offset of a font.
		 * @param font Font.
		 * @param font_size Font size.
		 * @return sf::Vector2f containing line height in x and baseline offset in y.
		 */
		sf::Vector2f GetFontHeightProperties( const sf::Font& font, unsigned int font_size ) const;

		void ParseTheme( const parser::theme::Theme& theme_to_parse );

		PropertyMap m_properties;

		mutable ResourceManager m_resource_manager;

		bool m_auto_refresh;
};

}

#include "Engine.inl"
