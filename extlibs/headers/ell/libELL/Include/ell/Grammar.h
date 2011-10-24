// This file is part of Ell library.
//
// Ell library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Ell library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Ell library.  If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_ELL_GRAMMAR_H
#define INCLUDED_ELL_GRAMMAR_H

#include <ell/Node.h>
#include <ell/Rule.h>

namespace ell
{
    template <typename Token>
    struct GrammarBase
    {
        virtual ~GrammarBase() { }

        EoS<Token>                      end;
        Eps<Token>                      eps;
        Any<Token>                      any;
        Nop<Token>                      nop;

        template <typename T>
        Ch<Token>                          ch(const T & t) const { return Ch<Token>((Token) t); }

#       define ELL_FLAG(FLAG, CLASS)                                                                              \
        template <typename P>                                                                                     \
        CLASS##Md<Token, P, true>          FLAG(const P & p) const { return CLASS##Md<Token, P, true>(p); }       \
                                                                                                                  \
        template <typename P>                                                                                     \
        CLASS##Md<Token, P, false>         no_##FLAG(const P & p) const { return CLASS##Md<Token, P, false>(p); } \

        ELL_PARSER_FLAGS
#       undef ELL_FLAG

        template <typename P, typename SK>
        SSk<Token, P, SK>                  skip(const P & p, const SK & skipper) { return SSk<Token, P, SK>(p, skipper); }

        template <typename P>
        NCs<Token, P>                       no_consume(const P & p) const { return NCs<Token, P>(p); }

        template <typename P>
        Lx<Token, P>                        lexeme(const P & p) const { return Lx<Token, P>(p); }

        template <const int exact, typename P>
        Rp<Token, P, exact, exact>          repeat(const P & p) const { return Rp<Token, P, exact, exact>(p); }

        template <const int min, const int max, typename P>
        Rp<Token, P, min, max>              repeat(const P & p) const { return Rp<Token, P, min, max>(p); }

        template <typename P, typename CP>
        DRp<Token, P, CP>                   repeat(const int CP::*exact, const P & p) const { return DRp<Token, P, CP>(p, exact, exact); }

        template <typename P, typename CP>
        DRp<Token, P, CP>                   repeat(const int CP::*min, const int CP::*max, const P & p) const { return DRp<Token, P, CP>(p, min, max); }

        template <typename P, typename Suffix>
        NSx<Token, P, Suffix>               no_suffix(const P & p, const Suffix & s) const { return NSx<Token, P, Suffix>(p, s); }

        Err<Token>                          error(const std::string & msg) const { return Err<Token>(msg); }
    };

    template <typename Token>
    struct CharGrammarBase : public GrammarBase<Token>
    {
        CharGrammarBase()
        {
            alpha = chset("a-zA-Z_");
            alnum = chset("a-zA-Z0-9_");
            blank = chset(" \t\n\r\t\v");
            space = chset(" \t\r\t\v"); 
            digit = range<(Token) '0', (Token) '9'>();
            upper = range<(Token) 'A', (Token) 'Z'>();
            lower = range<(Token) 'a', (Token) 'z'>();
            visible_ascii = range<(Token) '!', (Token) '~'>();

            alpha.set_name("alphabetic char");
            alnum.set_name("alphanumeric char or underscore");
            blank.set_name("blank char");
            space.set_name("white space");
            digit.set_name("digit");
            upper.set_name("upper case letter");
            lower.set_name("lower case letter");
            visible_ascii.set_name("visible ASCII char");
        }

        Rule<Token> alpha, alnum, blank, space, digit, upper, lower, visible_ascii;

        Idt<Token>                          ident;                           

        UTF8NonASCII                        utf8nonascii;

        Int<Token, signed long>             signed_dec;

        template <typename Sign, const int Radix, const int MinDigits, const int MaxDigits>
        Int<Token, Sign, Radix, MinDigits, MaxDigits>
                                            integer() const { return Int<Token, Sign, Radix, MinDigits, MaxDigits>(); }

        Int<Token, unsigned long>           dec;
        Int<Token, unsigned long, 16>       hex;
        Int<Token, unsigned long, 8>        oct;
        Int<Token, unsigned long, 2>        bin;

        Rl<double>                          real;

        template <const Token C1, const Token C2>
        Rg<Token, C1, C2>                   range() const { return Rg<Token, C1, C2>(); }

        ChS<Token>                          chset(const std::string & set) const { return ChS<Token>(set); }

        Str<Token>                          str(const std::basic_string<Token> & arg) const { return Str<Token>(arg); }

        IStr<Token>                         istr(const std::basic_string<Token> & arg) const { return IStr<Token>(arg); }

        Kw<Token>                           kw(const std::basic_string<Token> & s) const { return Kw<Token>(s); }

        IKw<Token>                          ikw(const std::basic_string<Token> & s) const { return IKw<Token>(s); }
    };

    template <typename Token>
    struct Grammar : public GrammarBase<Token>
    { };

    template <>
    struct Grammar<char> : public CharGrammarBase<char>
    { };

    template <>
    struct Grammar<wchar_t> : public CharGrammarBase<wchar_t>
    { };
}

#endif // INCLUDED_ELL_GRAMMAR_H
