#include <SFGUI/Engines/BREW.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {
namespace eng {

std::unique_ptr<RenderQueue> BREW::CreateNotebookDrawable( std::shared_ptr<const Notebook> notebook ) const {
	auto border_color = GetProperty<sf::Color>( "BorderColor", notebook );
	auto border_color_light( border_color );
	auto border_color_dark( border_color );
	auto border_color_shift = GetProperty<int>( "BorderColorShift", notebook );
	auto background_color = GetProperty<sf::Color>( "BackgroundColor", notebook );
	auto background_color_dark = GetProperty<sf::Color>( "BackgroundColorDark", notebook );
	auto background_color_prelight = GetProperty<sf::Color>( "BackgroundColorPrelight", notebook );
	auto padding = GetProperty<float>( "Padding", notebook );
	auto border_width = GetProperty<float>( "BorderWidth", notebook );
	auto scroll_button_size = GetProperty<float>( "ScrollButtonSize", notebook );
	auto arrow_color = GetProperty<sf::Color>( "Color", notebook );
	auto scroll_button_prelight = GetProperty<sf::Color>( "ScrollButtonPrelightColor", notebook );

	ShiftBorderColors( border_color_light, border_color_dark, border_color_shift );

	std::unique_ptr<RenderQueue> queue( new RenderQueue );

	auto page_count = notebook->GetPageCount();

	if( !page_count ) {
		return queue;
	}

	auto current_page = notebook->GetCurrentPage();
	auto prelight_tab = notebook->GetPrelightTab();

	// Get size in the dimension all tabs have uniform size.
	sf::Vector2f tab_size( notebook->GetNthTabLabel( 0 )->GetAllocation().width, notebook->GetNthTabLabel( 0 )->GetAllocation().height );

	// Get size in the dimension all children have uniform size.
	sf::Vector2f child_size( notebook->GetNthPage( 0 )->GetAllocation().width, notebook->GetNthPage( 0 )->GetAllocation().height );

	if( notebook->GetTabPosition() == Notebook::TabPosition::TOP ) {
		// Tabs are positioned at top.

		// Pane.
		queue->Add(
			Renderer::Get().CreatePane(
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
			Renderer::Get().CreateLine(
				sf::Vector2f( notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f, 0.f ),
				sf::Vector2f( notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f, tab_size.y + 3.f * border_width + 2.f * padding ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( auto index = notebook->GetFirstDisplayedTab(); index < notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount(); ++index ) {
			auto label = notebook->GetNthTabLabel( index );
			auto label_allocation = label->GetAllocation();

			// Top border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top - border_width - padding ),
					sf::Vector2f( label_allocation.left + label_allocation.width + padding - border_width, label_allocation.top - border_width - padding ),
					border_color_light,
					border_width
				)
			);

			// Right border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left + label_allocation.width - border_width + padding, label_allocation.top - border_width - padding ),
					sf::Vector2f( label_allocation.left + label_allocation.width - border_width + padding, label_allocation.top + label_allocation.height + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active left border
				queue->Add(
					Renderer::Get().CreateLine(
						sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top - border_width - padding ),
						sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top + label_allocation.height + border_width + padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding,
							label_allocation.top - padding,
							label_allocation.width + 2.f * padding - border_width,
							label_allocation.height + 2.f * ( border_width + padding )
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding,
							label_allocation.top - padding,
							label_allocation.width + 2.f * padding - border_width,
							label_allocation.height + 2.f * padding
						),
						( index == prelight_tab ) ? background_color_prelight : background_color_dark
					)
				);
			}
		}

		if( notebook->GetScrollable() ) {
			// Forward button
			if( ( notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount() ) < notebook->GetPageCount() ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size, 0.f ),
						sf::Vector2f( scroll_button_size, tab_size.y + 2.f * ( padding + border_width ) ),
						border_width,
						notebook->IsForwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingForward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size * .66f, .33f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size * .66f, .66f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size * .33f, .5f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						arrow_color
					)
				);
			}

			// Backward button
			if( notebook->GetFirstDisplayedTab() != 0 ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( 0.f, 0.f ),
						sf::Vector2f( scroll_button_size, tab_size.y + 2.f * ( padding + border_width ) ),
						border_width,
						notebook->IsBackwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingBackward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( scroll_button_size * .66f, .66f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( scroll_button_size * .66f, .33f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( scroll_button_size * .33f, .5f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						arrow_color
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::TabPosition::BOTTOM ) {
		// Tabs are positioned at bottom.

		// Pane.
		queue->Add(
			Renderer::Get().CreatePane(
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
			Renderer::Get().CreateLine(
				sf::Vector2f( notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f, child_size.y + 2.f * border_width + 2.f * padding ),
				sf::Vector2f( notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f, child_size.y + tab_size.y + 3.f * border_width + 4.f * padding ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( auto index = notebook->GetFirstDisplayedTab(); index < notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount(); ++index ) {
			auto label = notebook->GetNthTabLabel( index );
			auto label_allocation = label->GetAllocation();

			// Bottom border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left - padding, label_allocation.top + label_allocation.height + border_width + padding ),
					sf::Vector2f( label_allocation.left + label_allocation.width - border_width + padding, label_allocation.top + label_allocation.height + border_width + padding ),
					border_color_dark,
					border_width
				)
			);

			// Right border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left + label_allocation.width + padding - border_width, label_allocation.top - padding ),
					sf::Vector2f( label_allocation.left + label_allocation.width + padding - border_width, label_allocation.top + label_allocation.height + border_width + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active left border
				queue->Add(
					Renderer::Get().CreateLine(
						sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top - padding ),
						sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top + label_allocation.height + border_width + padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding,
							label_allocation.top - padding - border_width,
							label_allocation.width + 2.f * padding - border_width,
							label_allocation.height + 2.f * padding + 2.f * border_width
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding,
							label_allocation.top - padding,
							label_allocation.width + 2.f * padding - border_width,
							label_allocation.height + 2.f * padding + border_width
						),
						( index == prelight_tab ) ? background_color_prelight : background_color_dark
					)
				);
			}
		}

		if( notebook->GetScrollable() ) {
			// Forward button
			if( ( notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount() ) < notebook->GetPageCount() ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size, notebook->GetAllocation().height - ( scroll_button_size + padding ) ),
						sf::Vector2f( scroll_button_size, tab_size.y + 2.f * ( padding + border_width ) ),
						border_width,
						notebook->IsForwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingForward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size * .66f, notebook->GetAllocation().height - ( scroll_button_size + padding ) + .33f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size * .66f, notebook->GetAllocation().height - ( scroll_button_size + padding ) + .66f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( notebook->GetAllocation().width - scroll_button_size * .33f, notebook->GetAllocation().height - ( scroll_button_size + padding ) + .5f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						arrow_color
					)
				);
			}

			// Backward button
			if( notebook->GetFirstDisplayedTab() != 0 ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( 0.f, notebook->GetAllocation().height - ( scroll_button_size + padding ) ),
						sf::Vector2f( scroll_button_size, tab_size.y + 2.f * ( padding + border_width ) ),
						border_width,
						notebook->IsBackwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingBackward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( scroll_button_size * .66f, notebook->GetAllocation().height - ( scroll_button_size + padding ) + .66f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( scroll_button_size * .66f, notebook->GetAllocation().height - ( scroll_button_size + padding ) + .33f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						sf::Vector2f( scroll_button_size * .33f, notebook->GetAllocation().height - ( scroll_button_size + padding ) + .5f * ( tab_size.y + 2.f * ( padding + border_width ) ) ),
						arrow_color
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::TabPosition::LEFT ) {
		// Tabs are positioned at left.

		// Pane.
		queue->Add(
			Renderer::Get().CreatePane(
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
			Renderer::Get().CreateLine(
				sf::Vector2f( 0.f, notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f ),
				sf::Vector2f( tab_size.x + 2.f * padding + 3.f * border_width, notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( auto index = notebook->GetFirstDisplayedTab(); index < notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount(); ++index ) {
			auto label = notebook->GetNthTabLabel( index );
			auto label_allocation = label->GetAllocation();

			// Left border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top - border_width - padding ),
					sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top + label_allocation.height + padding ),
					border_color_light,
					border_width
				)
			);

			// Bottom border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top + label_allocation.height + padding ),
					sf::Vector2f( label_allocation.left + label_allocation.width + ( index == current_page ? border_width : 0.f ) + padding, label_allocation.top + label_allocation.height + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active top border
				queue->Add(
					Renderer::Get().CreateLine(
						sf::Vector2f( label_allocation.left - border_width - padding, label_allocation.top - border_width - padding ),
						sf::Vector2f( label_allocation.left + label_allocation.width + border_width + padding, label_allocation.top - border_width - padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding,
							label_allocation.top - padding,
							label_allocation.width + 2.f * ( border_width + padding ),
							label_allocation.height + 2.f * padding
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding,
							label_allocation.top - padding,
							label_allocation.width + 2.f * padding,
							label_allocation.height + 2.f * padding - border_width
						),
						( index == prelight_tab ) ? background_color_prelight : background_color_dark
					)
				);
			}
		}

		if( notebook->GetScrollable() ) {
			// Forward button
			if( ( notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount() ) < notebook->GetPageCount() ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( 0.f, notebook->GetAllocation().height - ( scroll_button_size ) ),
						sf::Vector2f( tab_size.x + 2.f * ( padding + border_width ), scroll_button_size ),
						border_width,
						notebook->IsForwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingForward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( ( tab_size.x + 2.f * ( padding + border_width ) ) * .6f, .33f * scroll_button_size + notebook->GetAllocation().height - scroll_button_size ),
						sf::Vector2f( ( tab_size.x + 2.f * ( padding + border_width ) ) * .4f, .33f * scroll_button_size + notebook->GetAllocation().height - scroll_button_size ),
						sf::Vector2f( ( tab_size.x + 2.f * ( padding + border_width ) ) * .5f, .66f * scroll_button_size + notebook->GetAllocation().height - scroll_button_size ),
						arrow_color
					)
				);
			}

			// Backward button
			if( notebook->GetFirstDisplayedTab() != 0 ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( 0.f, 0.f ),
						sf::Vector2f( tab_size.x + 2.f * ( padding + border_width ), scroll_button_size ),
						border_width,
						notebook->IsBackwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingBackward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( ( tab_size.x + 2.f * ( padding + border_width ) ) * .4f, .66f * scroll_button_size ),
						sf::Vector2f( ( tab_size.x + 2.f * ( padding + border_width ) ) * .6f, .66f * scroll_button_size ),
						sf::Vector2f( ( tab_size.x + 2.f * ( padding + border_width ) ) * .5f, .33f * scroll_button_size ),
						arrow_color
					)
				);
			}
		}
	}
	else if( notebook->GetTabPosition() == Notebook::TabPosition::RIGHT ) {
		// Tabs are positioned at right.

		// Pane.
		queue->Add(
			Renderer::Get().CreatePane(
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
			Renderer::Get().CreateLine(
				sf::Vector2f( child_size.x + 2.f * ( border_width + padding ), notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f ),
				sf::Vector2f( child_size.x + 4.f * padding + 3.f * border_width + tab_size.x, notebook->GetScrollable() && notebook->GetFirstDisplayedTab() != 0 ? scroll_button_size : 0.f ),
				border_color_light,
				border_width
			)
		);

		// Tab labels
		for( auto index = notebook->GetFirstDisplayedTab(); index < notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount(); ++index ) {
			auto label = notebook->GetNthTabLabel( index );
			auto label_allocation = label->GetAllocation();

			// Right border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left + label_allocation.width + border_width + padding, label_allocation.top - border_width - padding ),
					sf::Vector2f( label_allocation.left + label_allocation.width + border_width + padding, label_allocation.top + label_allocation.height + padding ),
					border_color_dark,
					border_width
				)
			);

			// Bottom border
			queue->Add(
				Renderer::Get().CreateLine(
					sf::Vector2f( label_allocation.left - padding, label_allocation.top + label_allocation.height + padding ),
					sf::Vector2f( label_allocation.left + label_allocation.width + border_width + padding, label_allocation.top + label_allocation.height + padding ),
					border_color_dark,
					border_width
				)
			);

			if( index == current_page ) {
				// Active top border
				queue->Add(
					Renderer::Get().CreateLine(
						sf::Vector2f( label_allocation.left - padding - border_width, label_allocation.top - border_width - padding ),
						sf::Vector2f( label_allocation.left + label_allocation.width + padding, label_allocation.top - border_width - padding ),
						border_color_light,
						border_width
					)
				);

				// Active background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding - border_width,
							label_allocation.top - padding,
							label_allocation.width + 2.f * padding + 2.f * border_width,
							label_allocation.height + 2.f * padding
						),
						background_color
					)
				);
			}
			else {
				// Inactive background
				queue->Add(
					Renderer::Get().CreateRect(
						sf::FloatRect(
							label_allocation.left - padding,
							label_allocation.top - padding,
							label_allocation.width + 2.f * padding + border_width,
							label_allocation.height + 2.f * padding
						),
						(index == prelight_tab) ? background_color_prelight : background_color_dark
					)
				);
			}
		}

		if( notebook->GetScrollable() ) {
			// Forward button
			if( ( notebook->GetFirstDisplayedTab() + notebook->GetDisplayedTabCount() ) < notebook->GetPageCount() ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * padding + border_width ), notebook->GetAllocation().height - ( scroll_button_size ) ),
						sf::Vector2f( tab_size.x + 2.f * ( padding + border_width ), scroll_button_size ),
						border_width,
						notebook->IsForwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingForward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) * .4f, .33f * scroll_button_size + notebook->GetAllocation().height - scroll_button_size ),
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) * .6f, .33f * scroll_button_size + notebook->GetAllocation().height - scroll_button_size ),
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) * .5f, .66f * scroll_button_size + notebook->GetAllocation().height - scroll_button_size ),
						arrow_color
					)
				);
			}

			// Backward button
			if( notebook->GetFirstDisplayedTab() != 0 ) {
				queue->Add(
					Renderer::Get().CreatePane(
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * padding + border_width ), 0.f ),
						sf::Vector2f( tab_size.x + 2.f * ( padding + border_width ), scroll_button_size ),
						border_width,
						notebook->IsBackwardScrollPrelight() ? scroll_button_prelight : background_color,
						border_color,
						notebook->IsScrollingBackward() ? -border_color_shift : border_color_shift
					)
				);

				queue->Add(
					Renderer::Get().CreateTriangle(
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) * .6f, .66f * scroll_button_size ),
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) * .4f, .66f * scroll_button_size ),
						sf::Vector2f( notebook->GetAllocation().width - ( tab_size.x + 2.f * ( padding + border_width ) ) * .5f, .33f * scroll_button_size ),
						arrow_color
					)
				);
			}
		}
	}

	return queue;
}

}
}
