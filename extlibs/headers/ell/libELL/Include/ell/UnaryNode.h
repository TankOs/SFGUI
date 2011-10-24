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

#ifndef INCLUDED_ELL_UNARY_NODE_H
#define INCLUDED_ELL_UNARY_NODE_H

#include <ell/Rule.h>

namespace ell
{
    template <typename Token, typename ConcreteNode, typename UnaryFlavour>
    struct UnaryNodeBase : public ConcreteNodeBase<Token, ConcreteNode>
    {
        const Node<Token> * get_child_at(int index) const
        {
            if (index == 0)
                return & ((const UnaryFlavour *) this)->target;
            return 0;
        }
    };

    template <typename Token, typename ConcreteNode, typename Child>
    struct UnaryNode : public UnaryNodeBase<Token, ConcreteNode, UnaryNode<Token, ConcreteNode, Child> >
    {
        UnaryNode(const Child & child)
          : target(child)
        { }

        Child target;
    };

    template <typename Token, typename ConcreteNode>
    struct UnaryNode<Token, ConcreteNode, Rule<Token> >
      : public UnaryNodeBase<Token, ConcreteNode, UnaryNode<Token, ConcreteNode, Rule<Token> > >
    {
        UnaryNode(const Rule<Token> & child)
          : target(child)
        { }

        const Rule<Token> & target;
    };
}

#endif // INCLUDED_ELL_UNARY_NODE_H
