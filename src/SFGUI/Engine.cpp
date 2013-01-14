#include <SFGUI/Engine.hpp>
#include <SFGUI/Parsers/ThemeParser/Parse.hpp>

#include <SFML/Graphics/Text.hpp>
#include <fstream>
#include <sstream>
#include <cmath>

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

Engine::Engine() :
	m_auto_refresh( false )
{
}

Engine::~Engine() {
}

sf::Vector2f Engine::GetFontHeightProperties( const sf::Font& font, unsigned int font_size ) const {
	// We want to cache line height values because they are expensive to compute.

	static std::map<std::pair<void*, unsigned int>, sf::Vector2f> height_property_cache;

	// Get the font face that Laurent tries to hide from us.
	struct FontStruct {
		void* font_face; // Authentic SFML comment: implementation details
		void* unused1;
		int* unused2;

		// Since maps allocate everything non-contiguously on the heap we can use void* instead of Page here.
		mutable std::map<unsigned int, void*> unused3;
		mutable std::vector<sf::Uint8> unused4;
	};

	void* face;

	// All your font face are belong to us too.
	memcpy( &face, reinterpret_cast<const char*>( &font ) + sizeof( sf::Font ) - sizeof( FontStruct ), sizeof( void* ) );

	std::pair<void*, unsigned int> id( face, font_size );

	std::map<std::pair<void*, unsigned int>, sf::Vector2f>::iterator iter( height_property_cache.find( id ) );

	if( iter != height_property_cache.end() ) {
		return iter->second;
	}

	sf::Vector2f properties( 0.f, 0.f );

	for( sf::Uint32 current_character = 0; current_character < 0x0370; ++current_character ) {
		const sf::Glyph& glyph = font.getGlyph( current_character, font_size, false );
		properties.x = std::max( properties.x, static_cast<float>( glyph.bounds.height ) );
		properties.y = std::max( properties.y, static_cast<float>( -glyph.bounds.top ) );
	}

	height_property_cache[id] = properties;

	return properties;
}

float Engine::GetFontLineHeight( const sf::Font& font, unsigned int font_size ) const {
	return GetFontHeightProperties( font, font_size ).x;
}

float Engine::GetFontBaselineOffset( const sf::Font& font, unsigned int font_size ) const {
	return GetFontHeightProperties( font, font_size ).y;
}

float Engine::GetFontLineSpacing( const sf::Font& font, unsigned int font_size ) const {
	return static_cast<float>( font.getLineSpacing( font_size ) );
}

sf::Vector2f Engine::GetTextMetrics( const std::basic_string<sf::Uint32>& string, const sf::Font& font, unsigned int font_size ) const {
	// SFML is incapable of giving us the metrics we need so we have to do it ourselves.
	float horizontal_spacing = static_cast<float>( font.getGlyph( L' ', font_size, false ).advance );
	float vertical_spacing = static_cast<float>( font.getLineSpacing( font_size ) );

	sf::Vector2f metrics( 0.f, 0.f );

	const static float tab_spaces = 2.f;

	sf::Uint32 previous_character = 0;

	std::size_t length = string.size();

	float longest_line = 0.f;

	for( std::size_t index = 0; index < length; ++index ) {
		sf::Uint32 current_character = string[index];

		metrics.x += static_cast<float>( font.getKerning( previous_character, current_character, font_size ) );

		switch( current_character ) {
			case L' ':
				metrics.x += horizontal_spacing;
				continue;
			case L'\t':
				metrics.x += horizontal_spacing * tab_spaces;
				continue;
			case L'\n':
				metrics.y += vertical_spacing;
				longest_line = std::max( metrics.x, longest_line );
				metrics.x = 0.f;
				continue;
			case L'\v':
				metrics.y += vertical_spacing * tab_spaces;
				continue;
			default:
				break;
		}

		const sf::Glyph& glyph = font.getGlyph( current_character, font_size, false );

		metrics.x += static_cast<float>( glyph.advance );
		metrics.y = std::max( metrics.y, static_cast<float>( glyph.bounds.height ) );
	}

	metrics.x = std::max( longest_line, metrics.x );

	return metrics;
}

sf::Vector2f Engine::GetTextMetrics( const sf::String& string, const sf::Font& font, unsigned int font_size ) const {
	// SFML is incapable of giving us the metrics we need so we have to do it ourselves.
	float horizontal_spacing = static_cast<float>( font.getGlyph( L' ', font_size, false ).advance );
	float vertical_spacing = static_cast<float>( font.getLineSpacing( font_size ) );

	sf::Vector2f metrics( 0.f, 0.f );

	const static float tab_spaces = 2.f;

	sf::Uint32 previous_character = 0;

	std::size_t length = string.getSize();

	float longest_line = 0.f;

	for( std::size_t index = 0; index < length; ++index ) {
		sf::Uint32 current_character = string[index];

		metrics.x += static_cast<float>( font.getKerning( previous_character, current_character, font_size ) );

		switch( current_character ) {
			case L' ':
				metrics.x += horizontal_spacing;
				continue;
			case L'\t':
				metrics.x += horizontal_spacing * tab_spaces;
				continue;
			case L'\n':
				metrics.y += vertical_spacing;
				longest_line = std::max( metrics.x, longest_line );
				metrics.x = 0.f;
				continue;
			case L'\v':
				metrics.y += vertical_spacing * tab_spaces;
				continue;
			default:
				break;
		}

		const sf::Glyph& glyph = font.getGlyph( current_character, font_size, false );

		metrics.x += static_cast<float>( glyph.advance );
		metrics.y = std::max( metrics.y, static_cast<float>( glyph.bounds.height ) );
	}

	metrics.x = std::max( longest_line, metrics.x );

	return metrics;
}

bool Engine::LoadThemeFromString( const std::string& data ) {
	parser::theme::Theme theme = parser::theme::ParseString( data );

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
			int hierarchy = Selector::ROOT;

			if( theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_combinator == ">" ) {
				hierarchy = Selector::CHILD;
			}
			else if( theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_combinator == " " ) {
				hierarchy = Selector::DESCENDANT;
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

bool Engine::LoadThemeFromFile( const std::string& filename ) {
	std::ifstream in( filename.c_str() );

	if( !in.good() ) {
		return false;
	}

	std::string data(
		(std::istreambuf_iterator<char>( in )),
		(std::istreambuf_iterator<char>())
	);

	return LoadThemeFromString( data );
}

void Engine::ShiftBorderColors( sf::Color& light_color, sf::Color& dark_color, int offset ) const {
	// TODO: Replace by += and -=. Currently not possible with SFML (see SFML issue #114).
	light_color.r = static_cast<sf::Uint8>( std::max( 0, std::min( 255, static_cast<int>( light_color.r ) + offset ) ) );
	light_color.g = static_cast<sf::Uint8>( std::max( 0, std::min( 255, static_cast<int>( light_color.g ) + offset ) ) );
	light_color.b = static_cast<sf::Uint8>( std::max( 0, std::min( 255, static_cast<int>( light_color.b ) + offset ) ) );

	dark_color.r = static_cast<sf::Uint8>( std::min( 255, std::max( 0, static_cast<int>( dark_color.r ) - offset ) ) );
	dark_color.g = static_cast<sf::Uint8>( std::min( 255, std::max( 0, static_cast<int>( dark_color.g ) - offset ) ) );
	dark_color.b = static_cast<sf::Uint8>( std::min( 255, std::max( 0, static_cast<int>( dark_color.b ) - offset ) ) );
}

const std::string* Engine::GetValue( const std::string& property, const Widget::PtrConst& widget ) const {
	// Look for property.
	PropertyMap::const_iterator prop_iter( m_properties.find( property ) );

	const std::string* value = 0;
	int score = -1;

	if( prop_iter != m_properties.end() ) {
		WidgetNameMap::const_iterator name_iter;

		if( widget ) {
			// Find widget-specific properties, first.
			name_iter = prop_iter->second.find( widget->GetName() );

			if( name_iter != prop_iter->second.end() ) {
				// Check against selectors.
				std::size_t selector_value_list_size = name_iter->second.size();

				for( std::size_t index = 0; index < selector_value_list_size; ++index ) {
					if( name_iter->second[index].first->Matches( widget ) ) {
						// Found, check if it is better than current best.
						int new_score = name_iter->second[index].first->GetScore();

						if( new_score > score ) {
							value = &name_iter->second[index].second;
							score = new_score;
						}
					}
				}
			}
		}

		// Look for general properties now.
		name_iter = prop_iter->second.find( "*" );

		if( name_iter != prop_iter->second.end() ) {
			std::size_t selector_value_list_size = name_iter->second.size();

			for( std::size_t index = 0; index < selector_value_list_size; ++index ) {
				if( name_iter->second[index].first->Matches( widget ) ) {
					// Found, check if it is better than current best.
					int new_score = name_iter->second[index].first->GetScore();

					if( new_score > score ) {
						value = &name_iter->second[index].second;
						score = new_score;
					}
				}
			}
		}
	}

	return value;
}

ResourceManager& Engine::GetResourceManager() const {
	return m_resource_manager;
}

bool Engine::SetProperty( const sfg::Selector::Ptr& selector, const std::string& property, const std::string& value ) {
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
	list.insert( list.begin(), SelectorValuePair( selector, value ) );

	if( m_auto_refresh ) {
		Widget::RefreshAll();
	}

	return true;
}

bool Engine::SetProperties( const std::string& properties ) {
	parser::theme::Theme theme = parser::theme::ParseString( properties );

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
			int hierarchy = Selector::ROOT;

			if( theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_combinator == ">" ) {
				hierarchy = Selector::CHILD;
			}
			else if( theme[ rule_index ].m_selector.m_simple_selectors[ simple_selector_index ].m_combinator == " " ) {
				hierarchy = Selector::DESCENDANT;
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

	if( m_auto_refresh ) {
		Widget::RefreshAll();
	}

	return true;
}

void Engine::ClearProperties() {
	m_properties.clear();
}

void Engine::SetAutoRefresh( bool enable ) {
	m_auto_refresh = enable;
}

}
