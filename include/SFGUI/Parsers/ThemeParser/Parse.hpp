#pragma once

#include <SFGUI/Config.hpp>

#include <string>
#include <vector>

namespace sfg {
namespace parser {
namespace theme {

struct Declaration {
	std::string m_property_name;
	std::string m_property_value;
};

struct Combinator {
	std::string m_combinator;
};

struct SimpleSelector {
	std::string m_type_selector;
	std::string m_class_selector;
	std::string m_state_selector;
	std::string m_id_selector;

	std::string m_combinator;
};

struct Selector {
	std::vector<SimpleSelector> m_simple_selectors;
};

struct Rule {
	Selector m_selector;
	std::vector<Declaration> m_declarations;
};

std::vector<Rule> ParseString( std::string str );
std::vector<Rule> ParseFile( std::string filename );

typedef std::vector<Rule> Theme;

}
}
}
