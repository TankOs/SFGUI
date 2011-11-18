#include <SFGUI/Engine.hpp>
#include <SFGUI/Parsers/ThemeParser/Parse.hpp>

#include <SFML/Graphics/Text.hpp>
#include <sstream>

namespace sf {
std::ostream& operator<<( std::ostream& stream, const Color& color ) {
	stream
		<< std::setfill( '0' )
		<< "#"
		<< std::hex
		<< std::setw( 2 ) << static_cast<int>( color.r )
		<< std::setw( 2 ) << static_cast<int>( color.g )
		<< std::setw( 2 ) << static_cast<int>( color.b )
		<< std::setw( 2 ) << static_cast<int>( color.a )
		<< std::dec
	;
	return stream;
}

std::istream& operator>>( std::istream& stream, Color& color ) {
	std::string buffer;
	stream >> buffer;

	// Check length and leading '#'.
	if( buffer.size() != 9 || buffer[0] != '#' ) {
		stream.setstate( std::ios::failbit );
		return stream;
	}

	std::stringstream r( buffer.substr( 1, 2 ) );
	std::stringstream g( buffer.substr( 3, 2 ) );
	std::stringstream b( buffer.substr( 5, 2 ) );
	std::stringstream a( buffer.substr( 7, 2 ) );
	int r_val( 0 );
	int g_val( 0 );
	int b_val( 0 );
	int a_val( 0 );

	r >> std::hex >> r_val;
	g >> std::hex >> g_val;
	b >> std::hex >> b_val;
	a >> std::hex >> a_val;

	color.r = static_cast<sf::Uint8>( r_val );
	color.g = static_cast<sf::Uint8>( g_val );
	color.b = static_cast<sf::Uint8>( b_val );
	color.a = static_cast<sf::Uint8>( a_val );

	return stream;
}
}


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

bool Engine::LoadThemeFromFile( const std::string& filename ) {
	parser::theme::Theme theme = parser::theme::ParseFile( filename );

	if( theme.empty() ) {
		return false;
	}

	std::size_t num_rules = theme.size();

	// Iterate over all rules
	for( std::size_t rule_index = 0; rule_index < num_rules; ++rule_index ) {
		std::size_t num_simple_selectors = theme[ rule_index ].m_selector.m_simple_selectors.size();

		Selector::Ptr selector;

		// Iterate over all simple selectors
		for( std::size_t simple_selector_index = 0; simple_selector_index < num_simple_selectors; ++simple_selector_index ) {
			int hierarchy = Selector::Root;

			if( theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_combinator == ">" ) {
				hierarchy = Selector::Child;
			}
			else if( theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_combinator == " " ) {
				hierarchy = Selector::Descendant;
			}
			else if( theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_combinator == "," ) {
				// Grouping combinator detected. Stop eating simple selectors and set the properties now.
				std::size_t num_declarations = theme[ rule_index ].m_declarations.size();

				// Iterate over all declarations
				for( std::size_t declaration_index = 0; declaration_index < num_declarations; ++declaration_index ) {
					std::string property_name = theme[ rule_index ].m_declarations[ declaration_index ].m_property_name;
					std::string property_value = theme[ rule_index ].m_declarations[ declaration_index ].m_property_value;

					// Finally set the property
					SetProperty( selector, property_name, property_value );
				}

				// Reset the current simple selector to be the root of a new chain.
				selector = Selector::Ptr();
			}

			selector = Selector::Create(
				theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_type_selector,
				theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_id_selector,
				theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_class_selector,
				theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_state_selector,
				hierarchy,
				selector
			);
		}

		std::size_t num_declarations = theme[ rule_index ].m_declarations.size();

		// Iterate over all declarations
		for( std::size_t declaration_index = 0; declaration_index < num_declarations; ++declaration_index ) {
			std::string property_name = theme[ rule_index ].m_declarations[ declaration_index ].m_property_name;
			std::string property_value = theme[ rule_index ].m_declarations[ declaration_index ].m_property_value;

			// Finally set the property
			SetProperty( selector, property_name, property_value );
		}
	}

	return true;
}

void Engine::ShiftBorderColors( sf::Color& light_color, sf::Color& dark_color, int offset ) const {
	// TODO: Replace by += and -=. Currently not possible with SFML (see SFML issue #114).
	light_color.r = static_cast<sf::Uint8>( std::min( 255, static_cast<int>( light_color.r ) + offset ) );
	light_color.g = static_cast<sf::Uint8>( std::min( 255, static_cast<int>( light_color.g ) + offset ) );
	light_color.b = static_cast<sf::Uint8>( std::min( 255, static_cast<int>( light_color.b ) + offset ) );

	dark_color.r = static_cast<sf::Uint8>( std::max( 0, static_cast<int>( dark_color.r ) - offset ) );
	dark_color.g = static_cast<sf::Uint8>( std::max( 0, static_cast<int>( dark_color.g ) - offset ) );
	dark_color.b = static_cast<sf::Uint8>( std::max( 0, static_cast<int>( dark_color.b ) - offset ) );
}

const std::string* Engine::GetValue( const std::string& property, std::shared_ptr<const Widget> widget ) const {
	// Look for property.
	PropertyMap::const_iterator prop_iter( m_properties.find( property ) );

	if( prop_iter != m_properties.end() ) {
		// Find widget-specific properties, first.
		WidgetNameMap::const_iterator name_iter( prop_iter->second.find( widget->GetName() ) );

		if( name_iter != prop_iter->second.end() ) {
			// Check against selectors.
			SelectorValueList::const_iterator sv_iter( name_iter->second.begin() );
			SelectorValueList::const_iterator sv_iter_end( name_iter->second.end() );

			for( ; sv_iter != sv_iter_end; ++sv_iter ) {
				if( sv_iter->first->Matches( widget ) ) {
					// Found, return value.
					return &sv_iter->second;
				}
			}
		}

		// Look for general properties now.
		name_iter = prop_iter->second.find( "*" );

		if( name_iter != prop_iter->second.end() ) {
			SelectorValueList::const_iterator sv_iter( name_iter->second.begin() );
			SelectorValueList::const_iterator sv_iter_end( name_iter->second.end() );

			for( ; sv_iter != sv_iter_end; ++sv_iter ) {
				if( sv_iter->first->Matches( widget ) ) {
					// Found, return value.
					return &sv_iter->second;
				}
			}
		}
	}

	return 0;
}

ResourceManager& Engine::GetResourceManager() const {
	return m_resource_manager;
}

bool Engine::SetProperty( sfg::Selector::Ptr selector, const std::string& property, const std::string& value ) {
	if( !selector ) {
		// Invalid selector string given.
		return false;
	}

	// If the selector does already exist, we'll remove it to make sure the newly
	// added value will get a higher priority than the previous one, because
	// that's the expected behaviour (LIFO).
	SelectorValueList& list( m_properties[property][selector->GetWidgetName()] ); // Shortcut.
	SelectorValueList::iterator list_begin( list.begin() );
	SelectorValueList::iterator list_end( list.end() );

	for( ; list_begin != list_end; ++list_begin ) {
		if( *list_begin->first == *selector ) {
			// Equal, remove.
			list.erase( list_begin );
			break;
		}
	}

	// Insert at top to get highest priority.
	list.push_front( SelectorValuePair( selector, value ) );

	return true;
}

}
