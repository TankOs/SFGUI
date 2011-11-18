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

#ifndef INCLUDED_ELL_NUMERICS_H
#define INCLUDED_ELL_NUMERICS_H

#include <ell/Node.h>
#include <ell/Parser.h>

namespace ell
{
    namespace
    {
        //@{
        /// Extract optional sign prefix
        template <typename Token, typename T>
        struct GetSign
        {
            int operator () (Parser<Token> * parser)
            {
                int r = 1;
                switch (parser->get())
                {
                case '-':
                    r = - 1;
                case '+':
                    do
                    {
                        parser->next();
                    }
                    while ((parser->get() == ' ') | (parser->get() == '\t'));
                }
                return r;
            }

            bool is_signed() { return true; }
        };

#       define D(T)                                          \
        template <typename Token>                            \
        struct GetSign<Token, T>                             \
        {                                                    \
            int operator () (Parser<Token> *) { return 1; }  \
            bool is_signed() { return false; }               \
        };
        D(unsigned char)
        D(unsigned)
        D(unsigned long)
        D(unsigned long long)
#       undef D
        //@}

        //@{
        /// Check radix: return 0 <= n < R, or a negative value if error
        template <typename Token, const int R>
        struct GetDigit
        {
            int operator () (Parser<Token> * parser)
            {
                int d = parser->get() - '0';
                if (d >= R)
                    return -1;
                return d;
            }
        };

        template <typename Token>
        struct GetDigit<Token, 16>
        {
            int operator () (Parser<Token> * parser)
            {
                int c = parser->get();

                if (c < '0') return -1;
                if (c <= '9') return (c - '0');
                if (c < 'A') return -1;
                if (c <= 'F') return (10 + c - 'A');
                if (c < 'a') return -1;
                if (c <= 'f') return (10 + c - 'a');

                return -1;
            }
        };
        //@}
    } // Anonymous namespace

    /// Generic integer parser
    template <typename Token, typename Sign = unsigned long, const int Radix = 10, const int MinDigits = 1, const int MaxDigits = 200>
    struct Int : public ConcreteNodeBase<Token, Int<Token, Sign, Radix, MinDigits, MaxDigits> >
    {
        using ConcreteNodeBase<Token, Int<Token, Sign, Radix, MinDigits, MaxDigits> >::parse;

        template <typename V>
        bool parse(Parser<Token> * parser, Storage<V> & se) const
        {
            ELL_BEGIN_PARSE
            typename Parser<Token>::Context sav_pos(parser);

            int digit_nb = 0;
            Storage<Sign> s;
            s.value = 0;
            int sign = GetSign<Token, Sign>()(parser);
            int d = 0;

            // Check mandatory digits
            while (digit_nb < MinDigits)
            {
                d = GetDigit<Token, Radix>()(parser);

                if (d < 0)
                {
                    sav_pos.restore(parser);
                    break;
                }

                s.value = s.value * Radix + d;
                ++digit_nb;
                parser->next();
            }

            if (d >= 0)
            {
                // Check optional digits
                while (1)
                {
                    d = GetDigit<Token, Radix>()(parser);

                    if (d < 0)
                        break;
                    if (digit_nb >= MaxDigits)
                        break;

                    s.value = s.value * Radix + d;
                    ++digit_nb;
                    parser->next();
                }

                if (d >= 0)
                {
                    sav_pos.restore(parser); // Too much digits
                }
                else
                {
                    match = true;
                    s.value *= sign;
                    assign(se, s);
                }
            }
            ELL_END_PARSE
        }

        std::string get_kind() const
        {
            std::ostringstream oss;
            oss << (GetSign<Token, Sign>().is_signed() ? "signed-" : "unsigned-")
                << (Radix == 8 ? "octal" : Radix == 16 ? "hexadecimal" : "decimal")
                << "-" << MinDigits << "-" << MaxDigits;
            return oss.str();
        }
    };

    /// Partial template specialization for numbers of any digits
    template <typename Token, typename Sign, const int Radix>
    struct Int<Token, Sign, Radix, 1, 200>
      : public ConcreteNodeBase<Token, Int<Token, Sign, Radix, 1, 200> >
    {
        using ConcreteNodeBase<Token, Int<Token, Sign, Radix, 1, 200> >::parse;

        template <typename V>
        bool parse(Parser<Token> * parser, Storage<V> & se) const
        {
            ELL_BEGIN_PARSE
            typename Parser<Token>::Context sav_pos(parser);

            Storage<Sign> s;
            s.value = 0;
            int sign = GetSign<Token, Sign>()(parser);
            int d = GetDigit<Token, Radix>()(parser);

            if (d >= 0)
            {
                s.value = s.value * Radix + d;
                parser->next();
                match = true;

                while (1)
                {
                    d = GetDigit<Token, Radix>()(parser);
                    if (d < 0)
                        break;
                    s.value = s.value * Radix + d;
                    parser->next();
                }

                s.value *= sign;
                assign(se, s);
            }
            else
            {
                sav_pos.restore(parser);
            }
            ELL_END_PARSE
        }

        std::string get_kind() const
        {
            return std::string(GetSign<Token, Sign>().is_signed() ? "" : "unsigned-")
                   + (Radix == 8 ? "octal" : Radix == 16 ? "hexadecimal" : "decimal");
        }

    };

    /// No wide string real number parsing implemented
    template <typename Size=double>
    struct Rl : public ConcreteNodeBase<char, Rl<Size> >
    {
        using ConcreteNodeBase<char, Rl<Size> >::parse;

        template <typename V>
        bool parse(Parser<char> * parser, Storage<V> & s) const
        {
            ELL_BEGIN_PARSE
            char * endptr;
            Storage<Size> sd;
            sd.value = strtod(parser->position, & endptr);

            if (endptr > parser->position)
            {
                assign(s, sd);
                parser->position = endptr;
                match = true;
            }
            ELL_END_PARSE
        }

        std::string get_kind() const { return "real"; }
    };
}

#endif // INCLUDED_ELL_NUMERICS_H
