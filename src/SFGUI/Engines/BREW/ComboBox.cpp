#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateComboBoxDrawable( std::shared_ptr<const ComboBox> combo_box ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", combo_box );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", combo_box );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", combo_box );
	auto highlighted_color = GetProperty<sf::Color>( "HighlightedColor", combo_box );
	auto color = GetProperty<sf::Color>( "Color", combo_box );
	auto arrow_color = GetProperty<sf::Color>( "ArrowColor", combo_box );
	auto border_width = GetProperty<float>( "BorderWidth", combo_box );
	const auto& font_name = GetProperty<std::string>( "FontName", combo_box );
	auto font_size = GetProperty<unsigned int>( "FontSize", combo_box );
	auto padding = GetProperty<float>( "ItemPadding", combo_box );
	const auto& font = GetResourceManager().GetFont( font_name );
	auto line_height = GetFontLineHeight( *font, font_size );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	if( combo_box->GetState() == ComboBox::State::ACTIVE ) {
		border_color_shift = -border_color_shift;
	}

	// Pane
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( combo_box->GetAllocation().width, combo_box->GetAllocation().height ),
			border_width,
			background_color,
			border_color,
			border_color_shift
		)
	);

	if( combo_box->IsDropDownDisplayed() ) {
		const sf::Vector2f item_size(
			combo_box->GetAllocation().width - 2 * border_width,
			line_height + 2 * padding
		);
		sf::Vector2f item_position(
			0.f,
			combo_box->GetAllocation().height
		);

		auto expanded_height = static_cast<float>( combo_box->GetDisplayedItemCount() ) * item_size.y;

		// Popup Pane
		queue->Add(
			Renderer::Get().CreatePane(
				sf::Vector2f( 0.f, combo_box->GetAllocation().height ),
				sf::Vector2f( combo_box->GetAllocation().width, expanded_height ),
				border_width,
				background_color,
				border_color,
				-border_color_shift
			)
		);

		// Labels.
		for( ComboBox::IndexType item_index = combo_box->GetDisplayedItemStart(); item_index < combo_box->GetDisplayedItemStart() + combo_box->GetDisplayedItemCount(); ++item_index ) {
			if( combo_box->GetItem( item_index ).getSize() == 0 ) {
				continue;
			}

			if( item_index == combo_box->GetHighlightedItem() ) {
				// Highlighted item background.
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							item_position.x + border_width,
							item_position.y + border_width,
							item_size.x,
							item_size.y - 2.f * border_width
						),
						highlighted_color
					)
				);
			}

			sf::Text text( combo_box->GetItem( item_index ), *font, font_size );
			text.setPosition( item_position.x + padding, item_position.y + padding );
			text.setColor( color );
			queue->Add( Renderer::Get().CreateText( text ) );

			item_position.y += item_size.y;
		}
	}

	if( combo_box->GetSelectedItem() != ComboBox::NONE ) {
		sf::Text text( combo_box->GetSelectedText(), *font, font_size );
		text.setPosition(
			border_width + padding,
			combo_box->GetAllocation().height / 2.f - line_height / 2.f
		);
		text.setColor( color );
		queue->Add( Renderer::Get().CreateText( text ) );
	}

	// Arrow.
	sf::Vector2f position(
		( combo_box->GetState() == ComboBox::State::ACTIVE ? border_width : 0.f ) + combo_box->GetAllocation().width - padding - GetFontLineHeight( *font, font_size ),
		( combo_box->GetState() == ComboBox::State::ACTIVE ? border_width : 0.f ) + padding
	);

	queue->Add(
		Renderer::Get().CreateTriangle(
			position + sf::Vector2f(
				GetFontLineHeight( *font, font_size ) / 2.f,
				GetFontLineHeight( *font, font_size ) * 3.f / 4.f
			),
			position + sf::Vector2f(
				GetFontLineHeight( *font, font_size ) * 3.f / 4.f,
				GetFontLineHeight( *font, font_size ) / 4.f
			),
			position + sf::Vector2f(
				GetFontLineHeight( *font, font_size ) / 4.f,
				GetFontLineHeight( *font, font_size ) / 4.f
			),
			arrow_color
		)
	);

	return queue;
}

}
}
