#include <SFGUI/Parsers/ThemeParser/Grammar.hpp>
#include <SFGUI/Parsers/ThemeParser/Parse.hpp>

#include <vector>
#include <string>
#include <fstream>

namespace sfg {
namespace parser {
namespace theme {

static ThemeGrammar _grammar;

std::string GetLine( std::string str, std::size_t line ) {
	for( std::size_t current_line = 1; current_line < line; ++current_line ) {
		std::size_t position = str.find( '\n' );
		if( ( position == std::string::npos ) || ( position == ( str.size() - 1 ) ) ) {
			return std::string("");
		}

		str = str.substr( position + 1 );
	}

	str = str.substr( 0, str.find( '\n' ) );

	return str;
}

std::size_t ColumnPosition( std::string str, std::size_t string_position ) {
	std::size_t column_position = 0;

	if( string_position >= str.size() ) {
		return 0;
	}

	for( std::size_t index = 0; ( index < string_position ) && ( index < str.size() ); ++index ) {
		++column_position;

		if( str[ index ] == '\n' ) {
			column_position = 0;
		}
	}

	return column_position;
}

std::vector<Rule> ParseString( std::string str ) {
	_grammar.ClearResults();
	//ELL_ENABLE_DUMP( grammar );

	bool result = false;

	std::vector<Rule> rules;

	try {
		_grammar.parse( str.c_str() );
		rules = _grammar.GetTheme();
		result = true;
	}
#ifdef SFGUI_DEBUG
	catch( const std::runtime_error& e ) {
		result = false;

		std::cerr << "Error parsing string:\n"
		 << str << "\n"
		 << std::string( _grammar.position - str.c_str() ,' ' ) << "^\n"
		 << "Expected " << strstr( e.what(), "expecting " ) + 10 << "\n";
	}
#else
	catch( const std::runtime_error& /*e*/ ) {
		result = false;
	}
#endif

	if( result && ( _grammar.get() == '\0' ) ) {
		return rules;
	}

	return std::vector<Rule>();
}

std::vector<Rule> ParseFile( std::string filename ) {
	_grammar.ClearResults();
	//ELL_ENABLE_DUMP( grammar );

	bool result = false;

	std::vector<Rule> rules;

	std::ifstream file( filename.c_str(), std::ifstream::in );
	file.unsetf( std::ios::skipws );

	if( !file.good() ) {
#ifdef SFGUI_DEBUG
		std::cerr << "Error opening file: " << filename << "\n";
#endif
		file.close();
		return std::vector<Rule>();
	}

	std::string str = std::string( std::istreambuf_iterator<char>( file ), std::istreambuf_iterator<char>() );

	try {
		_grammar.parse( str.c_str() );
		rules = _grammar.GetTheme();
		result = true;
	}
#ifdef SFGUI_DEBUG
	catch( const std::runtime_error& e ) {
		result = false;

		std::cerr << "Error parsing file \"" << filename << "\" at line " << _grammar.line_number <<":\n"
		 << GetLine( str, _grammar.line_number ) << "\n"
		 << std::string( ColumnPosition( str, _grammar.position - str.c_str() ) ,' ' ) << "^\n"
		 << "Expected " << strstr( e.what(), "expecting " ) + 10 << "\n";
	}
#else
	catch( const std::runtime_error& /*e*/ ) {
		result = false;
	}
#endif

	if( result && ( _grammar.get() == '\0' ) ) {
		file.close();
		return rules;
	}

	file.close();

	return std::vector<Rule>();
}

}
}
}
