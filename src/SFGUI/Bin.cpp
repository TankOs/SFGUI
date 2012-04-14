#include <SFGUI/Bin.hpp>

namespace sfg {

Bin::Bin() :
	Container()
{
}

Bin::~Bin() {
}

Widget::Ptr Bin::GetChild() const {
	if( GetChildren().size() < 1 ) {
		return Widget::Ptr();
	}

	return *GetChildren().begin();
}

void Bin::HandleAdd( const Widget::Ptr& child ) {
	Container::HandleAdd( child );

	if( GetChildren().size() > 1 ) {
#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Only one widget can be added to a Bin.\n";
#endif

		Remove( child );
	}
}

}
