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

#ifndef INCLUDED_ELL_UNARY_NODES_H
#define INCLUDED_ELL_UNARY_NODES_H

#include <ell/UnaryNode.h>
#include <ell/Parser.h>

namespace ell
{
#   define ELL_FLAG(FLAG, NAME)                                                  \
    template <typename Token, typename Child, const bool NV>                     \
    struct NAME ## Md                                                            \
      : public UnaryNode<Token, NAME ## Md<Token, Child, NV>, Child>             \
    {                                                                            \
        typedef UnaryNode<Token, NAME ## Md<Token, Child, NV>, Child> Base;      \
                                                                                 \
        NAME ## Md(const Child & target)                                         \
          : Base(target)                                                         \
        { }                                                                      \
                                                                                 \
        std::string get_kind() const                                             \
        {                                                                        \
            std::string s = #FLAG;                                               \
            std::replace(s.begin(), s.end(), '_', '-');                          \
            return NV ? s : "no-" + s;                                           \
        }                                                                        \
                                                                                 \
        using Base::parse;                                                       \
                                                                                 \
        template <typename V>                                                    \
        bool parse(Parser<Token> * parser, Storage<V> & s) const                 \
        {                                                                        \
            ELL_BEGIN_PARSE                                                      \
            SafeModify<> m1(parser->flags.FLAG, NV);                             \
            ELL_CUSTOM_##FLAG(NV)                                                \
            match = Base::target.parse(parser, s);                               \
            ELL_END_PARSE                                                        \
        }                                                                        \
    };

#   define ELL_CUSTOM_look_ahead(new_value)
#   define ELL_CUSTOM_action(new_value)
#   define ELL_CUSTOM_debug(new_value)

    // Re-enabling skipper in the middle of a grammar needs one more skipping (like at parsing init)
#   define ELL_CUSTOM_skip(new_value)                        \
        if (! m1.sav && new_value)                           \
        {                                                    \
            typename Parser<Token>::Context sav_pos(parser); \
            parser->skip();                                  \
            match = Base::target.parse(parser, s);           \
            if (! match)                                     \
                sav_pos.restore(parser);                     \
        }                                                    \
        else                                                 

    ELL_PARSER_FLAGS
#   undef ELL_FLAG

    template <typename Token, typename Child>
    struct NCs : public UnaryNode<Token, NCs<Token, Child>, Child>
    {
        typedef UnaryNode<Token, NCs<Token, Child>, Child> Base;

        NCs(const Child & target)
          : Base(target)
        { }

        std::string get_kind() const { return "no-consume"; }

        using Base::parse;
        template <typename V>
        bool parse(Parser<Token> * parser, Storage<V> & s) const
        {
            ELL_BEGIN_PARSE
            typename Parser<Token>::Context sav_pos(parser);
            match = Base::target.parse(parser, s);
            sav_pos.restore(parser);
            ELL_END_PARSE
        }
    };

    /// This class allows to define a new grammar terminator.
    /// When parsing parsing through its children, skipper is disabled and stepping back allowed.
    template <typename Token, typename Child>
    struct Lx : public UnaryNode<Token, Lx<Token, Child>, Child>
    {
        typedef UnaryNode<Token, Lx<Token, Child>, Child> Base;

        Lx(const Child & target)
          : Base(target)
        { }

        std::string get_kind() const { return "lexeme"; }

        using Base::parse;
        template <typename V>
        bool parse(Parser<Token> * parser, Storage<V> & s) const
        {
            ELL_BEGIN_PARSE
            SafeModify<> m1(parser->flags.look_ahead, true);
            SafeModify<> m2(parser->flags.skip, false);
            match = Base::target.parse(parser, s);
            ELL_END_PARSE
        }
    };

    /// Compile-time min/max repeat
    template <typename Token, typename Child, const int Min, const int Max>
    struct Rp : public UnaryNode<Token, Rp<Token, Child, Min, Max>, Child>
    {
        typedef UnaryNode<Token, Rp<Token, Child, Min, Max>, Child> Base;

        using Base::target;
        using Base::parse;

        Rp(const Child & target)
          : Base(target)
        { }

        std::string get_kind() const { return "repeat"; }

#       define MIN Min
#       define MAX Max
#       include "RepeatMatchTpl.h"

        std::string get_value() const
        {
            std::ostringstream os;
            os << Min << ',' << Max;
            return os.str();
        }
    };

    /// Run-time min/max repeat
    template <typename Token, typename Child, typename CP>
    struct DRp : public UnaryNode<Token, DRp<Token, Child, CP>, Child>
    {
        typedef UnaryNode<Token, DRp<Token, Child, CP>, Child> Base;

        using Base::target;
        using Base::parse;

        DRp(const Child & target,
                  const int CP::*min,
                  const int CP::*max)
          : Base(target),
            min(min),
            max(max)
        { }

        std::string get_kind() const { return "dynamic-repeat"; }

#       define MIN (((CP *) parser)->*min)
#       define MAX (((CP *) parser)->*max)
#       include "RepeatMatchTpl.h"

        std::string get_value() const { return "?,?"; }

    private:
        const int CP::*min;
        const int CP::*max;
    };

    template <typename Token, typename Child, typename ConcreteParser, typename Var, typename Value>
    struct Act
    { };

    namespace
    {
        template <typename CP, typename V>
        bool make_action(CP * parser, V CP::*var, Storage<V> & s)
        {
            parser->*var = s.value;
            return true;
        }

        template <typename CP, typename V>
        bool make_action(CP * parser, void(CP::*method)(const V &), Storage<V> & s)
        {
            (parser->*method)(s.value);
            return true;
        }

        template <typename CP, typename V>
        bool make_action(CP * parser, bool(CP::*method)(const V &), Storage<V> & s)
        {
            return (parser->*method)(s.value);
        }

        template <typename CP, typename V>
        bool make_action(CP * parser, void(CP::*method)(V), Storage<V> & s)
        {
            (parser->*method)(s.value);
            return true;
        }

        template <typename CP, typename V>
        bool make_action(CP * parser, bool(CP::*method)(V), Storage<V> & s)
        {
            return (parser->*method)(s.value);
        }

        template <typename CP>
        bool make_action(CP * parser, void(CP::*method)(), Storage<void> &)
        {
            (parser->*method)();
            return true;
        }

        template <typename CP>
        bool make_action(CP * parser, bool(CP::*method)(), Storage<void> &)
        {
            return (parser->*method)();
        }
    }

    template <typename Token, typename Child, typename ConcreteParser, typename Var, typename Value>
    struct Act<Token, Child, ConcreteParser, Var ConcreteParser::*, Value>
      : public UnaryNode<Token, Act<Token, Child, ConcreteParser, Var ConcreteParser::*, Value>, Child>
    {
        typedef UnaryNode<Token, Act<Token, Child, ConcreteParser, Var ConcreteParser::*, Value>, Child> Base;

        Act(const Child & target, Var ConcreteParser::*v)
          : Base(target),
            var(v)
        { }

        std::string get_kind() const { return "action"; }

        using Base::parse;

        template <typename V>
        bool parse(Parser<Token> * parser, Storage<V> & s) const
        {
            ELL_BEGIN_PARSE
            if (parser->flags.action)
            {
                Storage<Value> sa;
                typename Parser<Token>::Context sav_pos(parser);

                match = Base::target.parse(parser, sa);
                if (match)
                {
                    match = make_action((ConcreteParser *) parser, var, sa);
                    if (match)
                        assign(s, sa);
                    else
                        sav_pos.restore(parser);
                }
            }
            else
                match = Base::target.parse(parser);
            ELL_END_PARSE
        }

    private:
        Var ConcreteParser::*var;
    };
}

#endif // INCLUDED_ELL_UNARY_NODES_H
