#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/RendererTextureNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <map>
#include <deque>
#include <memory>

namespace sf {
class Window;
class RenderTarget;
class RenderWindow;
class RenderTexture;
class Texture;
class Font;
class Text;
class Image;
}

namespace sfg {

class RendererViewport;
class Primitive;
class PrimitiveTexture;
class Signal;

/** SFGUI Renderer interface.
 */
class SFGUI_API Renderer {
	public:
		enum InvalidateType : char {
			INVALIDATE_VERTEX = 1 << 0, //!< Vertex data needs a sync.
			INVALIDATE_COLOR = 1 << 1, //!< Color data needs a sync.
			INVALIDATE_TEXTURE = 1 << 2, //!< Texture data needs a sync.
			INVALIDATE_INDEX = 1 << 3, //!< Index data needs a sync.
			INVALIDATE_ALL = INVALIDATE_VERTEX | INVALIDATE_COLOR | INVALIDATE_TEXTURE | INVALIDATE_INDEX //!< All data needs a sync.
		};

		Renderer( const Renderer& ) = delete;
		Renderer& operator=( const Renderer& ) = delete;

		/** Create the Renderer singleton instance.
		 * SFGUI will automatically detect what is the best renderer for your given hardware.
		 * @return Renderer instance.
		 */
		static Renderer& Create();

		/** Get the Renderer singleton instance.
		 * @return Renderer instance.
		 */
		static Renderer& Get();

		/** Set the Renderer singleton instance.
		 * @param renderer Renderer instance.
		 */
		static void Set( std::shared_ptr<Renderer> renderer );

		/** Destroy the Renderer singleton instance.
		 */
		static void Destroy();

		/** Check if a Renderer has been instantiated
		 * @return true if a Renderer instance exists.
		 */
		static bool Exists();

		/** Dtor.
		 */
		virtual ~Renderer();

		/** Get default viewport that covers the entire window.
		 * @return Default viewport that covers the entire window.
		 */
		std::shared_ptr<RendererViewport> GetDefaultViewport();

		/** Create and register a new viewport with the renderer.
		 * @return New viewport.
		 */
		std::shared_ptr<RendererViewport> CreateViewport();

		/** Create and register a new text primitive with the renderer.
		 * @param text sf::Text describing the text to be drawn.
		 * @return New text primitive.
		 */
		std::shared_ptr<Primitive> CreateText( const sf::Text& text );

		/** Create and register a new quad primitive with the renderer.
		 * @param top_left Top left corner of the quad.
		 * @param bottom_left Bottom left corner of the quad.
		 * @param bottom_right Bottom right corner of the quad.
		 * @param top_right Top right corner of the quad.
		 * @param color Color of the quad.
		 * @return New quad primitive.
		 */
		std::shared_ptr<Primitive> CreateQuad( const sf::Vector2f& top_left, const sf::Vector2f& bottom_left,
		                           const sf::Vector2f& bottom_right, const sf::Vector2f& top_right,
		                           const sf::Color& color = sf::Color::White );

		/** Create and register a new pane (rect with border) primitive with the renderer.
		 * @param position Position of the pane (top left corner).
		 * @param size Size of the pane.
		 * @param border_width Width of the border.
		 * @param color Color of the rect.
		 * @param border_color Color of the border.
		 * @param border_color_shift Amount to shift border colors by to create a 3D effect.
		 * @return New pane primitive.
		 */
		std::shared_ptr<Primitive> CreatePane( const sf::Vector2f& position, const sf::Vector2f& size, float border_width,
		                           const sf::Color& color = sf::Color::White, const sf::Color& border_color = sf::Color::Black,
		                           int border_color_shift = 0 );

		/** Create and register a new rect primitive with the renderer.
		 * @param top_left Top left corner of the rect.
		 * @param bottom_right Bottom right corner of the rect.
		 * @param color Color of the rect.
		 * @return New rect primitive.
		 */
		std::shared_ptr<Primitive> CreateRect( const sf::Vector2f& top_left, const sf::Vector2f& bottom_right, const sf::Color& color = sf::Color::White );

		/** Create and register a new rect primitive with the renderer.
		 * @param rect sf::FloatRect describing the rect to be constructed.
		 * @param color Color of the rect.
		 * @return New rect primitive.
		 */
		std::shared_ptr<Primitive> CreateRect( const sf::FloatRect& rect, const sf::Color& color = sf::Color::White );

		/** Create and register a new triangle primitive with the renderer.
		 * Keep in mind that the points have to be specified in counter-clockwise order.
		 * @param point0 First point of the triangle.
		 * @param point1 Second point of the triangle.
		 * @param point2 Third point of the triangle.
		 * @param color Color of the triangle.
		 * @return New triangle primitive.
		 */
		std::shared_ptr<Primitive> CreateTriangle( const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& color = sf::Color::White );

		/** Create and register a new sprite primitive with the renderer.
		 * Sprite primitives are primitives that have a bound texture that
		 * was manually loaded prior to creation of the primitive. Subrects
		 * can also be specified to use only portions of the passed texture.
		 * @param rect sf::FloatRect describing the rect in which the sprite should be drawn.
		 * @param texture Texture loaded previously with LoadImage.
		 * @param subrect Subrectangle for specifying smaller regions of the texture to use. (0,0,0,0) to use whole texture.
		 * @param rotation_turns Turns to rotate the texture by in COUNTERCLOCKWISE direction. 1 turn is 90 degrees, -1 turn is -90 degrees etc. 0 to not rotate.
		 * @return New sprite primitive.
		 */
		std::shared_ptr<Primitive> CreateSprite( const sf::FloatRect& rect, std::shared_ptr<PrimitiveTexture> texture, const sf::FloatRect& subrect = sf::FloatRect( 0.f, 0.f, 0.f, 0.f ), int rotation_turns = 0 );

		/** Create and register a new line primitive with the renderer.
		 * @param begin Starting point of the line.
		 * @param end End point of the line.
		 * @param color Color of the line.
		 * @param thickness Thickness of the line.
		 * @return New line primitive.
		 */
		std::shared_ptr<Primitive> CreateLine( const sf::Vector2f& begin, const sf::Vector2f& end, const sf::Color& color = sf::Color::White, float thickness = 1.f );

		/** Create a canvas to draw custom GL stuff on.
		 * @param callback Signal containing draw routines to call.
		 * @return New canvas primitive.
		 */
		std::shared_ptr<Primitive> CreateGLCanvas( std::shared_ptr<Signal> callback );

		/** Register a primitive with the renderer.
		 * @param primitive Primitive to be registered.
		 */
		void AddPrimitive( std::shared_ptr<Primitive> primitive );

		/** Unregister a primitive from the renderer.
		 * @param primitive Primitive to be unregistered.
		 */
		void RemovePrimitive( std::shared_ptr<Primitive> primitive );

		/// @cond

		/** Load a Font at the given size and retrieve the texture atlas offset.
		 * @param font sf::Font containing the font.
		 * @param size Size of the font.
		 * @return Offset into the atlas the font texture is located at.
		 */
		sf::Vector2f LoadFont( const sf::Font& font, unsigned int size );

		/** Load an sf::Texture into the atlas and return a handle to the allocated texture.
		 * This merely copies the data from the origin sf::Texture into the atlas.
		 * @param texture sf::Texture containing the texture data.
		 * @return Shared handle to the allocated texture.
		 */
		std::shared_ptr<PrimitiveTexture> LoadTexture( const sf::Texture& texture );

		/** Load an sf::Image into the atlas and return a handle to the allocated texture.
		 * @param image sf::Image containing the image data.
		 * @return Shared handle to the allocated texture.
		 */
		std::shared_ptr<PrimitiveTexture> LoadTexture( const sf::Image& image );

		/** Unload the image at the given offset from the texture atlas.
		 * @param offset Offset of the image in the texture atlas.
		 */
		void UnloadImage( const sf::Vector2f& offset );

		/** Update the image at the given offset.
		 * The size of the old and new image must match.
		 * @param offset The offset the image is at in the texture atlas.
		 * @param data New image data.
		 */
		void UpdateImage( const sf::Vector2f& offset, const sf::Image& data );

		/// @endcond

		/** Invalidate renderer datasets so they are resynchronized with fresh data.
		 * @param datasets The datasets to invalidate. Default: INVALIDATE_ALL
		 * Bitwise OR of INVALIDATE_VERTEX, INVALIDATE_COLOR, INVALIDATE_TEXTURE or INVALIDATE_INDEX.
		 */
		void Invalidate( unsigned char datasets = INVALIDATE_ALL );

		/** Draw the GUI to an sf::Window.
		 * @param target sf::Window to draw to.
		 */
		virtual void Display( sf::Window& target ) const = 0;

		/** Draw the GUI to an sf::RenderWindow.
		 * @param target sf::RenderWindow to draw to.
		 */
		virtual void Display( sf::RenderWindow& target ) const = 0;

		/** Draw the GUI to an sf::RenderTexture.
		 * @param target sf::RenderTexture to draw to.
		 */
		virtual void Display( sf::RenderTexture& target ) const = 0;

		/** Force the renderer to discard its cache's FBO image and redraw.
		 */
		void Redraw();

		/** Get the size of the window the last time the GUI was displayed.
		 * @return Size of the window the last time the GUI was displayed.
		 */
		const sf::Vector2i& GetWindowSize() const;

		/** Add a required character set to the character sets that the Renderer will load for new fonts.
		 * This is required if using a script whose glyphs are not smaller than codepoint 0x370
		 * such as Greek, Cyrillic, Hebrew, Arabic etc.
		 * By default, only the Latin script and its extensions are loaded.
		 * @param low_bound Lower boundary of the character set, i.e. the glyph with the smallest codepoint.
		 * @param high_bound Higher boundary of the character set, i.e. the glyph with the largest codepoint.
		 */
		void AddCharacterSet( sf::Uint32 low_bound, sf::Uint32 high_bound );

		/** Get name of the Renderer.
		 * The name of a Renderer is a descriptive name of the Renderer itself. E.g.
		 * "Vertex Buffer Renderer" for the VertexBufferRenderer.
		 * @return Name of the Renderer.
		 */
		virtual const std::string& GetName() const = 0;

	protected:
		typedef std::pair<void*, unsigned int> FontID;

		/** Ctor.
		 */
		Renderer();

		virtual void InvalidateImpl( unsigned char datasets );

		void SortPrimitives();

		int GetMaxTextureSize() const;

		void WipeStateCache( sf::RenderTarget& target ) const;

		std::vector<std::shared_ptr<Primitive>> m_primitives;
		std::vector<std::unique_ptr<sf::Texture>> m_texture_atlas;

		std::shared_ptr<RendererViewport> m_default_viewport;

		int m_vertex_count;
		int m_index_count;

		mutable sf::Vector2i m_window_size;
		mutable sf::Vector2i m_last_window_size;
		mutable bool m_force_redraw;

	private:
		virtual void DisplayImpl() const = 0;

		std::deque<priv::RendererTextureNode> m_textures;
		std::map<FontID, std::shared_ptr<PrimitiveTexture>> m_fonts;
		std::vector<std::pair<sf::Uint32, sf::Uint32>> m_character_sets;

		std::shared_ptr<PrimitiveTexture> m_pseudo_texture;

		bool m_primitives_sorted;
};

}
