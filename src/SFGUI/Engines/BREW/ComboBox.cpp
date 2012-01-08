#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/ComboBox.hpp>

#include <SFML/Graphics/Text.hpp>
#include <cmath>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateComboBoxDrawable( SharedPtr<const ComboBox> combo_box ) const {
	sf::Color border_color_light( GetProperty<sf::Color>( "BorderColor", combo_box ) );
	sf::Color border_color_dark( border_color_light );
	int border_color_shift( GetProperty<int>( "BorderColorShift", combo_box ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", combo_box ) );
	sf::Color highlighted_color( GetProperty<sf::Color>( "HighlightedColor", combo_box ) );
	sf::Color color( GetProperty<sf::Color>( "Color", combo_box ) );
	sf::Color arrow_color( GetProperty<sf::Color>( "ArrowColor", combo_box ) );
	float border_width( GetProperty<float>( "BorderWidth", combo_box ) );
	const std::string& font_name( GetProperty<std::string>( "FontName", combo_box ) );
	unsigned int font_size( GetProperty<unsigned int>( "FontSize", combo_box ) );
	float padding( GetProperty<float>( "ItemPadding", combo_box ) );
	const sf::Font& font( *GetResourceManager().GetFont( font_name ) );
	const float line_height( GetLineHeight( font, font_size ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	// Background.
	queue->Add(
		Context::Get().GetProjectO().CreateRect(
			sf::FloatRect(
				0.f,
				0.f,
				combo_box->GetAllocation().Width,
				combo_box->GetAllocation().Height
			),
			background_color
		)
	);

	if( combo_box->GetState() != ComboBox::ACTIVE ) {
		queue->Add(
			CreateBorder(
				sf::FloatRect(
					0.f,
					0.f,
					combo_box->GetAllocation().Width,
					combo_box->GetAllocation().Height
				),
				border_width,
				border_color_light,
				border_color_dark
			)
		);
	}
	else {
		queue->Add(
			CreateBorder(
				sf::FloatRect(
					0.f,
					0.f,
					combo_box->GetAllocation().Width,
					combo_box->GetAllocation().Height
				),
				border_width,
				border_color_dark,
				border_color_light
			)
		);
	}

	// Labels.
	if( combo_box->IsPoppedUp() ) {
		const sf::Vector2f item_size(
			combo_box->GetAllocation().Width - 2 * border_width,
			line_height + 2 * padding
		);
		sf::Vector2f item_position(
			0.f,
			combo_box->GetAllocation().Height
		);

		float expanded_height = static_cast<float>( combo_box->GetItemCount() ) * item_size.y;

		// Popup background.
		queue->Add(
			Context::Get().GetProjectO().CreateRect(
				sf::FloatRect(
					0.f,
					combo_box->GetAllocation().Height,
					combo_box->GetAllocation().Width,
					expanded_height
				),
				background_color
			)
		);

		for( ComboBox::IndexType item_index = 0; item_index < combo_box->GetItemCount(); ++item_index ) {
			if( combo_box->GetItem( item_index ).GetSize() == 0 ) {
				continue;
			}

			if( item_index == combo_box->GetHighlightedItem() ) {
				// Highlighted item background.
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							item_position.x + border_width,
							item_position.y,
							item_size.x + border_width,
							item_size.y
						),
						highlighted_color
					)
				);
			}

			sf::Text* text( new sf::Text( combo_box->GetItem( item_index ), font, font_size ) );
			text->SetPosition( item_position.x + padding, item_position.y + padding );
			text->SetColor( color );
			queue->Add( Context::Get().GetProjectO().CreateText( *text ) );

			delete text;

			item_position.y += item_size.y;
		}

		queue->Add(
			CreateBorder(
				sf::FloatRect(
					0.f,
					combo_box->GetAllocation().Height,
					combo_box->GetAllocation().Width,
					expanded_height
				),
				border_width,
				border_color_light,
				border_color_dark
			)
		);
	}

	if( combo_box->GetSelectedItem() != ComboBox::NONE ) {
		sf::Text* text( new sf::Text( combo_box->GetSelectedText(), font, font_size ) );
		text->SetPosition(
			border_width + padding,
			combo_box->GetAllocation().Height / 2.f - line_height / 2.f
		);
		text->SetColor( color );
		queue->Add( Context::Get().GetProjectO().CreateText( *text ) );

		delete text;
	}

	// Arrow.
	sf::Vector2f position(
		( combo_box->GetState() == ComboBox::ACTIVE ? border_width : 0.f ) + combo_box->GetAllocation().Width - padding - GetLineHeight( font, font_size ),
		( combo_box->GetState() == ComboBox::ACTIVE ? border_width : 0.f ) + padding
	);

	queue->Add(
		Context::Get().GetProjectO().CreateTriangle(
			position + sf::Vector2f(
				GetLineHeight( font, font_size ) / 4.f,
				GetLineHeight( font, font_size ) / 4.f
			),
			position + sf::Vector2f(
				GetLineHeight( font, font_size ) * 3.f / 4.f,
				GetLineHeight( font, font_size ) / 4.f
			),
			position + sf::Vector2f(
				GetLineHeight( font, font_size ) / 2.f,
				GetLineHeight( font, font_size ) * 3.f / 4.f
			),
			arrow_color
		)
	);

	return queue;
}

}
}
