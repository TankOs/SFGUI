#include <SFGUI/Engines/Bob.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/ComboBox.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> Bob::CreateComboBoxDrawable( std::shared_ptr<const ComboBox> combo_box ) const {
	const auto& font_name = GetProperty<std::string>( "FontName", combo_box );
	auto font_size = GetProperty<unsigned int>( "FontSize", combo_box );
	auto padding = GetProperty<float>( "ItemPadding", combo_box );
	auto padding_h = GetProperty<float>( "ItemPaddingHorizontally", combo_box );
	auto highlight_padding = GetProperty<float>( "HighlightPadding", combo_box );
	auto color = GetProperty<sf::Color>( "Color", combo_box );
	const auto& font = GetResourceManager().GetFont( font_name );
	auto line_height = GetFontLineHeight( *font, font_size );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	// Field.
	queue->Add( CreateSpritebox( sf::FloatRect( 0.f, 0.f, combo_box->GetAllocation().width, combo_box->GetAllocation().height ),
								 GetResourceManager().GetImage( GetProperty<std::string>( "FieldImage", combo_box ) ),
								 UintRect() ) );

	// Text.
	if( combo_box->GetSelectedItem() != ComboBox::NONE ) {
		sf::Text text( combo_box->GetSelectedText(), *font, font_size );
		text.setPosition(
			padding_h,
			combo_box->GetAllocation().height / 2.f - line_height / 2.f
		);
		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	// Arrow.
	auto arrow_image( GetResourceManager().GetImage( GetProperty<std::string>( "ArrowImage", combo_box ) ) );
	PrimitiveTexture::Ptr arrow_texture( m_texture_manager.GetTexture( arrow_image ) );

	sf::Vector2f arrow_size = static_cast<sf::Vector2f>( arrow_image->getSize() );

	queue->Add( Renderer::Get().CreateSprite( sf::FloatRect( combo_box->GetAllocation().width - padding_h - arrow_size.x,
															 ( combo_box->GetAllocation().height  - arrow_size.y ) / 2.f,
															 arrow_size.x,
															 arrow_size.y ),
											  arrow_texture, sf::FloatRect() ) );

	if( combo_box->IsDropDownDisplayed() ) {
		const sf::Vector2f item_size( combo_box->GetAllocation().width, line_height + 2 * padding );
		sf::Vector2f item_position( 0.f, combo_box->GetAllocation().height );

		auto expanded_height = static_cast<float>( combo_box->GetDisplayedItemCount() ) * item_size.y;

		// Popup Pane
		queue->Add( CreateSpritebox( sf::FloatRect( 0.f, combo_box->GetAllocation().height, combo_box->GetAllocation().width, expanded_height ),
								     GetResourceManager().GetImage( GetProperty<std::string>( "PopUpImage", combo_box ) ),
									 UintRect() ) );


		// Items.
		for( ComboBox::IndexType item_index = combo_box->GetDisplayedItemStart(); item_index < combo_box->GetDisplayedItemStart() + combo_box->GetDisplayedItemCount(); ++item_index ) {
			if( combo_box->GetItem( item_index ).getSize() == 0 ) {
				continue;
			}

			// Highlight selected Item.
			if( item_index == combo_box->GetHighlightedItem() ) {
				queue->Add( CreateSpritebox( sf::FloatRect( item_position.x + highlight_padding, item_position.y + highlight_padding,
															item_size.x - 2.f * highlight_padding, item_size.y - 2.f * highlight_padding ),
											 GetResourceManager().GetImage( GetProperty<std::string>( "HighlightImage", combo_box ) ),
											 UintRect() ) );
			}

			// Labels.
			sf::Text text( combo_box->GetItem( item_index ), *font, font_size );
			text.setPosition( item_position.x + padding, item_position.y + padding );
			text.setColor( color );
			queue->Add( Renderer::Get().CreateText( text ) );

			item_position.y += item_size.y;
		}
	}

	return queue;

}

}
}
