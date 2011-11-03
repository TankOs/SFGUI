#include <SFGUI/Selector.hpp>
#include <SFGUI/Container.hpp>

namespace sfg {

Selector::Selector() :
	m_state( -1 ),
	m_hash( 0 )
{
}

Selector::Selector( const Selector& other ) :
	m_widget( other.m_widget ),
	m_id( other.m_id ),
	m_class( other.m_class ),
	m_state( other.m_state ),
	m_hash( 0 )
{
	if( other.m_parent ) {
		m_parent.reset( new Selector( *other.m_parent ) );
	}
}

Selector& Selector::operator=( const Selector& other ) {
	m_widget = other.m_widget;
	m_id = other.m_id;
	m_class = other.m_class;
	m_state = other.m_state;
	m_hash = other.m_hash;

	if( other.m_parent ) {
		m_parent.reset( new Selector( *other.m_parent ) );
	}
	else {
		m_parent.reset();
	}

	return *this;
}

Selector::Ptr Selector::Create( const std::string& str ) {
	std::string::const_iterator str_iter( str.begin() );
	bool parse_next( true );
	Ptr selector;

	while( parse_next ) {
		// Check bounds.
		if( str_iter == str.end() ) {
			break;
		}

		// If it's not the first selector part, check for next delimiter.
		if( selector ) {
			if( *str_iter != '>' ) {
				return Ptr();
			}

			// Skip delimiter.
			++str_iter;

			// Check bounds.
			if( str_iter == str.end() ) {
				return Ptr();
			}
		}

		// Eat any leading whitespace.
		EatWhitespace( str_iter, str.end() );

		Ptr next( new Selector );

		try {
			next->m_widget = ParseWidget( str_iter, str.end() );
			next->m_id = ParseId( str_iter, str.end() );
			next->m_class = ParseClass( str_iter, str.end() );
			next->m_state = ParseState( str_iter, str.end() );
		}
		catch( const ParserException& /*e*/ ) {
			return Ptr();
		}

		// If it's the first selector part, set it as current root.
		if( !selector ) {
			selector = next;
		}
		else {
			// If it's not the first, parent the current root.
			next->m_parent = selector;
			selector = next;
		}
	}

	// If there's no selector yet, it means we want to reach ALL widgets, which
	// is perfectly fine.
	if( !selector ) {
		selector = Ptr( new Selector );
	}

	// Hash.
	std::hash<std::string> string_hasher;
	selector->m_hash = string_hasher( selector->BuildString() );

	return selector;
}

std::string Selector::ParseWidget( std::string::const_iterator& begin, const std::string::const_iterator& end ) throw() {
	std::string token;

	for( ; begin != end; ++begin ) {
		// Check for ID, class and state delimiters.
		if( *begin == '#' || *begin == '.' || *begin == ':' || *begin == '>' ) {
			return token;
		}

		// Check for whitespace.
		if( *begin == ' ' ) {
			EatWhitespace( begin, end );
			return token;
		}

		// Check for valid char.
		if(
			(*begin < 'a' || *begin > 'z') &&
			(*begin < 'A' || *begin > 'Z')
		) {
			throw( ParserException( std::string( "Invalid character for widget name: " ) + *begin ) );
		}

		token += *begin;
	}

	return token;
}

std::string Selector::ParseId( std::string::const_iterator& begin, const std::string::const_iterator& end ) throw() {
	std::string token;

	// Check bounds.
	if( begin == end ) {
		return "";
	}

	// Check for delimiter.
	if( *begin != '#' ) {
		return "";
	}

	// Skip delimiter.
	++begin;

	for( ; begin != end; ++begin ) {
		// Check for class and state delimiters.
		if( *begin == '.' || *begin == ':' || *begin == '>' ) {
			return token;
		}

		// Check for whitespace.
		if( *begin == ' ' ) {
			EatWhitespace( begin, end );
			return token;
		}

		// Check for valid char.
		if(
			(*begin < 'a' || *begin > 'z') &&
			(*begin < 'A' || *begin > 'Z') &&
			(*begin < '0' || *begin > '9') &&
			(*begin != '_') &&
			(*begin != '-')
		) {
			throw( ParserException( std::string( "Invalid character for ID: " ) + *begin ) );
		}

		token += *begin;
	}

	return token;
}

std::string Selector::ParseClass( std::string::const_iterator& begin, const std::string::const_iterator& end ) throw() {
	std::string token;

	// Check bounds.
	if( begin == end ) {
		return "";
	}

	// Check for delimiter.
	if( *begin != '.' ) {
		return "";
	}

	// Skip delimiter.
	++begin;

	for( ; begin != end; ++begin ) {
		// Check for state delimiters.
		if( *begin == ':' || *begin == '>' ) {
			return token;
		}

		// Check for whitespace.
		if( *begin == ' ' ) {
			EatWhitespace( begin, end );
			return token;
		}

		// Check for valid char.
		if(
			(*begin < 'a' || *begin > 'z') &&
			(*begin < 'A' || *begin > 'Z') &&
			(*begin < '0' || *begin > '9') &&
			(*begin != '_') &&
			(*begin != '-')
		) {
			throw( ParserException( std::string( "Invalid character for class: " ) + *begin ) );
		}

		token += *begin;
	}

	return token;
}

int Selector::ParseState( std::string::const_iterator& begin, const std::string::const_iterator& end ) throw() {
	std::string token;

	// Check bounds.
	if( begin == end ) {
		return -1;
	}

	// Check for delimiter.
	if( *begin != ':' ) {
		return -1;
	}

	// Skip delimiter.
	++begin;

	for( ; begin != end; ++begin ) {
		// If there's whitespace, the state is complete. Check for delimiter or end of string.
		if( *begin == ' ' ) {
			EatWhitespace( begin, end );

			if( begin == end || *begin == '>' ) {
				break;
			}

			// Another stuff follows after whitespace, this shouldn't happen.
			throw( ParserException( "Expected '>' or end of string for state." ) );
		}

		// Check for valid char.
		if(
			(*begin < 'a' || *begin > 'z') &&
			(*begin < 'A' || *begin > 'Z')
		) {
			throw( ParserException( std::string( "Invalid character for state: " ) + *begin ) );
		}

		token += *begin;
	}

	// Check state string.
	if( token == "Normal" ) {
		return Widget::Normal;
	}
	else if( token == "Active" ) {
		return Widget::Active;
	}
	else if( token == "Prelight" ) {
		return Widget::Prelight;
	}
	else if( token == "Selected" ) {
		return Widget::Selected;
	}
	else if( token == "Insensitive" ) {
		return Widget::Insensitive;
	}

	throw( ParserException( "Invalid state: " + token ) );
}

void Selector::EatWhitespace( std::string::const_iterator& begin, const std::string::const_iterator& end ) {
	for( ; begin != end; ++begin ) {
		if( *begin != ' ' ) {
			return;
		}
	}
}

const std::string& Selector::GetWidgetName() const {
	return m_widget;
}

const std::string& Selector::GetId() const {
	return m_id;
}

const std::string& Selector::GetClass() const {
	return m_class;
}

int Selector::GetState() const {
	return m_state;
}

Selector::PtrConst Selector::GetParent() const {
	return m_parent;
}

std::string Selector::BuildString() const {
	std::string str;

	// Build parent's string first.
	if( m_parent ) {
		str += m_parent->BuildString();
		str += ">";
	}

	// Append own string.
	if( m_widget.empty() ) {
		// Use a wildcard for all widgets.
		str += "*";
	}
	else {
		str += m_widget;
	}

	if( !m_id.empty() ) {
		str += "#";
		str += m_id;
	}

	if( !m_class.empty() ) {
		str += ".";
		str += m_class;
	}

	if( m_state > -1 ) {
		str += ":";

		switch( m_state ) {
			case Widget::Normal:
				str += "Normal";
				break;
			case Widget::Prelight:
				str += "Prelight";
				break;
			case Widget::Active:
				str += "Active";
				break;
			case Widget::Selected:
				str += "Selected";
				break;
			case Widget::Insensitive:
				str += "Insensitive";
				break;

			default:
				str += "UNKNOWN";
				break;
		}
	}

	return str;
}

bool Selector::operator==( const Selector& other ) {
	// Check if valid selectors.
	if( !m_hash || !other.m_hash ) {
		return false;
	}

	return m_hash == other.m_hash;
}

bool Selector::Matches( Widget::PtrConst widget ) const {
	// If selector has a parent but widget doesn't, we don't need to go on any
	// further.
	if( m_parent && !widget->GetParent() ) {
		return false;
	}

	// Check attributes.
	if( !m_widget.empty() && m_widget != widget->GetName() ) {
		return false;
	}

	if( !m_id.empty() && widget->GetId() != m_id ) {
		return false;
	}

	if( !m_class.empty() && widget->GetClass() != m_class ) {
		return false;
	}

	if( m_state > -1 && widget->GetState() != m_state ) {
		return false;
	}

	// Check parent, if needed.
	if( m_parent ) {
		return m_parent->Matches( widget->GetParent() );
	}

	// No parent, success.
	return true;
}

}
