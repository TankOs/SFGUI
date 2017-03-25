#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <memory>
#include <random>
#include <string>

class MyCustomWidget : public sfg::Widget {
	public:
		// The standard shared_ptr typedefs.
		typedef std::shared_ptr<MyCustomWidget> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const MyCustomWidget> PtrConst; //!< Shared pointer.

		// Every widget should have a Create() method.
		// It can take any parameters you want, but must return a Ptr.
		static Ptr Create( const sf::String& label = L"" ) {
			auto ptr = Ptr( new MyCustomWidget );
			ptr->SetLabel( label );
			return ptr;
		}

		// Every widget must have a GetName() method
		// that returns the name of the widget type.
		const std::string& GetName() const override {
			static const std::string name( "MyCustomWidget" );
			return name;
		}

		// Widgets can have as many methods as you need.
		void SetLabel( const sf::String& label ) {
			m_label = label;
			RequestResize();
			Invalidate();
		}

		// Same as above.
		const sf::String& GetLabel() const {
			return m_label;
		}

	protected:
		// The widget constructor is usually protected so it
		// can only be created through the Create() method.
		MyCustomWidget() :
			m_generator( m_random_device() ),
			m_distribution( -5, 5 ),
			m_color_distribution( 0, 255 )
		{

		}

		// InvalidateImpl is called by SFGUI whenever something happens
		// that might make the widget look different. It returns an
		// sfg::RenderQueue containing the graphical elements that will
		// be rendered to display the widget on the screen.
		std::unique_ptr<sfg::RenderQueue> InvalidateImpl() const override {
			// In order to support engine themes, you should not hard-code
			// graphical parameters, but instead retrieve the properties you need
			// from the currently active engine. These properties determine
			// what your widget looks like and can be adjusted to affect a
			// large number of widgets at the same time.
			auto background_color = sfg::Context::Get().GetEngine().GetProperty<sf::Color>( "BackgroundColor", shared_from_this() );
			const auto& font_name = sfg::Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() );
			auto font_size = sfg::Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() );

			// Fonts are stored in the engine resource manager. Once you have the name
			// of a font that has been loaded, you can get it from the resource manager.
			const auto& font = sfg::Context::Get().GetEngine().GetResourceManager().GetFont( font_name );

			std::unique_ptr<sfg::RenderQueue> queue( new sfg::RenderQueue );

			if( GetLabel().getSize() > 0 ) {
				auto metrics = sfg::Context::Get().GetEngine().GetTextStringMetrics( GetLabel(), *font, font_size );
				metrics.y = sfg::Context::Get().GetEngine().GetFontLineHeight( *font, font_size );

				// SFGUI widgets are made out of elementary pieces.
				// You have to request the renderer to give you each of
				// these pieces through one of the Create* methods.
				// You add these pieces to the RenderQueue that you return and it
				// will be rendered where the widget is displayed on the screen

				// Refer to the sfg::Renderer documentation for more information
				// on the Create* methods.

				auto inverted_color = sf::Color::White - background_color;
				inverted_color.a = 255;

				// Outer pane.
				queue->Add(
					sfg::Renderer::Get().CreatePane(
						sf::Vector2f( 0.f, 0.f ),
						sf::Vector2f( GetAllocation().width, GetAllocation().height ),
						5.f,
						inverted_color,
						background_color,
						20.f
					)
				);

				auto inner_border_color = sf::Color::Green;

				// If the widget is active (currently pressed) change the inner border to red
				if( GetState() == State::ACTIVE ) {
					inner_border_color = sf::Color::Red;
				}

				// Inner pane.
				queue->Add(
					sfg::Renderer::Get().CreatePane(
						sf::Vector2f( GetAllocation().width / 4.f, GetAllocation().height / 4.f ),
						sf::Vector2f( GetAllocation().width / 2.f, GetAllocation().height / 2.f ),
						5.f,
						sf::Color(
							static_cast<sf::Uint8>( m_color_distribution( m_generator ) ),
							static_cast<sf::Uint8>( m_color_distribution( m_generator ) ),
							static_cast<sf::Uint8>( m_color_distribution( m_generator ) ),
							255
						),
						inner_border_color,
						20.f
					)
				);

				sf::Text text( GetLabel(), *font, font_size );

				// Set the text color to white.
				text.setFillColor( sf::Color::White );

				// Randomize the applied offset a bit
				auto x_offset = ( GetState() == State::ACTIVE ) ? static_cast<float>( m_distribution( m_generator ) ) : 0.f;
				auto y_offset = ( GetState() == State::ACTIVE ) ? static_cast<float>( m_distribution( m_generator ) ) : 0.f;

				text.setPosition(
					GetAllocation().width / 2.f - metrics.x / 2.f + x_offset,
					GetAllocation().height / 2.f - metrics.y / 2.f + y_offset
				);

				// Text.
				queue->Add( sfg::Renderer::Get().CreateText( text ) );
			}

			return queue;
		}

		// In order for SFGUI to know how much space your widget takes up
		// and position other widgets accordingly, it calls the
		// CalculateRequisition() method. This method should return the
		// total screen size of the widget.
		sf::Vector2f CalculateRequisition() override {
			// The size of a widget might depend on the current properties
			const std::string& font_name( sfg::Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
			unsigned int font_size( sfg::Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
			const sf::Font& font( *sfg::Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

			// The current engine can also be used to retrieve font metric
			// data size as size or line height.
			auto requisition = sfg::Context::Get().GetEngine().GetTextStringMetrics( m_label, font, font_size );
			requisition.y = sfg::Context::Get().GetEngine().GetFontLineHeight( font, font_size );

			requisition *= 3.f;
			requisition += sf::Vector2f( 20.f, 20.f );

			return requisition;
		}

		// You can override certain methods in order to be informed
		// when certain events happen. This handler handles the state change
		// event and is called when the current state of the widget changes.
		void HandleStateChange( State old_state ) override {
			sfg::Widget::HandleStateChange( old_state );
		}

	private:
		// This handler handles mouse enter events.
		void HandleMouseEnter( int x, int y ) override {
			SetLabel( sf::String( "Mouse Enter: " + std::to_string( x ) + "," + std::to_string( y ) ) );

			if( GetState() == State::NORMAL ) {
				SetState( State::PRELIGHT );
			}
		}

		// This handler handles mouse leave events.
		void HandleMouseLeave( int x, int y ) override {
			SetLabel( sf::String( "Mouse Leave: " + std::to_string( x ) + "," + std::to_string( y ) ) );

			if( GetState() == State::PRELIGHT ) {
				SetState( State::NORMAL );
			}
		}

		// This handler handles mouse button events
		void HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) override {
			if( !IsMouseInWidget() ) {
				if( GetState() == State::ACTIVE ) {
					SetState( State::NORMAL );
				}

				return;
			}

			if( button == sf::Mouse::Left ) {
				if( press ) {
					SetLabel( sf::String( "Mouse Left Press: " + std::to_string( x ) + "," + std::to_string( y ) ) );
					SetState( State::ACTIVE );
				}
				else if( GetState() == State::ACTIVE ) {
					SetState( State::PRELIGHT );
				}
			}
		}

		sf::String m_label;

		std::random_device m_random_device;
		mutable std::mt19937 m_generator;
		mutable std::uniform_int_distribution<> m_distribution;
		mutable std::uniform_int_distribution<> m_color_distribution;
};

int main() {
	// Create SFML's window.
	sf::RenderWindow render_window( sf::VideoMode( 800, 600 ), "Custom Widget" );

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our custom widget.
	auto custom_widget = MyCustomWidget::Create( "Custom Text" );

	// Create a simple button and connect the click signal.
	auto button = sfg::Button::Create( "Button" );
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&custom_widget] { custom_widget->SetLabel( L"You just clicked the other guy!" ); } );

	// Create a vertical box layouter with 5 pixels spacing and add our custom widget and button
	// and button to it.
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack( custom_widget );
	box->Pack( button, false );

	// Create a window and add the box layouter to it. Also set the window's title.
	auto window = sfg::Window::Create();
	window->SetTitle( "Custom Widget" );
	window->Add( box );

	// Create a desktop and add the window to it.
	sfg::Desktop desktop;
	desktop.Add( window );

	// We're not using SFML to render anything in this program, so reset OpenGL
	// states. Otherwise we wouldn't see anything.
	render_window.resetGLStates();

	// Main loop!
	sf::Event event;
	sf::Clock clock;

	while( render_window.isOpen() ) {
		// Event processing.
		while( render_window.pollEvent( event ) ) {
			desktop.HandleEvent( event );

			// If window is about to be closed, leave program.
			if( event.type == sf::Event::Closed ) {
				return 0;
			}
		}

		// Update SFGUI with elapsed seconds since last call.
		desktop.Update( clock.restart().asSeconds() );

		// Rendering.
		render_window.clear();
		sfgui.Display( render_window );
		render_window.display();
	}

	return 0;
}
