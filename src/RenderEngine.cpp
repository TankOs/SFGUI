#include <SFGUI/RenderEngine.hpp>

namespace sfg {

void RenderEngine::SetProperty( const std::string& property, const std::string& value ) {
	m_props[property] = value;
}

const std::string& RenderEngine::GetProperty( const std::string& property, const std::string& default_ ) const {
	PropertiesMap::const_iterator  iter( m_props.find( property ) );

	return iter != m_props.end() ? iter->second : default_;
}


}
