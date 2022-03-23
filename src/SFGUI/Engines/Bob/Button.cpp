#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Button.hpp>

#include <SFGUI/Engines/Bob/Spritebox.hpp>

namespace sfg {
namespace eng {

RenderQueue* Bob::CreateButtonDrawable( SharedPtr<const Button> button ) const {
	RenderQueue* queue( new RenderQueue );

	//Spritebox
	bob::Spritebox spritebox;
	spritebox.SetDimension( sf::Vector2i( static_cast<int>( button->GetAllocation().width ), static_cast<int>( button->GetAllocation().height ) ) );

	const sf::Image *image( GetResourceManager().GetImage( GetProperty<std::string>( "Image", button ) ) );
	if( image == NULL )
		return queue;

	SharedPtr< Primitive::Texture > texture_handle( m_texture_manager.GetTexture( image ) );
	spritebox.SetTexture( texture_handle );

	Primitive::Ptr primitive = spritebox.ConstructPrimitive();

	Renderer::Get().AddPrimitive( primitive );
	queue->Add( primitive );

	//Label
	if( button->GetLabel().getSize() > 0 ) {
		const std::string& font_name( GetProperty<std::string>( "FontName", button ) );
		unsigned int font_size( GetProperty<unsigned int>( "FontSize", button ) );
		sf::Color color( GetProperty<sf::Color>( "Color", button ) );
		float spacing( GetProperty<float>( "Spacing", button ) );

		const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
		sf::Vector2f metrics = GetTextMetrics( button->GetLabel(), font, font_size );
		metrics.y = GetFontLineHeight( font, font_size );

		sf::Text text( button->GetLabel(), font, font_size );
		Widget::PtrConst child( button->GetChild() );

		if( !child ) {
			text.setPosition(
				button->GetAllocation().width / 2.f - metrics.x / 2.f,
				button->GetAllocation().height / 2.f - metrics.y / 2.f
			);
		}
		else {
			float width( button->GetAllocation().width - spacing - child->GetAllocation().width );

			text.setPosition(
				child->GetAllocation().width + spacing + (width / 2.f - metrics.x / 2.f),
				button->GetAllocation().height / 2.f - metrics.y / 2.f
			);
		}

		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	return queue;
}

}
}
