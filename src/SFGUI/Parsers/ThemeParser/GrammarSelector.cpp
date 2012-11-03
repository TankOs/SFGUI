#include <SFGUI/Parsers/ThemeParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace theme {

void ThemeGrammar::GrammarSelector() {
	simple_selector_rule = (
		(
			// Simple selector including type or universal selector (*).
			( type_rule ) >> !(
				( class_rule >> !( ( state_rule >> !id_rule ) | ( id_rule >> !state_rule ) ) ) |
				( state_rule >> !( ( id_rule >> !class_rule ) | ( class_rule >> !id_rule ) ) ) |
				( id_rule >> !( ( state_rule >> !class_rule ) | ( class_rule >> !state_rule ) ) )
			)
		) | (
			// Simple selector omitting type or universal selector (*).
			// The universal selector is implied in this case.
			( class_rule >> !( ( state_rule >> !id_rule ) | ( id_rule >> !state_rule ) ) ) |
			( state_rule >> !( ( id_rule >> !class_rule ) | ( class_rule >> !id_rule ) ) ) |
			( id_rule >> !( ( state_rule >> !class_rule ) | ( class_rule >> !state_rule ) ) )
		)
	)[ &ThemeGrammar::PushSimpleSelector ];

	selector_combinator_rule = ( ( ch('>') )[ &ThemeGrammar::PushCombinatorChild ] |
	 ( ch(',') )[ &ThemeGrammar::PushCombinatorGroup ] |
	 ( eps )[ &ThemeGrammar::PushCombinatorDescendant ] );

	selector_rule = ( simple_selector_rule % selector_combinator_rule )[ &ThemeGrammar::PushSelector ];

	simple_selector_rule.set_name( "Simple Selector" );
	selector_combinator_rule.set_name( "Selector Combinator" );
	selector_rule.set_name( "Selector" );
}

}
}
}
