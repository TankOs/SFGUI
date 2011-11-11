#include <SFGUI/Parsers/StyleParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace style {

void StyleGrammar::GrammarToken() {
	comment_rule    = ( str( "/*" ) >> any * str( "*/" ) );
	skipper_rule    = ( blank | comment_rule );

	end_rule        = ell::CharGrammarBase<char>::end;

	identifier_rule = lexeme( alpha >> *( alnum | chset("-") ) )[ &StyleGrammar::PushIdentifier ];
	value_rule      = lexeme( +( alnum | chset(".#- /") ) )[ &StyleGrammar::PushValue ];

	skipper_rule.set_name( "Skipper" );
	comment_rule.set_name( "Comment" );

	end_rule.set_name( "Identifier or *" );

	identifier_rule.set_name( "Identifier" );
	value_rule.set_name( "Value" );
}

}
}
}
