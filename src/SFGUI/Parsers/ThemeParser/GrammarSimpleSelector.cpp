#include <SFGUI/Parsers/ThemeParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace theme {

void ThemeGrammar::GrammarSimpleSelector() {
	type_rule = ( identifier_rule | ch('*') )[ &ThemeGrammar::PushType ];
	class_rule = ( ch('.') >> identifier_rule )[ &ThemeGrammar::PushClass ];
	state_rule = ( ch(':') >> identifier_rule )[ &ThemeGrammar::PushState ];
	id_rule = ( ch('#') >> identifier_rule )[ &ThemeGrammar::PushID ];

	type_rule.set_name( "Type or *" );
	class_rule.set_name( "Class" );
	state_rule.set_name( "State" );
	id_rule.set_name( "ID" );
}

}
}
}
