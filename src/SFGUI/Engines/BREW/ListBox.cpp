#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/ListBox.hpp>
#include <SFGUI/RenderQueue.hpp>

#include <SFML/Graphics/Text.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateListBoxDrawable( std::shared_ptr<const ListBox> listbox ) const {
    auto border_color = GetProperty<sf::Color>( "BorderColor", listbox );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", listbox );
    auto highlighted_color = GetProperty<sf::Color>( "HighlightedColor", listbox );
    auto selected_color = GetProperty<sf::Color>( "SelectedColor", listbox );
	auto text_color = GetProperty<sf::Color>( "Color", listbox );
	auto text_padding = GetProperty<float>( "Padding", listbox );
	auto border_width = GetProperty<float>( "BorderWidth", listbox );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", listbox );
	const auto& font_name = GetProperty<std::string>( "FontName", listbox );
	const auto& font = GetResourceManager().GetFont( font_name );
	auto font_size = GetProperty<unsigned int>( "FontSize", listbox );

    std::unique_ptr<RenderQueue> queue( new RenderQueue );

    // Pane.
	queue->Add(
		Renderer::Get().CreatePane(
			sf::Vector2f( 0.f, 0.f ),
			sf::Vector2f( listbox->GetAllocation().width, listbox->GetAllocation().height ),
			border_width,
			background_color,
			border_color,
			-border_color_shift
		)
	);

    // Items.
    sf::Vector2f itemPosition = sf::Vector2f( border_width + text_padding, border_width + text_padding );
    for( ListBox::IndexType i = listbox->GetFirstDisplayedItemIndex();
        i < std::min(listbox->GetFirstDisplayedItemIndex() + listbox->GetMaxDisplayedItemsCount(), listbox->GetItemsCount());
        ++i ) {
        auto& itemText = listbox->GetDisplayedItemText( i );

        auto metrics = GetTextStringMetrics( itemText, *font, font_size );
		metrics.y = GetFontLineHeight( *font, font_size ); // Text only size

        auto item_height = listbox->GetItemHeight(); // Height of an item (including the image)

        sf::Text text( itemText, *font, font_size );
        text.setPosition(
            itemPosition.x + ( listbox->GetImagesSize() != sf::Vector2f() ? text_padding + listbox->GetImagesSize().x : 0.f ),
            itemPosition.y + item_height/2.f - metrics.y / 2.f
        );
        text.setColor(text_color);

        if( listbox->IsItemSelected( i ) ) {
            queue->Add(
                Renderer::Get().CreateRect(
                    sf::FloatRect(
                        itemPosition.x - text_padding,
                        itemPosition.y - text_padding,
                        listbox->GetAllocation().width - 2 * border_width,
                        std::min(item_height + text_padding * 2, listbox->GetAllocation().height - (itemPosition.y - text_padding) - border_width)
                        // Avoid the selection box to go further than the widget's height when the last displayed item padding space is not fully displayed.
                    ),
                    selected_color
                )
            );
        } else if( i == listbox->GetHighlightedItem() ) {
            queue->Add(
                Renderer::Get().CreateRect(
                    sf::FloatRect(
                        itemPosition.x - text_padding,
                        itemPosition.y - text_padding,
                        listbox->GetAllocation().width - 2 * border_width,
                        std::min(item_height + text_padding * 2, listbox->GetAllocation().height - (itemPosition.y - text_padding) - border_width)
                        // Avoid the highlight box to go further than the widget's height when the last displayed item padding space is not fully displayed.
                    ),
                    highlighted_color
                )
            );
        }

        if( listbox->GetImagesSize() != sf::Vector2f() && listbox->GetItemImage( i ).getSize() != sf::Vector2u() ) {
            auto texture = Renderer::Get().LoadTexture( listbox->GetItemImage( i ) );
            queue->Add(
        		Renderer::Get().CreateSprite(
        			sf::FloatRect(
        				itemPosition.x,
        				itemPosition.y,
        				listbox->GetImagesSize().x,
        				listbox->GetImagesSize().y
        			),
        			texture
        		)
        	);
        }

        queue->Add( Renderer::Get().CreateText(text) );

        itemPosition += sf::Vector2f( 0.f, item_height + text_padding * 2 );
    }

    return queue;
}

}
}
