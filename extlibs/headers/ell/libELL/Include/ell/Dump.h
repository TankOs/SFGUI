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

#ifndef INCLUDED_ELL_DUMP_H
#define INCLUDED_ELL_DUMP_H

#include <ell/Node.h>

namespace ell
{
    template <typename Token>
    std::string dump(const Node<Token> & node, bool need_parens)
    {
        std::string kind = node.get_kind();
        std::string value = protect(node.get_value());

        if (kind == "rule")
        {
            if (value.empty())
                return dump(* node.get_child_at(0), need_parens);
            else
                return node.get_value();  // Want unprotected
        }

        std::string binop;

        if (kind == "alternative" || kind == "longest")
            binop = " or ";
        else if (kind == "combination")
            binop = " combined with ";
        else if (kind == "aggregation")
            binop = " ";
        else if (kind == "exclusion")
            binop = " but not ";
        else if (kind == "list")
            binop = "+ separated by ";
        else if (kind == "bound-repetition")
            binop = " until ";
        else if (kind == "no-suffix")
            binop = " not followed by ";

        if (binop.size())
        {
            const Node<Token> & left = * node.get_child_at(0);
            const Node<Token> & right = * node.get_child_at(1);
            std::string s = dump(left, left.get_kind() != kind)
                          + binop
                          + dump(right, right.get_kind() != kind);
            if (need_parens)
                s = '(' + s + ')';
            return s;
        }

        const Node<Token> * target = node.get_child_at(0);

        if (target)
        {
            if (value.empty())
            {
#               if ELL_DEBUG == 1 && ELL_DUMP_ACTIONS == 1
                return kind + '(' + dump(* target, false) + ')';
#               else
                return dump(* target, need_parens);
#               endif
            }
            else
            {
                return kind + '(' + dump(* target, false) + ',' + value + ')';
            }
        }

        if (kind == "char")
            return value;
        else if (kind == "charset" || kind == "range")
            return '[' + value + ']';
        else if (kind == "error")
            return "error(" + value + ')';
        else if (kind == "string")
            return '"' + value + '"';
        else if (kind == "ignore-case-string")
            return "icase(\"" + value + "\")";
        else if (kind == "keyword" || kind == "ignore-case-keyword")
            return "'" + value + "' keyword";

        return kind;
    }

    template <typename Token>
    inline std::ostream & operator << (std::ostream & os, const Node<Token> & n)
    {
        return os << dump(n, false);
    }
}

#endif // INCLUDED_ELL_DUMP_H
