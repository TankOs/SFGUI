#include <SFGUI/Bin.hpp>

namespace sfg {

Bin::Bin() :
	Container()
{
}

Widget::Ptr Bin::GetChild() const {
	if( GetChildren().size() < 1 ) {
		return Widget::Ptr();
	}

	return *GetChildren().begin();
}

bool Bin::HandleAdd( Widget::Ptr /*child*/ ) {
	if( GetChildren().size() > 0 ) {
#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Only one widget can be added to a Bin." << std::endl;
#endif

		return false;
	}

	return true;
}

}
