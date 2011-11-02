#include <SFGUI/NonCopyable.hpp>

namespace sfg {

NonCopyable::NonCopyable() {
}

NonCopyable::~NonCopyable() {
}

NonCopyable::NonCopyable( const NonCopyable& ) {
}

const NonCopyable& NonCopyable::operator=( const NonCopyable& ) {
	return *this;
}

}
