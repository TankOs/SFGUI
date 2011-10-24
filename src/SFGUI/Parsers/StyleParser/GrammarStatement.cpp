#include <SFGUI/Parsers/StyleParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace style {

void StyleGrammar::GrammarStatement() {
	declaration_rule = ( identifier_rule >> ch(':') >> value_rule >> ch(';') )[ &StyleGrammar::PushDeclaration ];
	rule_rule        = ( selector_rule >> ch('{') >> *declaration_rule >> ch('}') )[ &StyleGrammar::PushRule ];

	declaration_rule.set_name( "Declaration" );
	rule_rule.set_name( "Rule" );
}

}
}
}
