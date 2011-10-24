#include <SFGUI/Parsers/StyleParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace style {

void StyleGrammar::GrammarSelector() {
	simple_selector_rule = ( ( type_rule | ch('*') ) >> !(
	                       ( class_rule >> !( ( state_rule >> !id_rule ) | ( id_rule >> !state_rule ) ) ) |
	                       ( state_rule >> !( ( id_rule >> !class_rule ) | ( class_rule >> !id_rule ) ) ) |
	                       ( id_rule >> !( ( state_rule >> !class_rule ) | ( class_rule >> !state_rule ) ) )
	                       ) )[ &StyleGrammar::PushSimpleSelector ];

	selector_rule        = ( simple_selector_rule % ch('>') )[ &StyleGrammar::PushSelector ];

	simple_selector_rule.set_name( "Simple Selector" );
	selector_rule.set_name( "Selector" );
}

}
}
}
