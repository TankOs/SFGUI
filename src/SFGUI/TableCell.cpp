#include <SFGUI/TableCell.hpp>
#include <SFGUI/Widget.hpp>

namespace sfg {
namespace priv {

TableCell::TableCell( Widget::Ptr child_, const sf::Rect<sf::Uint32>& rect_, int x_options_, int y_options_, const sf::Vector2f& padding_ ) :
	child( child_ ),
	rect( rect_ ),
	x_options( x_options_ ),
	y_options( y_options_ ),
	padding( padding_ )
{
}

}
}
