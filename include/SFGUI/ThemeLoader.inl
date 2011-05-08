#include <sstream>

namespace sfg {

template <class Loader>
bool ThemeLoader::LoadFromFile( const std::string& filename, Engine& engine ) {
	Loader loader;

	if( !loader.LoadFromFile( filename ) ) {
		return false;
	}

	Observer observer( engine );
	loader.Visit( observer );

	return true;
}

}
