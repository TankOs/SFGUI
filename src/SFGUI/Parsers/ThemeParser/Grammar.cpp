#include <SFGUI/Parsers/ThemeParser/Grammar.hpp>

namespace sfg {
namespace parser {
namespace theme {

ThemeGrammar::ThemeGrammar() :
	ell::Parser<char>( &theme_rule, &skipper_rule )
{
	flags.look_ahead = false;

	GrammarToken();
	GrammarSimpleSelector();
	GrammarSelector();
	GrammarStatement();

	theme_rule = ( end_rule | ( +rule_rule )[ &ThemeGrammar::PushTheme ] ) >> end_rule;
	theme_rule.set_name( "Identifier or *" );
}

const std::vector<struct Rule>& ThemeGrammar::GetTheme() const {
	return m_theme;
}

void ThemeGrammar::ClearResults() {
	m_theme.clear();
}

}
}
}
