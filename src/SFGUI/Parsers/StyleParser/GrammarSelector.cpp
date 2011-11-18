#include <SFGUI/Parsers/StyleParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace style {

void StyleGrammar::GrammarSelector() {
	simple_selector_rule     = ( ( type_rule ) >> !(
	                           ( class_rule >> !( ( state_rule >> !id_rule ) | ( id_rule >> !state_rule ) ) ) |
	                           ( state_rule >> !( ( id_rule >> !class_rule ) | ( class_rule >> !id_rule ) ) ) |
	                           ( id_rule >> !( ( state_rule >> !class_rule ) | ( class_rule >> !state_rule ) ) )
	                           ) )[ &StyleGrammar::PushSimpleSelector ];

	selector_combinator_rule = ( ( ch('>') )[ &StyleGrammar::PushCombinatorChild ] |
	                             ( ch(',') )[ &StyleGrammar::PushCombinatorGroup ] |
	                             ( eps     )[ &StyleGrammar::PushCombinatorDescendant ] );

	selector_rule            = ( simple_selector_rule % selector_combinator_rule )[ &StyleGrammar::PushSelector ];

	simple_selector_rule.set_name( "Simple Selector" );
	selector_combinator_rule.set_name( "Selector Combinator" );
	selector_rule.set_name( "Selector" );
}

}
}
}
