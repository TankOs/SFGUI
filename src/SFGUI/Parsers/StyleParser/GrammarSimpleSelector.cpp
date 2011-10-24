#include <SFGUI/Parsers/StyleParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace style {

void StyleGrammar::GrammarSimpleSelector() {
	type_rule  = ( identifier_rule )[ &StyleGrammar::PushType ];
	class_rule = ( ch('.') >> identifier_rule )[ &StyleGrammar::PushClass ];
	state_rule = ( ch(':') >> identifier_rule )[ &StyleGrammar::PushState ];
	id_rule    = ( ch('#') >> identifier_rule )[ &StyleGrammar::PushID ];

	type_rule.set_name( "Type or *" );
	class_rule.set_name( "Class" );
	state_rule.set_name( "State" );
	id_rule.set_name( "ID" );
}

}
}
}
