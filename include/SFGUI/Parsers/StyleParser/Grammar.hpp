#pragma once

// Since we don't want to mess with the ell headers (LGPL) to
// fix the warnings we just disable them for these 2 includes.
#if defined( _MSC_VER )
	// TODO: Disable MSVC warnings for these includes too

	#include <ell/Grammar.h>
	#include <ell/Parser.h>


#elif defined( __GNUC__ )
	#pragma GCC push_options
	#pragma GCC diagnostic ignored "-Wshadow"

	#include <ell/Grammar.h>
	#include <ell/Parser.h>

	#pragma GCC pop_options
#else
	#include <ell/Grammar.h>
	#include <ell/Parser.h>
#endif

#include <SFGUI/Parsers/StyleParser/Parse.hpp>

#include <string>
#include <stack>

namespace sfg {
namespace parser {
namespace style {

struct StyleGrammar : ell::Parser<char>, ell::Grammar<char>
{
	public:
		StyleGrammar();

		std::vector<struct Rule> GetStyle();

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
		void PushDeclaration();
		void PushRule();
		void PushStyle();

    ell::Rule<char> end_rule;
		ell::Rule<char> identifier_rule, value_rule;
    ell::Rule<char> type_rule, class_rule, state_rule, id_rule;
    ell::Rule<char> simple_selector_rule, selector_rule;
    ell::Rule<char> declaration_rule, rule_rule, style_rule;

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

    std::stack<struct Declaration> m_declarations;
    std::stack<struct Rule> m_rules;

    std::vector<struct Rule> m_style;
};

}
}
}

