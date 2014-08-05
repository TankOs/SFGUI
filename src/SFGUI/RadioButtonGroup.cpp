#include <SFGUI/RadioButtonGroup.hpp>

namespace sfg {

RadioButtonGroup::Ptr RadioButtonGroup::Create() {
	return Ptr( new RadioButtonGroup );
}

RadioButtonGroup::ContainerType& RadioButtonGroup::GetMembers() {
	return m_members;
}

}
