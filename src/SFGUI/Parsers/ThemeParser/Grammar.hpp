#pragma once
#include <ciso646> // For our beloved friend MSVC.

#include <SFGUI/Parsers/ThemeParser/Parse.hpp>
#include <ell/Grammar.h>
#include <ell/Parser.h>
#include <string>
#include <stack>

namespace sfg {
namespace parser {
namespace theme {

struct ThemeGrammar : ell::Parser<char>, ell::Grammar<char>
{
	public:
		ThemeGrammar();

		const std::vector<struct Rule>& GetTheme() const;
		void ClearResults();

	private:
		void GrammarToken();
		void GrammarSimpleSelector();
		void GrammarSelector();
		void GrammarStatement();

		void PushIdentifier( const ell::string& identifier );
		void PushValue( const ell::string& value );
		void PushType();
		void PushClass();
		void PushState();
		void PushID();
		void PushSimpleSelector();
		void PushSelector();
		void PushCombinatorChild();
		void PushCombinatorDescendant();
		void PushCombinatorGroup();
		void PushDeclaration();
		void PushRule();
		void PushTheme();

		ell::Rule<char> end_rule;
		ell::Rule<char> identifier_rule, value_rule;
		ell::Rule<char> type_rule, class_rule, state_rule, id_rule;
		ell::Rule<char> simple_selector_rule, selector_combinator_rule, selector_rule;
		ell::Rule<char> declaration_rule, rule_rule, theme_rule;

		ell::Rule<char> skipper_rule;
		ell::Rule<char> comment_rule;

		std::stack<ell::string> m_identifiers;
		std::stack<ell::string> m_values;

		std::stack<ell::string> m_types;
		std::stack<ell::string> m_classes;
		std::stack<ell::string> m_states;
		std::stack<ell::string> m_ids;

		std::stack<struct SimpleSelector> m_simple_selectors;
		std::stack<struct Selector> m_selectors;

		std::stack<struct Combinator> m_combinators;

		std::stack<struct Declaration> m_declarations;
		std::stack<struct Rule> m_rules;

		std::vector<struct Rule> m_theme;
};

}
}
}

