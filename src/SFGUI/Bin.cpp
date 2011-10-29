#include <SFGUI/Bin.hpp>

namespace sfg {

Bin::Bin() :
	Container()
{
	OnAdd.Connect( &Bin::HandleAdd, this );
}

Widget::Ptr Bin::GetChild() const {
	if( GetChildren().size() < 1 ) {
		return Widget::Ptr();
	}

	return *GetChildren().begin();
}

void Bin::HandleAdd() {
	// TODO: Virtual func.
	/*if( GetChildren().size() > 1 ) {

#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Only one widget can be added to a Bin." << std::endl;
#endif

		Remove( child );
		return;
	}*/

	// Make sure the Bin is large enough.
	RequestSize();
}

}
