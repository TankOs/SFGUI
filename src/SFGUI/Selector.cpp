#include <SFGUI/Selector.hpp>
#include <SFGUI/Container.hpp>
#include <SFGUI/Widget.hpp>

namespace sfg {

Selector::Selector() :
	m_hierarchy_type( HierarchyType::INVALID ),
	m_hash( 0 )
{
}

Selector::Selector( const Selector& other ) :
	m_hierarchy_type( other.m_hierarchy_type ),
	m_widget( other.m_widget ),
	m_id( other.m_id ),
	m_class( other.m_class ),
	m_hash( 0 )
{
	if( other.m_state ) {
		m_state.reset( new Widget::State( *other.m_state ) );
	}

	if( other.m_parent ) {
		m_parent.reset( new Selector( *other.m_parent ) );
	}
}

Selector& Selector::operator=( const Selector& other ) {
	m_hierarchy_type = other.m_hierarchy_type;
	m_widget = other.m_widget;
	m_id = other.m_id;
	m_class = other.m_class;
	m_hash = other.m_hash;

	if( other.m_state ) {
		m_state.reset( new Widget::State( *other.m_state ) );
	}

	if( other.m_parent ) {
		m_parent.reset( new Selector( *other.m_parent ) );
	}
	else {
		m_parent.reset();
	}

	return *this;
}

Selector::Ptr Selector::Create( const std::string& widget, const std::string& id, const std::string& class_, const std::string& state, HierarchyType hierarchy, Ptr parent ) {
	Ptr selector( new Selector );

	selector->m_widget = widget;
	selector->m_id = id;
	selector->m_class = class_;

	std::string uppercase_state( state );
	std::transform( uppercase_state.begin(), uppercase_state.end(), uppercase_state.begin(), toupper );

	if( uppercase_state == "NORMAL" ) {
		selector->m_state.reset( new Widget::State( Widget::State::NORMAL ) );
	}
	else if( uppercase_state == "ACTIVE" ) {
		selector->m_state.reset( new Widget::State( Widget::State::ACTIVE ) );
	}
	else if( uppercase_state == "PRELIGHT" ) {
		selector->m_state.reset( new Widget::State( Widget::State::PRELIGHT ) );
	}
	else if( uppercase_state == "SELECTED" ) {
		selector->m_state.reset( new Widget::State( Widget::State::SELECTED ) );
	}
	else if( uppercase_state == "INSENSITIVE" ) {
		selector->m_state.reset( new Widget::State( Widget::State::INSENSITIVE ) );
	}
	else if( !uppercase_state.empty() ) {
		throw( ParserException( "Invalid state: " + state ) );
	}

	selector->m_hierarchy_type = hierarchy;

	if( hierarchy != HierarchyType::ROOT ) {
		selector->m_parent = parent;
	}

	selector->m_hash = std::hash<std::string>()( selector->BuildString() );

	return selector;
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

const Widget::State* Selector::GetState() const {
	return m_state.get();
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
			case HierarchyType::CHILD: {
				str += ">";
			} break;
			case HierarchyType::DESCENDANT: {
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

	if( m_state ) {
		str += ":";

		switch( *m_state ) {
			case Widget::State::NORMAL:
				str += "NORMAL";
				break;
			case Widget::State::PRELIGHT:
				str += "PRELIGHT";
				break;
			case Widget::State::ACTIVE:
				str += "ACTIVE";
				break;
			case Widget::State::SELECTED:
				str += "SELECTED";
				break;
			case Widget::State::INSENSITIVE:
				str += "INSENSITIVE";
				break;

			default:
				str += "UNKNOWN";
				break;
		}
	}

	return str;
}

bool Selector::operator==( const Selector& other ) const {
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
	if( ( !m_widget.compare("*") && m_id.empty() && m_class.empty() && !m_state ) || // Wildcard
		 ( ( m_widget.empty() || !m_widget.compare("*") || m_widget == widget->GetName() ) && //
		 ( m_id.empty() || m_id == widget->GetId() ) && // Selector and widget match
		 ( m_class.empty() || m_class  == widget->GetClass() ) && //
		 ( !m_state || *m_state == widget->GetState() ) ) ) { //
		// Current stage is a pass...

		// Differentiate between different hierarchy types
		switch( m_hierarchy_type ) {
			case HierarchyType::ROOT: {
				// No parent, matching success
				return true;
			} break;
			case HierarchyType::CHILD: {
				// This is a child, check direct parent only
				return ( GetParent() && GetParent()->Matches( widget->GetParent() ) );
			} break;
			case HierarchyType::DESCENDANT: {
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

int Selector::GetScore() const {
	int score = 0;

	score += ( ( GetWidgetName().empty() || ( GetWidgetName() == "*" ) ) ? 0 : 1 );
	score += ( ( !GetState() ) ? 0 : 1 );
	score += ( GetClass().empty() ? 0 : 100 );
	score += ( GetId().empty() ? 0 : 10000 );

	if( ( m_hierarchy_type != HierarchyType::ROOT ) && GetParent() ) {
		score += GetParent()->GetScore();
	}

	return score;
}

}
