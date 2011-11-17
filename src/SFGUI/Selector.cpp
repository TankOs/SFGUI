#include <SFGUI/Selector.hpp>
#include <SFGUI/Container.hpp>

namespace sfg {

Selector::Selector() :
	m_hierarchy_type( Invalid ),
	m_state( -1 ),
	m_hash( 0 )
{
}

Selector::Selector( const Selector& other ) :
	m_hierarchy_type( other.m_hierarchy_type ),
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
	m_hierarchy_type = other.m_hierarchy_type;
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

	int hierarchy_type = Root;

	while( parse_next ) {
		// Eat any whitespace before the current simple selector.
		EatWhitespace( str_iter, str.end() );

		// Check bounds.
		if( str_iter == str.end() ) {
			break;
		}

		// If it's not the first simple selector, check for combinator.
		if( selector ) {
			if( *str_iter == '>' ) {
				hierarchy_type = Child;

				// Skip combinator.
				++str_iter;

				// Eat any whitespace after the '>'.
				EatWhitespace( str_iter, str.end() );
			}
			else {
				hierarchy_type = Descendant;
			}

			// Check bounds.
			if( str_iter == str.end() ) {
				return Ptr();
			}
		}

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

		next->m_hierarchy_type = hierarchy_type;

		// If it's the first simple selector, set it as current root.
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
	// binary1248 says: Actually, we want this case to be handled by the wildcard
	// which means if there is no selector, no widget (wildcard) was parsed
	// and the input is invalid.
	if( !selector ) {
		//selector = Ptr( new Selector );
		return Ptr();
	}

	// Hash.
	std::hash<std::string> string_hasher;
	selector->m_hash = string_hasher( selector->BuildString() );

	return selector;
}

Selector::Ptr Selector::Create( const std::string& widget, const std::string& id, const std::string& class_, const std::string& state, int hierarchy, Ptr parent ) {
	Ptr selector( new Selector );

	selector->m_widget = widget;
	selector->m_id = id;
	selector->m_class = class_;

	if( state == "Normal" ) {
		selector->m_state = Widget::Normal;
	}
	else if( state == "Active" ) {
		selector->m_state = Widget::Active;
	}
	else if( state == "Prelight" ) {
		selector->m_state = Widget::Prelight;
	}
	else if( state == "Selected" ) {
		selector->m_state = Widget::Selected;
	}
	else if( state == "Insensitive" ) {
		selector->m_state = Widget::Insensitive;
	}
	else if( !state.empty() ) {
		throw( ParserException( "Invalid state: " + state ) );
	}

	selector->m_hierarchy_type = hierarchy;

	if( hierarchy != Root ) {
		selector->m_parent = parent;
	}

	// Hash.
	std::hash<std::string> string_hasher;
	selector->m_hash = string_hasher( selector->BuildString() );

	return selector;
}

std::string Selector::ParseWidget( std::string::const_iterator& begin, const std::string::const_iterator& end ) {
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
			(*begin < 'A' || *begin > 'Z') &&
			(*begin != '*' )
			// TODO: Handle the cases where letters and asterisks (or multiple asterisks) get mixed together.
		) {
			throw( ParserException( std::string( "Invalid character for widget name: " ) + *begin ) );
		}

		token += *begin;
	}

	return token;
}

std::string Selector::ParseId( std::string::const_iterator& begin, const std::string::const_iterator& end ) {
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

std::string Selector::ParseClass( std::string::const_iterator& begin, const std::string::const_iterator& end ) {
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

int Selector::ParseState( std::string::const_iterator& begin, const std::string::const_iterator& end ) {
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

		switch( m_hierarchy_type ) {
			case Child: {
				str += ">";
			} break;
			case Descendant: {
				str += " ";
			} break;
			default: break;
		}
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
	if( !widget ) {
		return false;
	}

	// Recursion is your friend ;)

	// Check if current stage is a pass...
	if( ( !m_widget.compare("*") && m_id.empty() && m_class.empty() && m_state == -1 ) | // Wildcard
		  ( ( m_widget.empty() || m_widget == widget->GetName()  ) &&    //
		    ( m_id.empty()     || m_id     == widget->GetId()    ) &&    // Selector and widget match
		    ( m_class.empty()  || m_class  == widget->GetClass() ) &&    //
		    ( m_state == (-1)  || m_state  == widget->GetState() ) ) ) { //
		// Current stage is a pass...

		// Differentiate between different hierarchy types
		switch( m_hierarchy_type ) {
			case Root: {
				// No parent, matching success
				return true;
			} break;
			case Child: {
				// This is a child, check direct parent only
				return ( GetParent() && GetParent()->Matches( widget->GetParent() ) );
			} break;
			case Descendant: {
				// This is a descendant, check all parents and try to match to all of widgets parents
				for( PtrConst parent = GetParent(); parent; parent = parent->GetParent() ) {
					for( Widget::PtrConst widget_parent = widget->GetParent(); widget_parent; widget_parent = widget_parent->GetParent() ) {
						if( parent->Matches( widget_parent ) ) {
							return true;
						}
					}
				}
			} break;
			default: break;
		}
	}

	// Not wildcard and doesn't match, fail... :(
	return false;
}

}
