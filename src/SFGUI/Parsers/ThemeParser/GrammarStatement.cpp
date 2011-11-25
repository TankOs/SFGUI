#include <SFGUI/Parsers/ThemeParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace theme {

void ThemeGrammar::GrammarStatement() {
	declaration_rule = ( identifier_rule >> ch(':') >> value_rule >> ch(';') )[ &ThemeGrammar::PushDeclaration ];
	rule_rule = ( selector_rule >> ch('{') >> *declaration_rule >> ch('}') )[ &ThemeGrammar::PushRule ];

	declaration_rule.set_name( "Declaration" );
	rule_rule.set_name( "Rule" );
}

}
}
}
