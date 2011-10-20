#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Text.hpp>
#include <sstream>

namespace sfg {

Engine::~Engine() {
}

float Engine::GetLineHeight( const sf::Font& font, unsigned int font_size ) const {
	return static_cast<float>( font.GetLineSpacing( font_size ) ) + std::floor( static_cast<float>( font_size ) / 20.f ); // Last part from experimentation
}

sf::Vector2f Engine::GetTextMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size ) const {
	sf::Text  text( string, font, font_size );
	return sf::Vector2f( text.GetRect().Width, text.GetRect().Height );
}

const sf::Font& Engine::LoadFontFromFile( const std::string& filename ) const {
	if( filename.empty() ) {
		return sf::Font::GetDefaultFont();
	}

	FontMap::const_iterator  iter( m_fonts.find( filename ) );

	if( iter != m_fonts.end() ) {
		return iter->second;
	}

	sf::Font  font;
	if( !font.LoadFromFile( filename ) ) {
		return sf::Font::GetDefaultFont();
	}

	m_fonts[filename] = font;
	return m_fonts[filename];
}

void Engine::RegisterProperty( const std::string& name, PropertyType type ) {
	m_types[name] = type;
}

Engine::PropertyType Engine::GetPropertyType( const std::string& name ) const {
	PropertyTypeMap::const_iterator iter( m_types.find( name ) );

	if( iter == m_types.end() ) {
		return Undefined;
	}

	return iter->second;
}

bool Engine::SetPropertyImpl( const std::string& selector, const std::string& property, const boost::any& value ) {
	// Make sure property has been registered before.
	PropertyTypeMap::const_iterator type_iter( m_types.find( property ) );

	if( type_iter == m_types.end() ) {
		return false;
	}

	// Check for correct type.
	try {
		switch( type_iter->second ) {
			case Integer:
				boost::any_cast<int>( value );
				break;

			case UnsignedInteger:
				boost::any_cast<unsigned int>( value );
				break;

			case Float:
				boost::any_cast<float>( value );
				break;

			case String:
				boost::any_cast<const std::string>( value );
				break;

			case Color:
				boost::any_cast<const sf::Color>( value );
				break;

			default:
				return false;
		}
	}
	catch( const boost::bad_any_cast& e ) {
		// Type conversion failed.
		return false;
	}

	// Create selector.
	sfg::Selector::Ptr selector_object( sfg::Selector::Create( selector ) );

	if( !selector_object ) {
		// Invalid selector string given.
		return false;
	}

	// If the selector does already exist, we'll remove it to make sure the newly
	// added value will get a higher priority than the previous one, because
	// that's the expected behaviour (LIFO).
	SelectorValueList& list( m_properties[property][selector_object->GetWidgetName()] ); // Shortcut.
	SelectorValueList::iterator list_begin( list.begin() );
	SelectorValueList::iterator list_end( list.end() );

	for( ; list_begin != list_end; ++list_begin ) {
		if( *list_begin->first == *selector_object ) {
			// Equal, remove.
			list.erase( list_begin );
			break;
		}
	}

	// Insert at top to get highest priority.
	list.push_front( SelectorValuePair( selector_object, value ) );

	return true;
}

}
