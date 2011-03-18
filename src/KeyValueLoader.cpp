#include <SFGUI/KeyValueLoader.hpp>

namespace sfg {

KeyValueLoader::KeyValueLoader() {
}

KeyValueLoader::~KeyValueLoader() {
}

void KeyValueLoader::Visit( Observer& observer ) const {
	KeyValueMap::const_iterator citer( m_values.begin() );
	KeyValueMap::const_iterator citerend( m_values.end() );

	for( ; citer != citerend; ++citer ) {
		observer( citer->first, citer->second );
	}
}

void KeyValueLoader::AddValue( const std::string& key, const std::string& value ) {
	m_values.insert( std::pair<const std::string, const std::string>( key, value ) );
}

bool KeyValueLoader::LoadFromFile( const std::string& filename ) {
	m_values.clear();
	return LoadImpl( filename );
}

}
