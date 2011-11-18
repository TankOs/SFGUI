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

#ifndef INCLUDED_ELL_PRIMITIVES_H
#define INCLUDED_ELL_PRIMITIVES_H

#include <ell/Node.h>
#include <ell/Parser.h>

namespace ell
{
    template <typename Token, typename ConcreteNode>
    struct TokenPrimitiveBase : public ConcreteNodeBase<Token, ConcreteNode>
    {
        using ConcreteNodeBase<Token, ConcreteNode>::parse;

        template <typename V>
        bool parse(Parser<Token> * parser, Storage<V> & s) const
        {
            Storage<Token> si;
            si.value = parser->get();
            assign(s, si);
            return ((ConcreteNode *) this)->parse(parser);
        }
    };

    /// Epsilon (equivalent to `no_consume(any)`)
    template <typename Token>
    struct Eps : public TokenPrimitiveBase<Token, Eps<Token> >
    {
        using TokenPrimitiveBase<Token, Eps<Token> >::parse;
        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            match = true;
            ELL_END_PARSE
        }

        std::string get_kind() const { return "epsilon"; }
    };

    /// Never match
    template <typename Token>
    struct Nop : public ConcreteNodeBase<Token, Nop<Token> >
    {
        using ConcreteNodeBase<Token, Nop<Token> >::parse;
        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            ELL_END_PARSE
        }

        std::string get_kind() const { return "nop"; }
    };

    /// Always match
    template <typename Token>
    struct Any : public TokenPrimitiveBase<Token, Any<Token> >
    {
        using TokenPrimitiveBase<Token, Any<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            if (not parser->end())
            {
                match = true;
                parser->next();
            }
            ELL_END_PARSE
        }

        std::string get_kind() const { return "any"; }
    };

    /// End of tokens stream
    template <typename Token>
    struct EoS : public TokenPrimitiveBase<Token, EoS<Token> >
    {
        using TokenPrimitiveBase<Token, EoS<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            match = parser->end();
            ELL_END_PARSE
        }

        std::string get_kind() const { return "end"; }
    };

    /// Charset
    template <typename Token>
    struct ChS : public TokenPrimitiveBase<Token, ChS<Token> >
    {
        ChS(const std::string & s)
            : set(s)
        { }

        using TokenPrimitiveBase<Token, ChS<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            const char * p = & set[0];
            const wchar_t c = parser->get();
            while (* p)
            {
                const wchar_t first = * p;
                if (c == first)
                {
                    match = true;
                    parser->next();
                    break;
                }
                ++p;
                if (* p == '-' && * (p + 1))
                {
                    const wchar_t second = * (p + 1);

                    if ((c >= first) & (c <= second))
                    {
                        parser->next();
                        match = true;
                        break;
                    }

                    p += 2;
                }
            }
            ELL_END_PARSE
        }

        std::string get_kind() const { return "charset"; }
        std::string get_value() const { return set; }
        std::string set;
    };

    /// Character, ie a litteral token
    template <typename Token>
    struct Ch : public TokenPrimitiveBase<Token, Ch<Token> >
    {
        Ch(const Token _c)
          : c(_c)
        { }

        std::string get_kind() const { return "char"; }
        using TokenPrimitiveBase<Token, Ch<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            if (parser->get() == c)
            {
                parser->next();
                match = true;
            }
            ELL_END_PARSE
        }

        std::string get_value() const
        {
            std::ostringstream os;
            os << c;
            return os.str();
        }

        const Token c;
    };

    /// Token range
    template <typename Token, const Token C1, const Token C2>
    struct Rg : public TokenPrimitiveBase<Token, Rg<Token, C1, C2> >
    {
        using TokenPrimitiveBase<Token, Rg<Token, C1, C2> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            const Token c = parser->get();
            const int d1 = c - C1;
            const int d2 = c - C2;
            if (d1 >= 0 && d2 <= 0)
            {
                parser->next();
                match = true;
            }
            ELL_END_PARSE
        }

        std::string get_value() const { return std::basic_string<Token>() + C1 + '-' + C2; }
        std::string get_kind() const { return "range"; }
    };

    /// Error raiser
    template <typename Token>
    struct Err : public ConcreteNodeBase<Token, Err<Token> >
    {
        Err(const std::string & s)
            : str(s)
        { }

        using ConcreteNodeBase<Token, Err<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            parser->raise_error(str);
            ELL_END_PARSE
        }

        std::string get_kind() const { return "error"; }
        std::string get_value() const { return str; }
        std::string str;
    };

    /// Case insensitive string
    template <typename Token>
    struct IStr : public ConcreteNodeBase<Token, IStr<Token> >
    {
        IStr(const std::basic_string<Token> & s)
          : str(s)
        { }

        using ConcreteNodeBase<Token, IStr<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            typename Parser<Token>::Context sav_pos(parser);
            const Token * p = & str[0];
            match = true;
            while (* p)
            {
                const wchar_t c = * p;
                const wchar_t gc = parser->get();

                if ((c == gc) |
                    ((c >= 'A') & (c <= 'Z') & (c + 32 == gc)) |
                    ((c >= 'a') & (c <= 'z') & (c - 32 == gc)) )
                {
                    parser->next();
                    ++p;
                }
                else
                {
                    match = false;
                    sav_pos.restore(parser);
                    break;
                }
            }
            ELL_END_PARSE
        }

        std::string get_value() const { return str; }
        std::string get_kind() const { return "ignore-case-string"; }
        std::basic_string<Token> str;
    };

    /// Exact matching string parser
    /// Can compare strings of different types if == operator is defined between them
    /// The ostream << operator must be defined for Token
    template <typename Token>
    struct Str : public ConcreteNodeBase<Token, Str<Token> >
    {
        Str(const std::basic_string<Token> & s)
          : str(s)
        { }

        using ConcreteNodeBase<Token, Str<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            typename Parser<Token>::Context sav_pos(parser);
            const Token * p = str.c_str();
            match = true;
            while (* p)
            {
                if (! (* p == parser->get()))
                {
                    match = false;
                    sav_pos.restore(parser);
                    break;
                }
                parser->next();
                ++p;
            }
            ELL_END_PARSE
        }

        std::string get_value() const { return str; }
        std::string get_kind() const { return "string"; }
        std::basic_string<Token> str;
    };

    /// Keywords
    template <typename Token>
    struct Kw : public ConcreteNodeBase<Token, Kw<Token> >
    {
        Kw(const std::basic_string<Token> & s)
          : decorated(Str<Token>(s), ChS<Token>("a-zA-Z0-9_"))
        { }

        using ConcreteNodeBase<Token, Kw<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> & v) const
        {
            return decorated.parse(parser, v);
        }

        std::string get_value() const { return decorated.left.str; }
        std::string get_kind() const { return "keyword"; }

        NSx<Token, Str<Token>, ChS<Token> > decorated;
    };

    /// Case-insensitive keywords
    template <typename Token>
    struct IKw : public ConcreteNodeBase<Token, IKw<Token> >
    {
        IKw(const std::basic_string<Token> & s)
          : decorated(IStr<Token>(s), ChS<Token>("a-zA-Z0-9_"))
        { }

        using ConcreteNodeBase<Token, IKw<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> & v) const
        {
            return decorated.parse(parser, v);
        }

        std::string get_value() const { return decorated.left.str; }
        std::string get_kind() const { return "ignore-case-keyword"; }

        NSx<Token, IStr<Token>, ChS<Token> > decorated;
    };

    /// C-like identifiers
    template <typename Token>
    struct Idt : public ConcreteNodeBase<Token, Idt<Token> >
    {
        using ConcreteNodeBase<Token, Idt<Token> >::parse;

        bool parse(Parser<Token> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            wchar_t c = parser->get();
            if (((c >= 'a') & (c <= 'z')) |
                ((c >= 'A') & (c <= 'Z')) |
                (c == '_'))
            {
                match = true;
                do
                {
                  parser->next();
                  c = parser->get();
                }
                while (((c >= 'a') & (c <= 'z')) |
                       ((c >= 'A') & (c <= 'Z')) |
                       ((c >= '0') & (c <= '9')) |
                       (c == '_'));
            }
            ELL_END_PARSE
        }

        std::string get_kind() const { return "identifier"; }
        std::string get_value() const { return "identifier"; }
    };

    /// Only for byte strings...
    struct UTF8NonASCII : public ConcreteNodeBase<char, UTF8NonASCII>
    {
        using ConcreteNodeBase<char, UTF8NonASCII>::parse;

        bool parse(Parser<char> * parser, Storage<void> &) const
        {
            ELL_BEGIN_PARSE
            Parser<char>::Context sav_pos(parser);

            unsigned char c = parser->get();
            if (c < 0xC0 || c > 0xFD)
                match = false;
            else
            {
                match = true;
                parser->next();
                c = parser->get();
                int n = 1;

                if (c >= 0xE0)
                    n++;
                if (c >= 0xF0)
                    n++;
                if (c >= 0xF8)
                    n++;
                if (c >= 0xFC)
                    n++;

                for (int i = 0; i < n; ++i)
                {
                    c = parser->get();
                    parser->next();
                    if (c < 0x80 || c > 0xBF)
                    {
                        match = false;
                        sav_pos.restore(parser);
                        break;
                    }
                }
            }
            ELL_END_PARSE
        }

        std::string get_kind() const { return "utf8nonascii"; }
    };
}

#endif // INCLUDED_ELL_PRIMITIVES_H
