#include <SFGUI/Parsers/ThemeParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace theme {

void ThemeGrammar::GrammarToken() {
	comment_rule = ( str( "/*" ) >> any * str( "*/" ) );
	skipper_rule = ( blank | comment_rule );

	end_rule = ell::CharGrammarBase<char>::end;

	identifier_rule = lexeme( alpha >> *( alnum | chset("-") ) )[ &ThemeGrammar::PushIdentifier ];
	value_rule = lexeme( +( alnum | chset(".# /-") ) )[ &ThemeGrammar::PushValue ];

	skipper_rule.set_name( "Skipper" );
	comment_rule.set_name( "Comment" );

	end_rule.set_name( "Identifier or *" );

	identifier_rule.set_name( "Identifier" );
	value_rule.set_name( "Value" );
}

}
}
}
