#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Notebook.hpp>

namespace sfg {
namespace eng {

RenderQueue* BREW::CreateNotebookDrawable( SharedPtr<const Notebook> notebook ) const {
	sf::Color border_color( GetProperty<sf::Color>( "BorderColor", notebook ) );
	sf::Color border_color_light( border_color );
	sf::Color border_color_dark( border_color );
	int border_color_shift( GetProperty<int>( "BorderColorShift", notebook ) );
	sf::Color background_color( GetProperty<sf::Color>( "BackgroundColor", notebook ) );
	sf::Color background_color_dark( GetProperty<sf::Color>( "BackgroundColorDark", notebook ) );
	sf::Color background_color_prelight( GetProperty<sf::Color>( "BackgroundColorPrelight", notebook ) );
	float padding( GetProperty<float>( "Padding", notebook ) );
	float border_width( GetProperty<float>( "BorderWidth", notebook ) );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	RenderQueue* queue( new RenderQueue );

	Notebook::IndexType page_count = notebook->GetPageCount();

	if( !page_count ) {
		return queue;
	}

	Notebook::IndexType current_page = notebook->GetCurrentPage();
	Notebook::IndexType prelight_tab = notebook->GetPrelightTab();

	// Get size in the dimension all tabs have uniform size.
	sf::Vector2f tab_size( notebook->GetNthTabLabel( 0 )->GetAllocation().Width, notebook->GetNthTabLabel( 0 )->GetAllocation().Height );

	// Get size in the dimension all children have uniform size.
	sf::Vector2f child_size( notebook->GetNthPage( 0 )->GetAllocation().Width, notebook->GetNthPage( 0 )->GetAllocation().Height );

	if( notebook->GetTabPosition() == Notebook::TOP ) {
		// Tabs are positioned at top.

		// Pane.
		queue->Add(
			Context::Get().GetProjectO().CreatePane(
				sf::Vector2f( 0.f, tab_size.y + 2.f * ( border_width + padding ) ),
				sf::Vector2f( child_size.x + 2.f * ( border_width + padding ), child_size.y + 2.f * ( border_width + padding ) ),
				border_width,
				background_color,
				border_color,
				border_color_shift
			)
		);

		// First tab label left border
		queue->Add(
			Context::Get().GetProjectO().CreateLine(
				sf::Vector2f( 0.f, 0.f ),
				sf::Vector2f( 0.f, tab_size.y + 3.f * border_width + 2.f * padding ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Top border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top - border_width - padding ),
					sf::Vector2f( label_allocation.Left + label_allocation.Width + padding, label_allocation.Top - border_width - padding ),
					border_color_light,
					border_width
				)
			);

			// Right border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left + label_allocation.Width - border_width + padding, label_allocation.Top - border_width - padding ),
					sf::Vector2f( label_allocation.Left + label_allocation.Width - border_width + padding, label_allocation.Top + label_allocation.Height + border_width + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active left border
				queue->Add(
					Context::Get().GetProjectO().CreateLine(
						sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top - border_width - padding ),
						sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top + label_allocation.Height + 2.f * border_width + padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding - border_width,
							label_allocation.Height + 2.f * ( border_width + padding )
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding - border_width,
							label_allocation.Height + border_width + 2.f * padding - border_width
						),
						( index == prelight_tab ) ? background_color_prelight : background_color_dark
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::BOTTOM ) {
		// Tabs are positioned at bottom.

		// Pane.
		queue->Add(
			Context::Get().GetProjectO().CreatePane(
				sf::Vector2f( 0.f, 0.f ),
				sf::Vector2f( child_size.x + 2.f * ( border_width + padding ), child_size.y + 2.f * ( border_width + padding ) ),
				border_width,
				background_color,
				border_color,
				border_color_shift
			)
		);

		// First tab label left border
		queue->Add(
			Context::Get().GetProjectO().CreateLine(
				sf::Vector2f( 0.f, child_size.y + 2.f * ( border_width + padding ) - border_width ),
				sf::Vector2f( 0.f, child_size.y + 2.f * ( border_width + padding ) + tab_size.y + 2.f * ( border_width + padding ) - border_width ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Bottom border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top + label_allocation.Height + border_width + padding ),
					sf::Vector2f( label_allocation.Left + label_allocation.Width + padding, label_allocation.Top + label_allocation.Height + border_width + padding ),
					border_color_dark,
					border_width
				)
			);

			// Right border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left + label_allocation.Width + padding - border_width, label_allocation.Top - padding ),
					sf::Vector2f( label_allocation.Left + label_allocation.Width + padding - border_width, label_allocation.Top + label_allocation.Height + border_width + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active left border
				queue->Add(
					Context::Get().GetProjectO().CreateLine(
						sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top - padding ),
						sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top + label_allocation.Height + border_width + padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding,
							label_allocation.Top - padding - border_width,
							label_allocation.Width + 2.f * padding - border_width,
							label_allocation.Height + 2.f * padding + 2.f * border_width
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding - border_width,
							label_allocation.Height + 2.f * padding + border_width
						),
						( index == prelight_tab ) ? background_color_prelight : background_color_dark
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::LEFT ) {
		// Tabs are positioned at left.

		// Pane.
		queue->Add(
			Context::Get().GetProjectO().CreatePane(
				sf::Vector2f( tab_size.x + 2.f * ( border_width + padding ), 0.f ),
				sf::Vector2f( child_size.x + 2.f * ( border_width + padding ), child_size.y + 2.f * ( border_width + padding ) ),
				border_width,
				background_color,
				border_color,
				border_color_shift
			)
		);

		// First tab label top border
		queue->Add(
			Context::Get().GetProjectO().CreateLine(
				sf::Vector2f( 0.f, 0.f ),
				sf::Vector2f( tab_size.x + 2.f * padding + 3.f * border_width, 0.f ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Left border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top - border_width - padding ),
					sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top + label_allocation.Height + border_width + padding ),
					border_color_light,
					border_width
				)
			);

			// Bottom border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top + label_allocation.Height + padding ),
					sf::Vector2f( label_allocation.Left + label_allocation.Width + border_width + padding, label_allocation.Top + label_allocation.Height + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active top border
				queue->Add(
					Context::Get().GetProjectO().CreateLine(
						sf::Vector2f( label_allocation.Left - border_width - padding, label_allocation.Top - border_width - padding ),
						sf::Vector2f( label_allocation.Left + label_allocation.Width + 2.f * border_width + padding, label_allocation.Top - border_width - padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * ( border_width + padding ),
							label_allocation.Height + 2.f * padding
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding,
							label_allocation.Height + 2.f * padding - border_width
						),
						( index == prelight_tab ) ? background_color_prelight : background_color_dark
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::RIGHT ) {
		// Tabs are positioned at right.

		// Pane.
		queue->Add(
			Context::Get().GetProjectO().CreatePane(
				sf::Vector2f( 0.f, 0.f ),
				sf::Vector2f( child_size.x + 2.f * ( border_width + padding ), child_size.y + 2.f * ( border_width + padding ) ),
				border_width,
				background_color,
				border_color,
				border_color_shift
			)
		);

		// First tab label top border
		queue->Add(
			Context::Get().GetProjectO().CreateLine(
				sf::Vector2f( child_size.x + 2.f * ( border_width + padding ), 0.f ),
				sf::Vector2f( child_size.x + 4.f * padding + 3.f * border_width + tab_size.x, 0.f ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( Notebook::IndexType index = 0; index < page_count; ++index ) {
			Widget::Ptr label = notebook->GetNthTabLabel( index );
			sf::FloatRect label_allocation = label->GetAllocation();

			// Right border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left + label_allocation.Width + border_width + padding, label_allocation.Top - border_width - padding ),
					sf::Vector2f( label_allocation.Left + label_allocation.Width + border_width + padding, label_allocation.Top + label_allocation.Height + border_width + padding ),
					border_color_dark,
					border_width
				)
			);

			// Bottom border
			queue->Add(
				Context::Get().GetProjectO().CreateLine(
					sf::Vector2f( label_allocation.Left - padding, label_allocation.Top + label_allocation.Height + padding ),
					sf::Vector2f( label_allocation.Left + label_allocation.Width + border_width + padding, label_allocation.Top + label_allocation.Height + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active top border
				queue->Add(
					Context::Get().GetProjectO().CreateLine(
						sf::Vector2f( label_allocation.Left - padding - border_width, label_allocation.Top - border_width - padding ),
						sf::Vector2f( label_allocation.Left + label_allocation.Width + border_width + padding, label_allocation.Top - border_width - padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding - border_width,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding + 2.f * border_width,
							label_allocation.Height + 2.f * padding
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Context::Get().GetProjectO().CreateRect(
						sf::FloatRect(
							label_allocation.Left - padding,
							label_allocation.Top - padding,
							label_allocation.Width + 2.f * padding + border_width,
							label_allocation.Height + 2.f * padding
						),
						(index == prelight_tab) ? background_color_prelight : background_color_dark
					)
				);
			}
		}
	}

	return queue;
}

}
}
