#include <SFGUI/Parsers/ThemeParser/Grammar.hpp>

#include <algorithm>

namespace sfg {
namespace parser {
namespace theme {

void ThemeGrammar::PushIdentifier( const ell::string& identifier ) {
	m_identifiers.push( identifier );
}

void ThemeGrammar::PushValue( const ell::string& value ) {
	m_values.push( value );
}

void ThemeGrammar::PushType() {
	if( m_identifiers.empty() ) {
		m_types.push( "*" );
	}
	else {
		m_types.push( m_identifiers.top() );
		m_identifiers.pop();
	}
}

void ThemeGrammar::PushClass() {
	m_classes.push( m_identifiers.top() );
	m_identifiers.pop();
}

void ThemeGrammar::PushState() {
	m_states.push( m_identifiers.top() );
	m_identifiers.pop();
}

void ThemeGrammar::PushID() {
	m_ids.push( m_identifiers.top() );
	m_identifiers.pop();
}

void ThemeGrammar::PushSimpleSelector() {
	struct SimpleSelector simple_selector;

	if( m_types.empty() ) {
		simple_selector.m_type_selector = "*";
	}
	else {
		simple_selector.m_type_selector = m_types.top().str();
		m_types.pop();
	}

	if( m_classes.empty() ) {
		simple_selector.m_class_selector = "";
	}
	else {
		simple_selector.m_class_selector = m_classes.top().str();
		m_classes.pop();
	}

	if( m_states.empty() ) {
		simple_selector.m_state_selector = "";
	}
	else {
		simple_selector.m_state_selector = m_states.top().str();
		m_states.pop();
	}

	if( m_ids.empty() ) {
		simple_selector.m_id_selector = "";
	}
	else {
		simple_selector.m_id_selector = m_ids.top().str();
		m_ids.pop();
	}

	m_simple_selectors.push( simple_selector );
}

void ThemeGrammar::PushSelector() {
	struct Selector selector;

	if( !m_combinators.empty() ) {
		m_combinators.pop();
	}

	while( !m_simple_selectors.empty() ) {
		auto simple_selector = m_simple_selectors.top();

		if( m_combinators.empty() ) {
			simple_selector.m_combinator = "";
		}
		else {
			simple_selector.m_combinator = m_combinators.top().m_combinator;
			m_combinators.pop();
		}

		selector.m_simple_selectors.push_back( simple_selector );
		m_simple_selectors.pop();
	}

	std::reverse( selector.m_simple_selectors.begin(), selector.m_simple_selectors.end() );

	m_selectors.push( selector );
}

void ThemeGrammar::PushCombinatorChild() {
	struct Combinator combinator;

	combinator.m_combinator = ">";

	m_combinators.push( combinator );
}

void ThemeGrammar::PushCombinatorDescendant() {
	struct Combinator combinator;

	combinator.m_combinator = " ";

	m_combinators.push( combinator );
}

void ThemeGrammar::PushCombinatorGroup() {
	struct Combinator combinator;

	combinator.m_combinator = ",";

	m_combinators.push( combinator );
}

void ThemeGrammar::PushDeclaration() {
	struct Declaration declaration;

	declaration.m_property_name = m_identifiers.top().str();
	m_identifiers.pop();

	declaration.m_property_value = m_values.top().str();
	m_values.pop();

	m_declarations.push( declaration );
}

void ThemeGrammar::PushRule() {
	struct Rule rule;

	if( m_selectors.empty() ) {
		struct Selector selector;
		rule.m_selector = selector;
	}
	else {
		rule.m_selector = m_selectors.top();
		m_selectors.pop();
	}

	while( !m_declarations.empty() ) {
		rule.m_declarations.push_back( m_declarations.top() );
		m_declarations.pop();
	}

	std::reverse( rule.m_declarations.begin(), rule.m_declarations.end() );

	m_rules.push( rule );
}

void ThemeGrammar::PushTheme() {
	while( !m_rules.empty() ) {
		m_theme.push_back( m_rules.top() );
		m_rules.pop();
	}

	std::reverse( m_theme.begin(), m_theme.end() );
}

}
}
}
